## Curiosity task - 3D object viewer

We are not doing Pacman. Instead we are doing a Curiosity Task.
We are focusing heavily on Architecture/Framework development for an OpenGL
graphics engine.

The goal of our engine is to provide a 3D viewer with loading of resource files
during run-time. The 3D-viewer will support 5 different files which may be
loaded, edited, re-loaded during run-time by the user:
- Texture files   (e.g. brickwall.jpg)
- Shader files    (e.g. base.glsl)
- Material files  (e.g. brickwall.yml  - custom format)
- Model files     (e.g. cube.yml       - custom format)
- Scene files     (e.g. playground.yml - custom format)

We are focusing on being as generic as possible when supporting these files,
to empower the user. We are trying to make it possible for non-programmers
to enjoy the 3D-viewer, although the text-only input system might scare some
people away.

Our target user is an educator or a student who wants to learn about 3D
graphics datastructures.

To make our custom Model format usable, we want to write an exporter for Blender
for exporting high-poly models to our custom format.

We want to model an entire scene graph.


## Install Instructions

#### Windows

#### MacOS

#### Linux
Since we are using c++17 features, a newer compiler is necessary.
We are also using python3 or above.
```bash
    sudo apt-get install clang++-5.0
    sudo apt-get install python3.5
    #TODO insert install instructions for cmake 11.

    git clone <REPO LINK>
    cd imt2531-assignment2
    mkdir build
    cd build

    cmake .. -DCMAKE_CXX_COMPILER=clang++-5.0
    make

    ./cube
    ./cube assets/scenes/demo1.yml # Load specific scene.
```

## Roadmap

### Minimum Requirements assignment 2

- [x] Textured walls floors
- [ ] Menu for pause, resume and exit. (pull from assignment 1?)
- [x] Move objects/camera using keyboard or mouse.
- [x] Load at least 1 model from a model file. (custom format)
- [x] Basic lighting model

### Optional requirement assignment 2

- [x] Advanced lighting/reflection materials
- [x] Change perspective
- [x] Use of multiple advanced mesh.
- [x] Write own object loader
- [x] Make Camera movable
- [x] Multiple scenes.


### Own requirements Engine

- [x] Render a cube. <br>
- [x] Add texture to the cube <br>
- [x] Make the cube spin. <br>
- [x] Add material to the cube. <br>
- [x] Use 2 materials on different parts of the cube. <br>
- [x] Move the cube using mouse input. <br>
- [x] Add floor. <br>
- [x] Texturize the floor. <br>
- [x] Toggle camera spin around the cube. <br>
- [x] Toggle ambient light. Scale ambient light up down. <br>
- [x] Add a directional light. <br>
- [x] Implement Phong lighting model using cube vertex normals. <br>
- [x] Render cube with specular map. <br>
- [ ] Add one of the simple shadow models. <br>
- [x] Swap between 2 different shaders at run-time. <br>
- [x] Swap between 2 different cube materials at run-time. <br>
- [x] Render 2 cubes side by side with different materials. <br>
- [x] Repeat the above with model of the teapot. <br>

### Extra File loader
- [x] Load a Shader. (vertex & geometry & fragment) <br>
- [x] Load a Texture. (diffuse & specular) <br>
- [x] Load a Vertexbuffer <br>
- [x] Load a Mesh with indicies to that vertexbuffer. <br>
- [x] Load a Material.  <br>
- [x] Load a Transform.  <br>
- [x] Load an Entity { Mesh, Material, Transform }  <br>
- [x] Render that loaded Entity.  <br>
- [x] Update render of loaded Entity on file change <br>




## Reflection

### Things we would have done differently

#### Decouple vertex-mesh from material and shader

Currently in our model-files we are specifying which shader and material are to be used in the rendering of our models.
This has created a strong coupling of a mesh to the shader and material. Stronger than we would like.

I will mention two big problems with this:

1. It is not convenient to go inside the model file, in between all the vertices and triangle to edit the materials and shaders attached to that model.
2. For every new model with the same vertices, but with different materials, we have to duplicate all the vertices and indicies. This does not make sense for a big model.

```yml
vertices: 1560
v:  1.138500  0.598000 -0.100000    0.707 -0.707  0.000    0.000  0.000   255 255 255 255
v:  1.138500  0.598000  0.100000    0.707 -0.707  0.000    0.000  0.000   255 255 255 255
v:  1.138500  0.384000 -0.100000    0.707  0.707  0.000    0.000  0.000   255 255 255 255
v:  1.138500  0.384000  0.100000    0.707  0.707  0.000    0.000  0.000   255 255 255 255
.... 1000's more vertices

meshes: 1
mesh: blendermesh
material: _default   # < -- Awkward to have to go here to edit material here in between 1000's of lines of data.
shader: _default
triangles: 1166
t: 64 92 65
t: 92 66 65
t: 92 0 66
t: 92 1 0
t: 92 2 1
t: 92 3 2
t: 92 4 3
t: 92 5 4
.... 1000's more triangles
```

The entity specifies that it wants to use a model. This implicitly also select which materials and shaders are to be used.
An intermediate format like a `prefab model` would have been nice.

A prefab would then stich everything together. An entity would be an instance of a prefab with a transform in a scene.

```yml

prefabs: 2

prefab: cube
meshes: 4

    mesh: rightarm
    shader: basic-color
    material: red

    mesh: leftarm
    shader: basic-color
    material: blue

    mesh: head
    shader: mat-texture
    material: _default

    mesh: foot
    shader: mat-texture
    material: brick


prefab: Suzanne
meshes: 1

    mesh: suzanne-head
    shader: subsurface-scatter
    material: glass

```
