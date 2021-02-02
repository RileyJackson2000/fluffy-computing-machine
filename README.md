# Physics Simulator

https://docs.google.com/document/d/1d8BcXjWF1bPuGfM2PeCxXdvw-QjfWAuVCvRD2_8Fwgk/

# Installation
```console
$ git clone --recursive git@github.com:RileyJackson2000/fluffy-computing-machine.git
$ mkdir build
$ cd build
$ cmake ..
$ make
```

# Workflow
Currently master is protected so any changes must be made on local branches, pass all checks, and have atleast one review before being able to be merged.

# Layout
```
+-- app                            // application folder
|   +-- main.cpp
+-- cmake                          // cmake scripts, finding libraries, etc.
+-- docs                           // documentation folder @TODO (maybe?)
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
