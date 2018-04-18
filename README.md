# Curiosity task

We are not doing Pacman. Instead we are doing a Curiousity Task.
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

We are focusing on beiing as generic as possible when supporting these files, 
to empower the user. We are trying to make it possible for non-programmers
to enjoy the 3D-viewer, although the text-only input system might scare some
people awy.

Our target user is an educator or a student who wants to learn about 3D 
graphics datastructures.

To make our custom Model format usable, we want to write an exporter for Blender
for exporting high-poly models to our custom format.

We want to model an entire scene graph.

# Roadmap

### Minimum Requirements assignment 2

- [x] Textured walls floors
- [ ] Menu for pause, resume and exit. (pull from assignment 1?)
- [x] Move objects/camera using keyboard or mouse.
- [x] Load at least 1 model from a model file. (custom format)
- [ ] Basic lighting model

### Optional requirement assignment 2

- [ ] Advanced lighting/reflection materials
- [x] Change perspective 
- [ ] Use of multiple advanced models.
- [ ] Write own object loader
- [x] Make Camera movable
- [ ] Multiple scenes
 

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
- [ ] Toggle ambient light. Scale ambient light up down. <br>
- [ ] Add a directional light. <br>
- [ ] Implement Phong lighting model using cube vertex normals. <br>
- [ ] Render cube with specular map. <br>
- [ ] Add one of the simple shadow models. <br>
- [x] Swap between 2 different shaders at run-time. <br>
- [x] Swap between 2 different cube materials at run-time. <br>
- [ ] Render 2 cubes side by side with different materials. <br>
- [ ] Repeat the above with model of the teapot. <br>
- [ ] Do something cool with the geometry/tesselation' shader <br>

### Extra File loader
- [x] Load a Shader. (vertex & geometry & fragment) <br>
- [x] Load a Texture. (diffuse & specular) <br>
- [x] Load a Vertexbuffer <br>
- [x] Load a Mesh with indicies to that vertexbuffer. <br>
- [x] Load a Material.  <br>
- [ ] Load a Transform.  <br>
- [ ] Load an Entity { Mesh, Material, Transform }  <br>
- [ ] Render that loaded Entity.  <br>
- [ ] Update render of loaded Entity on file change <br>