# Physics Simulator

https://docs.google.com/document/d/1d8BcXjWF1bPuGfM2PeCxXdvw-QjfWAuVCvRD2_8Fwgk/

# Installation
```console
foo@bar:~$ git clone --recursive git@github.com:RileyJackson2000/fluffy-computing-machine.git
```

# Layout
```
+-- app                            // application folder
|   +-- main.cpp
+-- cmake                          // cmake scripts, finding libraries, etc.
+-- docs                           // documentation folder @TODO (maybe?)
+-- extern                         // for submodules
|   +-- googletest
+-- include                        // internal libraries
|   +-- math
|   +-- physics
|   +-- render
|   +-- utils
+-- python                         // python bindings
+-- scripts                        // bash scripts, etc.
+-- src                            // internal library source files
|   +-- math
|   +-- physics
|   +-- render
|   +-- utils
+-- tests                          // unit tests
```
