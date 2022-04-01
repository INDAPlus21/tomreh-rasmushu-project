# Project Rayman

## Overview

This project is a real time 3D fractal rendering engine using ray marching. 

The algorithm used will more specifically be sphere tracing which is a type of ray marching that works especially well for fractal rendering as it relys on SDFs (signed distance functions). Ray marching is similar to ray tracing in many ways but can be better suited for objects where a SDF can be calculated as it does not require multiple samples per pixel to achieve soft shadows and other lighting effects. Furthermore, these SDFs can be calculated for both simple primitive types and many types of fractals which makes it ideal to use for a project like this one. 

Other visual features such as post processing effects and more game-related feaures may be added, depending on the time it takes to develop the fractal renderer itself. Potential post processing effects are gamma correction, exposure settings, contrast settings, bloom and depth of field. The engine may also be used for some kind of simple game if there is enough time but that idea will be further planned if there is time left.

## Naming conventions

The same naming is used for *issues* and *commits*, which is uppercase first letter and future tense.
> Example: "Add OpenGL context and window"

The naming convention used for *branches* is lowercase letters only with hyphens connecting words.
> Example: "cross-platform"

## Time distribution

It is difficult to plan how the time will be distributed because we have not implemented ray marching or fractal rendering before, which makes it difficult to predict how much time will be spent on a given task. We should have time to implement at least a few of the post processing effects based on our current research on the topic but we are not sure if it will be possible to implement some form of game play.

## Work distribution

The work will be equally distributed and we will try to work on the more complex and interesting parts together as mush as possible. Some of the simpler tasks will be split up to save time, as an example may one person work on loading shaders while the other person works on creating vertex buffers. But the ray marching will be developed together as it is important that both parts contribute and understands the code.
