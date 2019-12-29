# Astral Engine

Astral Engine is created by 2 students from UPC-CITM for the Videogame engines subject for educational purposes.

## Installation

Download the latest release [here](https://github.com/Scarzard/Astral_Engine/releases). Decompress the .zip and execute Astral_Engine v_X.X
and play around with the engine

## Controls

- Holding Left Shift: Duplicates any movement.

- While **Mouse Right Click is down:**

- Dragging mouse: Move camera in editor.

- W: Move the camera upwards.

- A: Move the camera to the left.

- S: Move the camera donwards.

- D: Move the camera to the right.

- While **Mouse Left Alt is down:**

- Right click and mouse drag: Zoom in/out.

- Left click and mouse drag: Orbit camera.

## Tasklist

### Both members:

- Game Objects

- Decoupling Desing Pattern for Component creation.

- Engine overall aesthetic.

- Animation importing with Assimp.

- Skeleton animation.

### Josep Lleal

- Hierarchy/Console/Resource window

- Every propietary file format.

- Scene serialization.

- Resource manager. Refactor for Components to use resources.

- Space partition as Quadtrees.

- Importing

- Animation component

- Bone parenting.

- Animation interpolation.

- Animation blending.

### Victor Chen

- Game/Inspector/Configuration window

- Interactive transformations in Inspector.

- AABBs.

- Frustum culling.

- Camera refactor to use frustums.

- Time manager and Play/Stop/Pause buttons

- Bone Component.

- Skinning. 

### High-level system: Skeletal Animation

- First of all, we imported all the animation information using Assimp, primarily using .fbx files.

- When loading a gameObject, the engine will try to find if the imported object has any animations attached to it, if it doesn't, it will load as a normal gameObject.
If it does, it will load the animations into memory and creating the corresponding ComponentAnimation. It will also create a skeleton using ComponentBones, then linking the
bones to the affected mesh.

- When animating the gameObject, the engine stores the transformations over time (position, rotation and scale) of the bones that we will be moving, thus affecting the geometry of our mesh.

- Once we have the mesh and skeleton (made of ComponentBones), they are linked by their ID, we duplicate all the geometry of our mesh by creating a temporal resource that will be used to repose our gameObject.

- When updating it, the engine gets the stored transformations and applies the resulting movement. This movement is achieved by interpolating from Key A to B, using a Linear Interpolation (Lerp) for positions and scales. It will use Spherical Interpolations (Slerp) for rotations.

- The blending of the animations is achieved by storing temporarily the actual position of the current animation and interpolating it to the first frame of the next animation, then the next animation will take over.


## Github

As stated on the license, anyone can look at or modify the code of this project. 
It can be found [here](https://github.com/Scarzard/Astral_Engine)

## Authors
   
Josep Lleal
   - [Josep's GitHub Link](https://github.com/JosepLleal)
   
Victor Chen Chen
   - [Victor's GitHub Link](https://github.com/Scarzard)
   
# License:

MIT License

Copyright (c) 2019 Josep Lleal and Victor Chen

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.