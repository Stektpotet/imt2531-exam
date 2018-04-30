[TOC]
# Curiosity task - 3D object viewer

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
people away.

Our target user is an educator or a student who wants to learn about 3D 
graphics datastructures and systems.

To make our custom model format usable, we want to write an exporter for Blender
for exporting models, i.e. their vertex- and face informaion (position, normal, uv coordinates ...) to our custom format.

We want to model an entire scene graph.


# Install Instructions

## Windows
1. Adding the CMake-module for Visual Studio:
    - Install Visual Studio 2017 15.4 or newer (the CMake module is only available for these versions).
    - From Visual Studio: go to `Tools->Get Tools And Features...`
    - Select the box named `Desktop development with C++`
    - Enable the checkbox named `Visual C++ Tools for CMake` in the righthand column
    - Press `Modify` to apply the module
 2. Install [python 3](https://www.python.org/downloads/)(or newer)
    - Make sure the installation directory is `C:\tools\Python36\python.exe`
 3. Clone [the repository](http://prod3.imt.hig.no/overkill-studios/imt2531-assignment2)
     - from gitbash: `> cd "./yourdesiredworkdir/"`
     - from gitbash: `> git clone http://prod3.imt.hig.no/overkill-studios/imt2531-assignment2.git`
 4. In Visual Studio: go to `File->Open->CMake...`, and select the CMakeLists.txt file located in `./yourdesiredworkdir/imt2531-assignment2/`


##### Additional/Optional setup
 - Running the program with different scenes:
    To start the program from a differet scene all you need to do is specify another configuration in the file named `launch.vs.json`, this can be found in the hidden directory `.vs`, or by going to `CMake->Debug and Launch Settings`.
Setting the args of a configuration to reflect a scene will make that scene open upon building & debugging with said configuration.

## MacOS

## Linux
Since we are using c++17 features, a newer compiler is necessary.
We are also using python3 or above.
```bash
    # Install Clang++ 5 (allowing c++17 features to be compiled)
    sudo apt-get install clang++-5.0
    # Install python3 (NOTE: a lot of ubuntu distros already come with this)
    sudo apt-get install python3.5
    #Remove previous installations of cmake
    sudo apt-get remove --purge cmake
    
    mkdir ~/temp # make a temp folder for the cmake binaries
    cd ~/temp
    
    wget https://cmake.org/files/v3.11/cmake-3.11.0-Linux-x86_64.tar.gz
    # unzip the binaries
    tar -xzvf cmake-3.11.0-Linux-x86_64.tar.gz
    cd ~/temp/cmake-3.11.0-Linux-x86_64/
    
    #place the binaries
    cp -r ./bin     /usr/    
    cp -r ./doc     /usr/share/
    cp -r ./man     /usr/share/
    cp -r ./share   /usr/

    cmake --version         # verify the new installation:
                            # expected: cmake version 3.11.0

    rm -r ~/temp    # cleanup temp directory, it's no longer needed

    git clone http://prod3.imt.hig.no/overkill-studios/imt2531-assignment2.git
    
    cd imt2531-assignment2
    mkdir build
    cd build

    cmake .. -DCMAKE_CXX_COMPILER=clang++-5.0
    make

    ./cube
    ./cube assets/scenes/demo1.yml # Load specific scene.
```



# Controls
```
Reload shaders   - 1
Reload materials - 2
Reload models    - 3
Reload scene     - 4

Move camera:
    Forwards   -  W
    Left       -  A
    Backwards  -  S
    Right      -  D
    Down       -  Q 
    Up         -  E
Rotate camera:
    Pitch      - Mouse up/down
    Yaw        - Mouse left/right
    
Switch camera  - Tab
Switch camera mode - Space

```

# Roadmap

## Minimum Requirements assignment 2

- [x] Textured walls floors
- [ ] Menu for pause, resume and exit. (we were excused for this point as it didn't fit the kind of program we made)
- [x] Move objects/camera using keyboard or mouse.
- [x] Load at least 1 model from a model file. (custom format)
- [x] Basic lighting model

## Optional requirement assignment 2

- [x] Advanced lighting/reflection materials
- [x] Change perspective 
- [x] Use of multiple advanced mesh.
- [x] Write own object loader
- [x] Make Camera movable
- [x] Multiple scenes
 

## Own requirements Engine

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

## Extra File loader
- [x] Load a Shader. (vertex & geometry & fragment) <br>
- [x] Load a Texture. (diffuse & specular) <br>
- [x] Load a Vertexbuffer <br>
- [x] Load a Mesh with indicies to that vertexbuffer. <br>
- [x] Load a Material.  <br>
- [x] Load a Transform.  <br>
- [x] Load an Entity { Mesh, Material, Transform }  <br>
- [x] Render that loaded Entity.  <br>
- [x] Update render of loaded Entity on file change <br>


# Extended feature list

## 1. Rendering of 3D models.
3D models are rendered in a GLFW window. Rendering 3D models in it's simplest form is just pushing each individual vertex down the 3D rendering pipeline of the GPU using shaders. In OpenGL this is achieved by writing -> loading -> compiling -> linking -> buffering -> binding and running the following shader minimum.

```c=
#shader vertex
#version 150

in vec4 position;
in vec4 color;
out vec4 color_from_vshader;

void main() {
	gl_Position = position;
	color_from_vshader = color;
}

#shader fragment
#version 150

in vec4 color_from_vshader;
out vec4 out_color;

void main() {
	out_color = color_from_vshader;
}
```

We expanded quite a bit more than just rendering 3D models.
Some of the features you will find in our shaders are:
* Texture mapping
* Bump mapping (normal mapping)
* Ambient lighting
* Diffuse lighting
* Specular lighting
* Subsurface scattering

 ## 2. Loading of 3D models from custom simplified format.
The costum file format in yaml that consists of a vertex count, then a list of vertecies where each define a point in space, its normal vector, its UV coordintes and lastly color. <br>

```yaml
vertices: 1560
#      x   y    z     nx   ny     nz    u   v    r   g   b   a 
v:  1.24 0.65 0.10  -0.85 -0.52  0.00  0.0 0.0  255 255 255 150
v:  1.27 0.67 0.10 ......
v:  1.30 0.60 0.10 .......
...

```
Then a mesh count, describing how many meshes are to come.<br>
Each mesh has its name, material, shader and count of how many triangles it consists of. Each triangle lists three vertex indecies to define it.
 
```yaml
meshes: 1
mesh: blendermesh
material: _default
shader: _default
triangles: 1166
t: 64 92 65
t: 92 66 65
t: 92 0 66
```
 
 ## 3. Python script for blender, to export to custom model format.

Since we had our own model format, we needed a way to bring in models. Using Blender for this was a good choice. It supports almost any model format. The only thing we need is an exporter from Blender. 

Blender is also an excellent tool for just fixing stuff related to models. Triangulation, scaling, rotation.

Blender is scripted using Python. The Python script can be executed inside blenders text-editor.
```python=
import bpy
active_object = bpy.context.active_object
```
By reading the data stored in the `bpy.context` we can pull out almost anything. A comprehensive documentation of the Blender Python API can be found here https://docs.blender.org/api/2.79/ - 30.04.2018

Blender also has a python console for easy testing

![Imgur](https://i.imgur.com/8AUWTTo.png)
 
 ## 4. Transorms, complete with position, rotation, scale, velocity,angular velocity, and children.
 Entities in the framework have a tag(string) that is unique, with a corresponing id(int) that is equally unique. The reason is that int are much faster to address by during runtime. However strings are much more readable.<br>
The entity uses 6 glm vector3s to keep track of its position, rotation, scale, velocity and angular velocity. These vectors are in model space, if the entiry is root in the world, that vector is in the world's model space, more commonly refered to as world space. Hovever if it is child of another entity, the position is relative to it's parent's. <br>
Each entity also has a list of entity ids that describe which other entities in the world that are children of the entity. When the entity updates, the model matrix is passed to each of the children.

 ## 5. Shader programs combined in single files. Parsed during runtime.
 Our shader program parser reads entire shaderprograms written as a single file with the directives `#shader vertex | geometry | fragment` marking the start of different shaders. 
 This allows users to more clearly see the flow of input/output between shaders.
 ## 6. Additional syntax for shaders
 As mentioned above, we've added custom directives for the parser to handle. in addition to the `#shader` directive, we've added the `#prop` directive.
The `#prop` directive is used for setting program specific draw properties.
The draw properties we allow to change through this directive are:
 - Culling
 `#prop Cull {{on | off} | {back | front | both}}`
 - Depth Test
 `#prop ZTest {on | off}`
 - Blend Function
  `#prop Blend {{on | off} | <A> <B>}`
  where A and B may be one of the following (see [OpenGL blending](https://www.khronos.org/opengl/wiki/Blending) for further explanation):
```
    OneMinusConstantAlpha, OneMinusConstantColor, OneMinusDstAlpha, OneMinusSrcColor, 
    OneMinusDstColor, OneMinusSrcAlpha, SrcAlphaSaturate, ConstantColor, ConstantAlpha
    Zero, One, SrcColor, DstColor, SrcAlpha, DstAlpha
```
for readability the parser allows the following negative values:
`off | false | 0 | disabled`
and positive values:
`on | true | 1 | enabled` (or for that matter anything else that is not considered negative)

if these directives are not used, the program will default to the following:
```
Cullface: Back
BlendFunc: SrcAlpha, OneMinusDstAlpha
DepthTest: On
```
 These directives allow shader programs to take control of the way they are rendered by the renderer by swapping the state of said properties on just before anything is drawn (i.e. on binding), and disabling them as soon as the drawcall is done.
 ## 7. Material files coupled with shaders that are attached to models. Transformable enity.
 yaml
 
 ## 8. Point lights. Transformable entity.
 ## 9. Directional light. Transformable entity(only rotation and angular velocity).
 ## 10. Cameras using perspective projection 
 There can be several cameras in the scene. You can switch between them using TAB. There are also two camera modes, FREELOOK and ORBITAL, which you switch between using SPACE. 
 
```cpp
 enum CameraMode
{
    FREELOOK,
    ORBITAL
};
```
  
The freelook mode is like the noclip mode first person shooter often have when spectating a game. You can look around with the mouse, and move in whatever direction you're pointing(using WASD-QE).  
  
Orbital mode is similar to freelook. However, when moving the mouse, the camera will orbit around origin of the camera's parent. If the camera is root that's 0,0,0 world space.  
Implementing a third camera mode would be pretty painless as the structure of the camera is set up to support several. It would be as simpe as adding an enum entry, and two switch cases, all in the same class.

 ## 11. Loading of scene files that defines the layout of cameras, models, directional light, and pointlights.
The scene file consists of cameras, entities(models, and empty nodes), point lights, directional light, and child-parent relations.  
  
The file starts with a camera count, then the camera entities.  
Each camera entity has it's tag, position, rotation, velocity and angular velocity. Followed by the camera mode, nearclip and lastly farclip.  
  
Then entity count. An entity has it's unique entity tag, a model tag that is unique per model(the model tag can be NULL/null/none/_ to signify that is should not be drawn. This is useful for grouping entities under a "core"/"container" entity), position, roation, scale, velocity and it's angular velocity.  
  
Then point light count, then the point light entites.  
Each point light has an entity tag, it's position, velocity, it's vec3 of light intensities which determine it's color, lastly it's falloff which is a vec3, with the components a, b and c. The falloff is calculated as described here:  
https://www.desmos.com/calculator/vgg77cmctb  

Then a bool, hasSun, stating rather there is a directional light in the scene.  
The directional light has an entity tag, rotation, angular velocity, and intensity.  
  
The last section of the scene loading is relations.  
It starts with a count stating how many realtions exist. Followed by all the leaf nodes(the entities that don't have children). The reason for this is that they should be defined before they are mentioned as someone else's children.

```yaml
relations: 3
    grandChild: 0
    child: 1
        grandChild
    parent: 1
        child
```
 
 ## 12. Robust feedback
 Somewhat good error feedback - 
 - **For shader errors**: we're using program introspection to get linking status which in turn allows us to direct users to potential issues in their shader code. We print information about which shader is unable to attatch/link (vertex/fragment/geometry), and the line in the shaderprogram file in which the error occurs. Additionally an error-shader is used, clearly indicating to the user that something went wrong.
 - **For model errors**: if you're trying to use a non-existing model in a scene, the application will instead plug in an error-model where the model should've appeared.
 - **For material errors**: if you're trying to use a non-existing material on a model, the application will instead use an error-material. This material applies _crazy_ values to uniforms to try to make the shader program result as _wrong_ as possible, indicating that something is wrong.

All of the above cases will be logged to the console, allowing the user to inspect exactly what went wrong when loading.

 ## 13. Child-parent relationship between entities where children inherit their parent's transormation. 

OPS already described in 11. scene loading.
```yaml
relations: 24

    sun: 0

    mercury: 0
    mercuryCore: 1
        mercury

    venusCam: 0
    venus: 1
        venusCam
    venusCore: 1
        venus

    earthCam: 0
    moonMoonCam: 0
    moon2: 0
    moonCore2: 1
        moon2
    moon: 2
        moonCore2
        moonMoonCam
    moonCore: 1
        moon
        
# Many more relations....
```

# Future work / Discussion 

## 1. Split up  \<model>.yml files into \<mesh>.yml and \<prefab>.yml files


### How do we do it now?

Currently in our model-files we are specifying which shader and material are to be used in the rendering of our models.
This has created a strong coupling of a mesh to the shader and material. Stronger than we would like.

I will mention two big problems with this:

1. It is not convenient to go inside the model file, in between all the vertices and triangle to edit the materials and shaders attached to that model.
2. For every new model with the same vertices, but with different materials, we have to duplicate all the vertices and indicies. This does not make sense for a big model. Loading times will get out of hand quickly.


*model/human.yml*
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

*scene/demo.yml*
```yml
entities: 1

    entity: player1
        model: human
        position: 10 0 0
        rotation: 0 0 -90
        # ...and so on
```

### What could be a better approach?

An intermediate format representing a `prefab` would have been nice.

A prefab would then stich everything together. An entity would be an instance of a prefab.

*mesh/right-arm.yml*
```yml
vertices: 1560
    v:  1.138500  0.598000 -0.100000    0.707 -0.707  0.000    0.000  0.000   255 255 255 255
    v:  1.138500  0.598000  0.100000    0.707 -0.707  0.000    0.000  0.000   255 255 255 255
    v:  1.138500  0.384000 -0.100000    0.707  0.707  0.000    0.000  0.000   255 255 255 255
    v:  1.138500  0.384000  0.100000    0.707  0.707  0.000    0.000  0.000   255 255 255 255
#.... 1000's more vertices

triangles: 1166
    t: 64 92 65
    t: 92 66 65
    t: 92 0 66
    t: 92 1 0
    t: 92 2 1
    t: 92 3 2
    t: 92 4 3
    t: 92 5 4
    #.... 1000's more triangles
```

*prefab/human.yml*
```yml
meshes: 4

    mesh: right-arm
    shader: basic-color
    material: red

    mesh: left-arm
    shader: basic-color
    material: blue

    mesh: head
    shader: mat-texture
    material: _default

    mesh: foot
    shader: mat-texture
    material: brick
```

*scene/demo.yml*
```yml
entities: 1

    entity: player1
        prefab: human
        position: 10 0 0
        rotation: 0 0 -90
        # ...and so on
```

## 2. Watch files and reload on modify

Currently systems in our 3D engine reloads ALL files every time the users asks for an update. If a shader is edited, the user click `KEY_1` and all shaders are reloaded and compiled.

After using our system, we see that it would be better to just update the single file a users saves changes to. This should also just happen automatically. I file watcher would signal the engine of which file it should reload/recompile.

An attempt was made to actually implement this in our project but we did not succeed finding a good solution within fair time.

A few solutions was explored 


### std::filessystem  (c++17 library)

This is a really good candidate for solving this issue. The problem is that very few compilers have support for this library as of april 2018. It would be awesome to just include a standard library.

*std::filesystem* - http://en.cppreference.com/w/cpp/filesystem - 30.04.2018


### boost::filesystem  from the C++ boost library

The boost::filesystem is the inspiration for std::filesystem, and is currently in a much more stable state. The BIG drawback is that it require installation of the boost library, which is not a trivial thing to have as a dependency. The library in itself is huge, and is not easy pick specific modules. The size of the library is around 2.3 GB on windows. I could not succeed installing the library on windows.

*boost* - https://www.boost.org/doc/libs/1_67_0/libs/filesystem/doc/index.htm - 30.04.2018

### Using stat(2) unix - get file status

Unix stat is in theory supported on all 3 major platforms(linux, windows and mac). I could not get it to read modified status during run-time. I could read it every time the program booted up, but I then had to restart the program to get updated information.

*stat* - http://man7.org/linux/man-pages/man2/stat.2.html - 30.04.2018
*_stat* - https://msdn.microsoft.com/en-us/library/14h5k7ff.aspx - 30.04.2018


### QFileSystemWatcher

As with boost::library this is probably a good stable solution. Nonetheless we chose to not explorer this for the same reasons we did not go any further with boost::filesystem.

We wanted to keep our project as simple as possible to maintain, and keep the project size as small and clean as possible. QT is a big library, and does not fit our ethos.

*QFilsystemWatcher* - http://doc.qt.io/qt-5/qfilesystemwatcher.html - 30.04.2018

### Using platform-specific Filewatcher API's

I did not explore this, and I am regretting it. Filesystems differs widely from platform to platform. This might be the reason why it is so hard to find a standardized way of reading file status during run-time on C++.

If I just allowed myself to be platform specific I might have made everything a lot easier for myself solving this specific issue.

*WIN32 FindFirstChangeNotification* - https://msdn.microsoft.com/en-us/library/aa364417%28VS.85%29.aspx 


*macOs  FSEvents* - https://developer.apple.com/library/content/documentation/Darwin/Conceptual/FSEvents_ProgGuide/UsingtheFSEventsFramework/UsingtheFSEventsFramework.html - 30.04.2018


*Linux INotify* - http://man7.org/linux/man-pages/man7/inotify.7.html - 30.04.2018

*SO Different platform-specific API's* - https://stackoverflow.com/a/931165/9636402 - 30.04.2018


### Remote process call to Python through a pipe

This is what we actually ended up pursuing. We got file discovery working, but we did not get file watching running smoothly. If we have had more time there was a clear path to getting it working this way.

Pythons libraries are by nature cross-platform as long as you can run the python run-time and install libraries with `pip`.

Here is a github repo which was used to explore this direction - https://github.com/Arxcis/filewatcher/blob/master/watcher.py.

The python code was trivial. The most difficult was the communication across the pipe. Some sort of real-time communications protocoll has to be implemented across the pipe.

Also there is problems with blocking system calls to read and write to the pipe. This could have been solved with multi-threading or using timeouts and heartbeat signals, which both are non-trivial things to implemented.

In the end we just settled with file-discovery on boot up for now.


*popen* - http://man7.org/linux/man-pages/man3/popen.3.html - 30.04.2018
*python watchdog* - https://pythonhosted.org/watchdog/


## 3. Improving performance for the model looader

Loading models was the obvious performance limitation of our system. Loading a Bunny with 70k vertices and 144k indicies took 4-5 seconds.

This is documented in this issue on the project repo - http://prod3.imt.hig.no/overkill-studios/imt2531-assignment2/issues/41 - 30.04.2018

It was actually a lot worse in the beginning, but after replacing `std::stringstream` with c-style `scanf` improved performance a lot.

We could have gone a lot further though. After running performance analysis in Visual Studio we discovered that most of the time was not spent in the actual file loading, but parsing. Of the time in the parser a lot was wasted away in allocating temporary strings.

How do we fix this?

Introducing C++17 string_view  - http://en.cppreference.com/w/cpp/string/basic_string_view - 30.04.2018

std::string_view does not hold any stringdata. Just pointers which is allocated on the stack. std::string on the other hand allocates by default no the heap. I would expect 2x-3x performance improvent of the loading, just by replacing all strings with strig_views in the parsing.

Allocating excessive amounts of temp strings is a known issue in the C++ community. Here is a hacker news discussion of how Google Chrome spends a lot of time allocating strings https://news.ycombinator.com/item?id=8704318 - 30.04.2018



## 4. Static Vertex VS Dynamic Vertex format


Our vertex format was very static. We only had a single way of representing a vertex

```
x y z  nx ny nz  u v  r g b a
```
We did this to make the task less complicated. After working with this, it is not entirely clear how we would make this more dynamic in an elegant way.

Limitations:
* Only normal, lacking binormal and tangent.
* We support only 1 set of UV coordinates. Unity supports many sets of UV coordinates.
* We only support drawing triangles. Blender has triangles, quads and more.

More research has to be done on this for us to get a good grasp of how to handle the "Vertex-format situation" better. For the time beiing, we have something that works good enough for many 3D modelling tasks.

The strictness of our vertex format made it challanging to write the Blender parser though. Since Blender has many more possible ways of vertices, we have to convert between two datastructures which may not be entirely compatible. 

We experienced many ugly atrifacts when rendering models exported from Blender. Triangles were missing. Stitching between two textures was ugly.



## 5. Packing vertexdata

We wanted to pack down the vertex data to a minimum practical size before sending it to the GPU, as is described on Khronos' ["Vertex Specification Best Practices" wiki page](https://www.khronos.org/opengl/wiki/Vertex_Specification_Best_Practices#Attribute_sizes)

We managed to pack the normals from 3 floats (12 bytes) -> 1 int (4 bytes). The packing happens when parsing the contents of the file. Since everything is bitwise opeartions it happens really fast, and does not affect load times considerably
```cpp
auto Util::packNormal(float x, float y, float z) -> GLint
{
    float magnitude = sqrt(x * x + y * y + z * z);
    x /= magnitude;
    y /= magnitude;
    z /= magnitude;
    const auto DISCARDMASK = 1023;
    const auto MAX = 511;        //01 1111 1111
    GLint r = ((GLint(z * MAX) & DISCARDMASK) << 20) | ((GLint(y * MAX) & DISCARDMASK) << 10) | (GLint(x * MAX) & DISCARDMASK);
    return r;
}
```

With uv coordinates we are just compressing the dhe dhe dhe dhe dhe dhe dhe dhe dhe dhe dhe dhe dhe dhe dhe dhe dhe dhe dhe dhe dhe dhe dhe dhe d To fit a float inside short in we just multiply with 65k.
```cpp
    float u, v;

    vert.u = 65535U * u;
    vert.v = 65535U * v;
```

We are also reading colors into 4 bytes, 1 for each color + alpha. This is dont by scanf directly by using the `%c`. 
```cpp
auto changecount = sscanf(valueString.data(), "%f  %f  %f    %f  %f  %f    %f  %f   %c %c %c %c", 
             &vert.x, &vert.y, &vert.z, &nx, &ny, &nz, &u, &v, &vert.r, &vert.g, &vert.b, &vert.a);

```

Here is our complete vertex struct for reference

```cpp
struct Vertex
{
    GLfloat  x,y,z;     // 12 bytes
    GLint    n;         // 4 bytes     
    GLushort u,v;       // 4 bytes 
    GLubyte  r,g,b,a;   // 4 byte  
                        // total 24 bytes
};
```

### Byte allignment issues

The reason why our struct is 24 bytes is not by chance. For the VertexArrayObject to operate correctly, it needs data which is byte alligned by the nearest 4 byte. That is at least what we experienced. When we tried to pack uv coordinates from 4 to 2 bytes, we had to have extra padding to make sure our struct still was 24 bytes. 

```cpp
struct Vertex
{
    GLfloat  x,y,z;    // 12 bytes
    GLint      n;      // 4 bytes     
    GLushort    uv;    // 2 byte <-- packing uvs yay
    GLubyte  r,g,b,a;  // 4 byte  
    GLushort padding;  // 2 byte <-- wasting space aww
                       // total still 24 bytes
};
```

Because of this it did not make sense to pack uv coordinates.


## 6. Animations - still a largely unexplored territory.

The only animations we support now is movement and rotation. We can find this in our scene format

```yml
entity: Suzanne
  model: Suzanne
  position: 4 10 1
  rotation: 45 45 45
  scale: 10 10 10
  velocity: 0 0 0 
  angleVelocity: 1 3.4 1.67
```

We do not know how actual animation data looks like. How it would fit into our system. Would we have to expand our  vertex file format ?
What is even animations as data?
Where does it come from?