# alphaWaterRegionIntegral 
This is a custom OpenFOAM functionObject used to calculate the fluid volume fraction (e.g., for alpha.water) inside an irregular region such as a cylinder, truncated cone, sphere, box, or a user-defined expression.

It is intended for post-processing and can be run after case completion to extract regional liquid content information.

# Compilation
Compile the function using `wclean` and `wmake libso` inside the `Make` folder:

```bash
cd ralphaWaterRegionIntegral/Make
wclean
wmake libso
```
Make sure the resulting libalphaWaterRegionIntegral.so is copied or linked into your OpenFOAM lib directory or case libs path.

# Example Usage
1. Create configuration file
Place a file named waterIntegral_cylinder in your case’s system folder:
   
Cylinder Example
```foam
waterIntegral_cylinder
{
    type        alphaWaterRegionIntegral;
    libs        ("libalphaWaterRegionIntegral.so");

    field       alpha.water;
    // write control
    writeControl    timeStep;
    writeInterval   500;

    threshold   0.5;         // threshold is used to define the minimum value of alpha.water to consider the cell as "liquid".
    regionType  cylinder;  // region type（TruncateCone/expression/sphere/cylinder/box）cylinder;
    
    // set your region
    origin      (-0.28284 -0.28284 -1.674865);     // geometry center
    originSmall (0 0 0);     // The origin of the smaller top of the TruncateCone (if other type it is not used) 
    radius      0.245;         // sphere/cylinder radius (if other type not used) 
    radiusLarge 0;         // The larger radius of the TruncateCone
    radiusSmall 0;         // The smaller radius of the TruncateCone
    height      0.16649;         // height of the cylinder/box (if other type not used)
    length      0.0;         // box length (if other type not used)
    width       0.0;         // box width (if other type not used)
}
```
TruncateCone Example

```foam
waterIntegral_truncateCone
{
    type        alphaWaterRegionIntegral;
    libs        ("libalphaWaterRegionIntegral.so");

    field       alpha.water;
 
    writeControl    timeStep;
    writeInterval   500;

   
    threshold   0.5;     
    regionType  TruncateCone;  // region type（TruncateCone/expression/sphere/cylinder/box）cylinder;

    origin      (-0.28284 -0.28284 -1.88746);     
    originSmall (-0.28284 -0.28284 -1.88811);     
    radius      0;         
    radiusLarge 0.245;         
    radiusSmall 0.01;         
    height      0.13;         
    length      0.0;         
    width       0.0;         
}
```
2. Include in system/controlDict

```foam
functions
{
    #include "waterIntegral_cylinder"
    #include "waterIntegral_truncateCone"
}
```

# Notes

- threshold is used to define the minimum value of alpha.water to consider the cell as "liquid".
- The region is defined geometrically and only cells within this region contribute to the integral.
- Multiple regions can be evaluated simultaneously by including multiple functionObject blocks.
- Requires successful compilation of libalphaWaterRegionIntegral.so.