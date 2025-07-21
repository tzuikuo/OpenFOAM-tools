/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2011-2017 OpenFOAM Foundation
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

\*---------------------------------------------------------------------------*/

#include "fvCFD.H"
#include "Time.H"
#include "IFstream.H"
#include "OFstream.H"
#include "HashTable.H"
#include "List.H"
#include "Tuple2.H"
#include "vector.H"
#include "Vector.H"
#include "tensor.H"

using namespace Foam;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //
// Main program:

int main(int argc, char *argv[])
{
    #include "setRootCase.H"
    #include "createTime.H"
    //#include "createMesh.H"

    vector rotCenter0=vector(0,0,14.2);//global rotcenter


    // rotation matrices and translation vectors
    HashTable<Vector<vector>> rotationMatrices;
    HashTable<vector> translationVectors;

    IFstream matrixFile("rotateAndTranslate/rotationMatrices.dat");
    IFstream translationFile("rotateAndTranslate/translationVectors.dat");

    //
    if (matrixFile.good())
    {
        List<Tuple2<scalar, Vector<vector>>> timeValues
        (
            matrixFile
        );
        Info<<"read rotation matrices ok!"<<endl;

        forAll(timeValues, i)
        {
            scalar time=timeValues[i].first(); 
            Vector<vector> R=timeValues[i].second();
            rotationMatrices.insert(Foam::name(time), R);
        }
    }
    else
    {
        FatalErrorInFunction
            << "Cannot open file rotationMatrices.dat"
            << exit(FatalError);
    }

    //
    if (translationFile.good())
    {
        List<Tuple2<scalar, vector>> timeValues
        (
            translationFile
        );
        Info<<"read translation vectors ok!"<<endl;

        forAll(timeValues, i)
        {
            scalar time=timeValues[i].first(); 
            vector trans=timeValues[i].second();
            translationVectors.insert(Foam::name(time), trans);
        }
    }
    else
    {
        FatalErrorInFunction
            << "Cannot open file translationVectors.dat"
            << exit(FatalError);
    }

    fvMesh origMesh // import original global mesh
    (
        IOobject
        (
            fvMesh::defaultRegion,
            "constant", 
            runTime, 
            IOobject::MUST_READ
        )
    );
    const pointField& origPoints = origMesh.points(); // save original mesh

    // get all runtimes
    instantList timeDirs = runTime.times();

    // iterate all runtimes
    forAll(timeDirs, timeI)
    {
        runTime.setTime(timeDirs[timeI], timeI);
        Info << "\nProcessing time: " << runTime.timeName() << endl;

        // read current time mesh
        fvMesh mesh
        (
            IOobject
            (
                fvMesh::defaultRegion,
                runTime.timeName(),
                runTime,
                IOobject::MUST_READ
            )
        );

        // iniatial the matrix and vector
        tensor Rot = tensor::I;  
        vector Trans = Zero;  
        
        // get matrix and vector from the file
        if (rotationMatrices.found(Foam::name(runTime.value())))
        {
            Rot = rotationMatrices[Foam::name(runTime.value())];
            Info << "Using predefined matrix:\n" << Rot << endl;
        }
        else
        {
            Info << "No matrix defined, using identity matrix" << endl;
        }
        //
        if (translationVectors.found(Foam::name(runTime.value())))
        {
            Trans = translationVectors[Foam::name(runTime.value())];
            Info << "Using predefined vector:\n" << Trans << endl;
        }
        else
        {
            Info << "No vector defined, using zero vector" << endl;
        }

        // 旋转网格点
        pointField points = origPoints; 
        forAll(points, i)
        {
            if(runTime.timeName()=="constant"){
                continue;
            }
            else{
                points[i] = transform(Rot, points[i]-rotCenter0)+rotCenter0;
                //points[i] = points[i]+Trans;
            }
        }

        // update the mesh and write to the current time
        mesh.movePoints(points);
        mesh.write();

        // ange U 
        /*if (isFile(runTime.timePath()/"U"))
        {
            volVectorField U
            (
                IOobject("U", runTime.timeName(), mesh, IOobject::MUST_READ),
                mesh
            );
            U = transform(Rot, U);
            U.write();
            Info << "Rotated vector field U" << endl;
        }
        Info << "Mesh rotated with matrix successfully!" << runTime.value() << endl;
        */
    }
    return 0;
}

// ************************************************************************* //
