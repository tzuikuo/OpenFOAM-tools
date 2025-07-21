# rotateAndTranslateMesh

This folder contains utility functions for performing mesh transformations by applying rotation and translation operations.  
These tools are designed to move OpenFOAM meshes according to given attitude (rotation) matrices and translation vectors.

## Features

- Apply rigid body rotation to mesh points using input rotation matrices  
- Translate mesh points by specified vectors  
- Support reading motion data from external files (e.g., CSV or TXT)  
- Automate mesh movement for post-processing

## Usage

1. Prepare your input motion data files (rotation matrices and translation vectors) in the required format.

## Usage Description (File Format)

The input files should follow the OpenFOAM read file format.

### For List of Tuple2<scalar, Vector<vector>> (e.g., `timeValues` from a matrix file):

```foam
timeValues
(
    matrixFile
);
```

The format of matrixFile is:
```plaintext
60         // total number of time points
(
    (0.1 (()()()))  // Tuple2: time and 3x3 matrix in OpenFOAM style
    (0.1 (()()()))
    ...
)            // end of list
```
### For List of Tuple2<scalar, vector> (e.g., timeValues from a vector file):

The format of vectorFile is:
```plaintext
60         // total number of time points
(
    (0.1 ())    // Tuple2: time and 3D vector in OpenFOAM style
    (0.1 ())
    ...
)            // end of list
```

2. Compile the function using `wclean` and `wmake` inside the `Make` folder:

```bash
cd rotateAndTranslateMesh/Make
wclean
wmake
```

3. In your OpenFOAM case folder (outside the rotateAndTranslateMesh folder), run the function with:
```bash
rotateAndTranslateMesh
```