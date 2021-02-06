include(GoogleTest)

file(GLOB_RECURSE sources ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_executable(tests tests.cpp ${sources} ${headers})

target_include_directories(tests PUBLIC ../src ../extern/googletest/googletest/include)

target_link_libraries(tests PUBLIC physics render model utils
    glfw libglew_static glm
    gtest gtest_main)

gtest_discover_tests(tests)
