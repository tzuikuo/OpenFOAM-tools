/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | www.openfoam.com
     \\/     M anipulation  |
-------------------------------------------------------------------------------
    Copyright (C) 2013-2016 OpenFOAM Foundation
    Copyright (C) 2016-2020 OpenCFD Ltd.
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

SourceFiles
    alphaWaterRegionIntegral.C

\*---------------------------------------------------------------------------*/

#ifndef functionObjects_alphaWaterRegionIntegral_H
#define functionObjects_alphaWaterRegionIntegral_H

#include "fieldExpression.H"
#include "writeFile.H"
#include "cartesianCS.H"
#include "volFieldsFwd.H"
#include "volFields.H"
#include "HashSet.H"
#include "Tuple2.H"
#include "OFstream.H"
#include "Switch.H"
#include "uniformDimensionedFields.H"
#include "volFieldSelection.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{
namespace functionObjects
{

/*---------------------------------------------------------------------------*\
                          Class myCenterOfMass Declaration
\*---------------------------------------------------------------------------*/

class alphaWaterRegionIntegral
:
    public fieldExpression,
    public writeFile
{
protected:

    // Protected Data

        //- Alpha threshold
        scalar threshold_;

        //- Region origin point
        point origin_;
        point originSmall_;

        //- Region radius (for spherical/cylindrical regions)
        scalar radius_;

        //- Region height (for box/cylindrical/truncatedCone regions)
        scalar height_;
        scalar length_;
        scalar width_;

        //- Region R (for truncatedCone regions)
        scalar radiusLarge_;
        scalar radiusSmall_;

        //- Region type
        word regionType_;


    // Protected Member Functions

        //- Check if cell center is in target region
        bool inTargetRegion(const point& p) const;

        //- Calculate the integral
        virtual bool calc();
        


public:

    //- Runtime type information
    TypeName("alphaWaterRegionIntegral");

    // Constructors

        //- Construct from Time and dictionary
        alphaWaterRegionIntegral
        (
            const word& name,
            const Time& runTime,
            const dictionary& dict
        );

        //- No copy construct
        alphaWaterRegionIntegral(const alphaWaterRegionIntegral&) = delete;

        //- No copy assignment
        void operator=(const alphaWaterRegionIntegral&) = delete;


    //- Destructor
    virtual ~alphaWaterRegionIntegral() = default;


    // Member Functions

        //- Read the input data
        bool read(const dictionary& dict);

};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace functionObjects
} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
