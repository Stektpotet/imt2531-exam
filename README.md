# IMT2531 - Exam


# Tasks

https://drive.google.com/file/d/13sfWUgYOSYyhzyoRiC0l1G2VHZR38nuW/view

## Task 1 Advanced Features:

- [x] Combining daylight cycle and season, using a ramp texture where sampling happens on seasonTime and dayTime.
- [x] dynamic intensities of sunlight (sunrise/sunset) based on real life kelvin temperature values of the sun, applied by sampling a ramp texture on the day/night-time.
- [x] water shader with effects based on sunlight
- [x] shader to dynamically draw the sun on in the skybox. works out of the box with any skybox texture. everything is done on the shader
- [x] decreased color values in steep areas of the map, further simulating effects of gravity and erotion in the map. (works somewhat ok, the magic values needed has not quite yet been found)


## Task 2 Advanced Features:
- [x] Internals view/cockpit view (although, due to conversion isses between obj and the internal format, none of the textures are applied correctly within the cockpit, so the view is fairly useless)
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

    git clone https://github.com/Stektpotet/imt2531-exam.git
    
    cd imt2531-assignment2
    mkdir build
    cd build

    cmake .. -DCMAKE_CXX_COMPILER=clang++-5.0
    make
    
    cp ../docs/config.example.yml config.yml    # Copy default config to build.

    ./cube                         # Load default scene.
    ./cube assets/scenes/demo1.yml # Load specific scene.
```



## Key bindings


```
// ---------------------------------------------------------------------
// ---------------------------------------------------------------------
// --------------------
//    General:
// --------------------
//  ESC: quit app
//
//   right click: 
//   toggle capture mouse (i.e. toggle mouse lookaround. Allows for using mouse on UI)
// -------------------
//   Time and weather:
//  -------------------
//   1: Jump to Winter
//   2: Jump to Spring
//   3: Jump to Summer
//   4: Jump to Autumn
//   5: Pause Season Cycle
//
//   6: Jump to Morning
//   7: Jump to Noon
//   8: Jump to Afternoon
//   9: Jump to Evening
//   0: Pause Day/Night Cycle
//
// ------------------------
//   Camera:
// ------------------------
//   i: forward
//   k: backward
//   l: right
//   j: left
//   y: up
//   h: down
//   space: toggl camera mode [FREELOOK | ORBITAL]  (Specular lights not working correctly in orbital)
//   tab: cycle through cameras
//   mouse: look around
// 
// -----------------------
//    Glider:
// -----------------------  
//    W: pitch down
//    S: pitch up
//    A: yaw left 	(animate roll -sort of)
//    D: yaw right   (animate roll -sort of) 
//    F: Switch through different predefined locations
//    R: Reset to the last position
//   
//    comma:  accelerate
//    period: decelerate
//
//
// -------------------------------
//    Visualizations:
// -------------------------------
//   o: switch heightmap visualizations (i.e. contour variations. - regular/mixed/contour)
//
//
// --------------------------------------
//    Reload asset files:
// --------------------------------------
//   F1:      RELOAD Shaders
//   F2:      RELOAD Materials
//   F3:      RELOAD Models
//   F4:      RELOAD Scene + Config file
// ---------------------------------------------------------------------
// ---------------------------------------------------------------------
```