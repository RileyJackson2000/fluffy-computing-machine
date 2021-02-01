include(GoogleTest)

add_executable(tests tests.cpp)

target_include_directories(tests PUBLIC ../src ../extern/googletest/googletest/include)

target_link_libraries(tests PUBLIC gtest gtest_main)

gtest_discover_tests(tests)
