# IMT2531 - Exam

![Gif of sunrise](https://i.imgur.com/0SkJSw2.gif)

# Tasks

https://drive.google.com/file/d/13sfWUgYOSYyhzyoRiC0l1G2VHZR38nuW/view

## Task 1 Advanced Features:

- [x] Combining daylight cycle and season, using a ramp texture where sampling happens on seasonTime and dayTime.
- [x] dynamic intensities of sunlight (sunrise/sunset) based on real life kelvin temperature values of the sun, applied by sampling a ramp texture on the day/night-time.
- [x] water shader with effects based on sunlight (due to the blend function and color of terrain underneath the water, this almost looks like some sort of water scattering)
- [x] shader to dynamically draw the sun on in the skybox. works out of the box with any skybox texture. everything is done on the shader
- [x] decreased color values in steep areas of the map, further simulating effects of gravity and erotion in the map. (works somewhat ok, the magic values needed has not quite yet been found. Turns out, this was also commented out in a commit due to the desaturated color values it caused - FIX: use a smoothstep or other stepping/curve mechanism to evaluate steepness erosion effect) 


## Task 2 Advanced Features:
- [x] Internals view/cockpit view (although, due to conversion isses between obj and the internal format, none of the textures are applied correctly within the cockpit, so the view is fairly useless - For some reason this works better on Linux, no idea why...)
- [x] Simulated roll-rotation when rotating arount the yaw axis. 

# Install Instructions

## All systems

Although The CMake-list should allow to build on Windows, MacOS and Linux, it should be noted that building has only been tested on Windows, using Visual Studio.

## Windows
1. Adding the CMake-module for Visual Studio:
    - Install Visual Studio 2017 15.4 or newer (the CMake module is only available for these versions).
    - From Visual Studio: go to `Tools->Get Tools And Features...`
    - Select the box named `Desktop development with C++`
    - Enable the checkbox named `Visual C++ Tools for CMake` in the righthand column
    - Press `Modify` to apply the module
 2. Install [python 3](https://www.python.org/downloads/)(or newer)
    - Make sure to specify the installpath (or path variable, likely being `python`) in the config
 3. Clone [the repository](http://prod3.imt.hig.no/overkill-studios/imt2531-assignment2)
     - from gitbash: `> cd "./yourdesiredworkdir/"`
     - from gitbash: `> git clone https://github.com/Stektpotet/imt2531-exam.git`
 4. In Visual Studio: go to `File->Open->CMake...`, and select the CMakeLists.txt file located in `./yourdesiredworkdir/imt2531-exam/`


## MacOS

```shell
brew install cmake

git clone https://github.com/Stektpotet/imt2531-exam.git
cd ./imt2531-exam

cp docs/config.example.yml config.yml # Edit this to fit your system
mkdir build/
cd build/
cmake ..
make
```

## Linux
Since we are using c++17 features, both a newer compiler, and an update of libraries is necessary.
In order to get the newer C++ libs, run this:
```
    # Install updated c++ libraries
    sudo add-apt-repository ppa:jonathonf/gcc-7.1
    sudo apt-get update
    sudo apt-get install gcc-7 g++-7
    
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

    git clone https://github.com/Stektpotet/imt2531-exam.git
    
    cd imt2531-exam
    mkdir build
    cd build

    cmake .. -DCMAKE_CXX_COMPILER=clang++-5.0
    make
    
    cp ../docs/config.example.yml config.yml    # Copy default config to build.

    ./cube                         # Load default scene.
    ./cube assets/scenes/demo1.yml # Load specific scene.
```



## Key bindings

| General | |
|---------|-|
| ESC | Quit App |
| Right Click | Toggle capture cursor (i.e. toggle mouse look-around. Allows for using mouse on UI) |
| Left mouse drag | Drag UI sliders |
| Left mouse click | UI interaction |

| Time & Season | |
|---------------|-|
| 1 | Jump to Winter |
| 2 | Jump to Spring |
| 3 | Jump to Summer |
| 4 | Jump to Autumn |
| 5 | Toggle Season Cycle |
| 6 | Jump to Morning |
| 7 | Jump to Noon |
| 8 | Jump to Afternoon |
| 9 | Jump to Evening |
| 0 | Toggle Day/Night Cycle |

| Camera | | |
|--------|-|-|
| I | Forward ||
| K | Backward ||
| L | Right ||
| J | Left ||
| Y | Up ||
| H | Down ||
| Space | Toggle Camera Mode [`FREELOOK / RELATIVE FREELOOK` &#124; `ORBITAL` &#124; `LOCKED`] | (Specular lights not working correctly in orbital)
| Tab | Cycle Cameras in scene | In `demo1` there are two cameras: the glider camera and the freelook camera |
| Mouse Δ | Look around ||

| Glider | | |
|--------|-|-|
| W | Pitch down ||
| S | Pitch up ||
| A | Yaw left | "Animates" roll. (This is buggy and hacky as it's based on velocity) |
| D | Yaw right | "Animates" roll. (This is buggy and hacky as it's based on velocity) |
| F | "Jump" through predefined locations | in `demo1` there's a predefined location under the map, this can easily be changed in the scene file |
| R | "Jump" back to last location ||
| , | Accelerate | Comma |
| . | Decelerate | Period |


| Visualizations | | |
|----------------|-|-|
| O | Switch Heightmap Visualization [`REGULAR` &#124; `CONTOUR` &#124; `MIXED`] | Hotswaps material to swap ramp texture |

| MISC - Reloading asset files | | |
|------------------------------|-|-|
| F1 | Reload Shaders ||
| F2 | Reload Materials ||
| F3 | Reload Models ||
| F4 | Reload Scene + Config file | swap scene by changing config (`demo1` is the only one for the exam)|

