# This is a preProcessing tool

## Features

- createBafflesDict: create baffle from faceZones
- createPatchDict-deleteWall: delete wall (change type from wall to empty) so that you don't need to set boundary conditions for it
- createPatchDict-makeWall: create cyclic wall from set (maybe not work)
- topoSetDict-boundary2facesSet: create faceSet from boundary (i.e. patch)
- topoSetDict-boundary2facesZoneSet: create faceZoneSet from boundary (i.e. patch)
- topoSetDict-createCyclic: create faceSet and cellSet from geometry (here cylinder), and create cyclic boundary from the Set.

- genBoundaryFile.m: matlab scripte for batch set the boundary conditions
- genCreatePatchFile.m: matlab scripte for creating the createPatchDict to delete wall

It is for the inner wall (shared) as wall not inner wall in boundary condition.
  
## Usage

change the Dict name above with no "-XXX" at the last

```bash
cd caseFolder
createPatch -overwrite
topoSet -overwrite
createBaffles -overwrite
```