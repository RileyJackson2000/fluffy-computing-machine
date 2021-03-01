# Physics Simulator

https://docs.google.com/document/d/1d8BcXjWF1bPuGfM2PeCxXdvw-QjfWAuVCvRD2_8Fwgk/

# Installation

Install dependencies:

```console
sudo apt-get install libglew-dev libglfw3-dev libglm-dev libx11-dev libxi-dev libgl1-mesa-dev libglu1-mesa-dev libxrandr-dev libxext-dev libxcursor-dev libxinerama-dev libxi-dev
```

You'll also need the following dependencies for the current recording setup
```console
sudo apt-get install libpng-dev libfreetype-dev libfreetype6 libfreetype6-dev
```

In order for the recording script to work, you need
```console
sudo apt install ffmpeg
```

Clone repo:

```console
$ git clone --recursive git@github.com:RileyJackson2000/fluffy-computing-machine.git
$ mkdir build
$ cd build
$ cmake ..
$ make
```

We also need to manually disable the uninstall target in extern/eigen/CMakeLists.txt

# Workflow
Currently master is protected so any changes must be made on local branches, pass all checks, and have atleast one review before being able to be merged.

# Layout
```
+-- app                            // application folder
|   +-- main.cpp
+-- cmake                          // cmake scripts, finding libraries, etc.
+-- extern                         // for submodules
|   +-- googletest
+-- include                        // internal libraries
|   +-- model
|   +-- physics
|   +-- render
|   +-- utils
+-- python                         // python bindings
+-- scripts                        // bash scripts, etc.
+-- src                            // internal library source files
|   +-- model
|   +-- physics
|   +-- render
|   +-- utils
+-- tests                          // unit tests
```
