# Programmable Pipeline Scene
A 3D Scene made in C++ & DirectX 11 (HLSL) for my programming with shaders module in university. Debugged with RenderDoc.

Implemented using an internal framework, which contains bindings for [Dear ImGui](https://github.com/ocornut/imgui).

![GIF of flying about](TODO)

## Running
![Screenshot of scene with two teapots, and a wall](TODO)
Requires a 64-bit version of Windows with DirectX 11 and Shader Model 5 support. [Download](TODO)

* W, A, S, D to move
* Q, E to move up/down
* ← ↑ → ↓ (arrow keys) to look around
* Use the debug window to adjust parameters. 
    * Note 1: Having more than one part non-collapsed will cause more than one value be edited.
    * Note 2: Some values are unit vectors - there are no restrictions on editing them.

## Key Features
This provides an overview of features. For the full details of implementation (and a reflection), see the [Report](TODO)
### Dynamic Tessellation
![Dynamic tessellation screenshot](TODO)
Changes the height of vertices on a 3D plane by sampling a height map, then changes the number of vertices (via the tessellation stage) dynamically based off how far away the camera is.
### Multiple Shadowed Directional Lights
![Screenshot of Teapot with two shadows](TODO)
Two directional lights are present in the scene, both being shadow-mapped.
### Multiple Point & Spot Lights
![Screenshot of point and spot lights](TODO)
Multiple point and spot lights are present in the scene (not shadowed).
### Bloom Post Process Effect
![Screenshot of bloom](TODO)
The post-process effect is applied directly to the render texture, which takes the brightest parts of the scene, blurs them, then adds them to the render texture.
### Game Object System
Simple system which features entities with overrideable functions (note: not an entity component system).