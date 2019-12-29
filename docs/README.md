# Astral Engine

Astral Engine is created by 2 students from UPC-CITM for the Videogame engines subject for educational purposes. 

The main goal is to code our own simple game engine from scratch using various external libraries, and for the last delivery, we must implement a high level system, **skeletal animation**.

## Installation

Download the latest release [here](https://github.com/Scarzard/Astral_Engine/releases). Decompress the .zip and execute Astral_Engine v_1.0
and play around with the engine

## Tasklist

### Both members:

-

### Josep Lleal
- 

### Victor Chen

- Interactive component transform through Inspector window.

- Game Object bounding boxes.

- Frustum culling.

- Camera refactor to use frustums.

- Time manager and Play/Stop/Pause buttons

###  Main Core Sub-systems

- The hierarchy shows all the game objects even if they are unactive. It also shows all its childs. The information of each gameObject is shown in the inspector window when selecting it from the hierarchy.
It can have any number of coded components.

- You can delete a gameObject by selecting it on the hierarchy and pressing DELETE button. If it has any childs, they will also be deleted.

- We have our own file format and resources for each component to reduce load in memory.

- The static objects in the scene are divided with QuadTrees.

- The camera can cull static objects.

- 



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