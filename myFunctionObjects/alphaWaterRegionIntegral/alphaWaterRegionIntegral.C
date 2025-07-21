/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2013-2016 OpenFOAM Foundation
    Copyright (C) 2019-2020 OpenCFD Ltd.
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

#include "alphaWaterRegionIntegral.H"
#include "fvcGrad.H"
#include "addToRunTimeSelectionTable.H"
#include "fvCFD.H"
#include "immiscibleIncompressibleTwoPhaseMixture.H"

// * * * * * * * * * * * * * * Static Data Members * * * * * * * * * * * * * //

namespace Foam
{
namespace functionObjects
{
    defineTypeNameAndDebug(alphaWaterRegionIntegral, 0);
    addToRunTimeSelectionTable(functionObject, alphaWaterRegionIntegral, dictionary);
}
}


// * * * * * * * * * * * * * Private Member Functions  * * * * * * * * * * * //

bool Foam::functionObjects::alphaWaterRegionIntegral::inTargetRegion
(
    const point& p
) const
{
    if (regionType_ == "sphere")
    {
        return (mag(p - origin_) < radius_);
    }
    else if (regionType_ == "cylinder")
    {
        const vector r = p - origin_;
        const scalar rXY = sqrt(sqr(r.x()) + sqr(r.y()));
        return (rXY < radius_) && (mag(r.z()) < height_/2);
    }
    else if (regionType_ == "box")
    {
        return 
            (p.x() > origin_.x() - length_/2) && 
            (p.x() < origin_.x() + length_/2) &&
            (p.y() > origin_.y() - width_/2) && 
            (p.y() < origin_.y() + width_/2) &&
            (p.z() > origin_.z() - height_/2) && 
            (p.z() < origin_.z() + height_/2);
    }
    else if(regionType_=="TruncateCone")
    {
        scalar h0 = fabs(p.z() - originSmall_.z());
        scalar x=(radiusLarge_-radiusSmall_)/height_*h0;
        if(fabs(p.z()-origin_.z())>(height_/2.0)) return false;
        else{
            if(fabs(p.x()-origin_.x())<x) return true;
            else return false;
        }
    }
    else{
        return false;
    }
}


bool Foam::functionObjects::alphaWaterRegionIntegral::calc()
{
    if (foundObject<volScalarField>(fieldName_))
    {
        const volScalarField& alpha =
            lookupObject<volScalarField>(fieldName_);
        scalar waterVolume = 0;

        forAll(mesh_.C(), celli)
        {
            const scalar alphaVal = alpha[celli];
            const scalar cellVol = mesh_.V()[celli];
            const point& cellCenter = mesh_.C()[celli];

            if (inTargetRegion(cellCenter) && alphaVal > threshold_)
            {
                waterVolume += alphaVal * cellVol;
            }
        }

        reduce(waterVolume, sumOp<scalar>());

        if (Pstream::master())
        {

            Log << type() << " " << name() << " output:" << nl
                << "    Water volume in region : " << waterVolume << endl;

            writeCurrentTime(file());
            file()
                << tab << waterVolume << endl;
        }
        Info<<"ok!!!!!!!!!"<<endl;
        return true;
    }
    else{
        Info<<"error!!!!!!"<<endl;
        return false;
    }
}


// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

Foam::functionObjects::alphaWaterRegionIntegral::alphaWaterRegionIntegral
(
    const word& name,
    const Time& runTime,
    const dictionary& dict
)
:
    fieldExpression(name, runTime, dict, fieldName_),
    writeFile(mesh_, name, typeName, dict),
    threshold_(0.0),
    origin_(Zero),
    originSmall_(Zero),
    radius_(0.0),
    height_(0.0),
    length_(0.0),
    width_(0.0),
    radiusLarge_(0.0),
    radiusSmall_(0.0),
    regionType_("TruncateCone")
{
    read(dict);
    setResultName(typeName, fieldName_);
}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

bool Foam::functionObjects::alphaWaterRegionIntegral::read
(
    const dictionary& dict
)
{
    writeFile::read(dict);

    dict.readIfPresent("threshold", threshold_);
    dict.readIfPresent("origin", origin_);
    dict.readIfPresent("originSmall", originSmall_);
    dict.readIfPresent("radius", radius_);
    dict.readIfPresent("height", height_);
    dict.readIfPresent("length", length_);
    dict.readIfPresent("width", width_);
    dict.readIfPresent("radiusLarge", radiusLarge_);
    dict.readIfPresent("radiusSmall", radiusSmall_);
    dict.readIfPresent("regionType", regionType_);

    // Create output file header
    if (Pstream::master())
    {
        writeHeader(file(), "alpha.water volume integral");
        writeCommented(file(), "Time");
        writeTabbed(file(), "WaterVolume");
        file() << endl;
    }

    return true;
}



// ************************************************************************* //
