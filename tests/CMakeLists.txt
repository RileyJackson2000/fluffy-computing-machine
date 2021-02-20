include(GoogleTest)

file(GLOB_RECURSE sources ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_executable(tests tests.cpp ${sources} ${headers})

target_include_directories(tests PUBLIC ../include ../extern/googletest/googletest/include)

target_link_libraries(tests PUBLIC gtest gtest_main PRIVATE physics render model utils math
  glfw libglew_static glm PNGwriter)

gtest_discover_tests(tests)
