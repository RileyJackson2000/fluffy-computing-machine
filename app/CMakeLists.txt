add_executable(fluff main.cpp)
target_compile_features(fluff PRIVATE cxx_std_17)
target_link_libraries(fluff PRIVATE model physics render utils
    glfw libglew_static)
