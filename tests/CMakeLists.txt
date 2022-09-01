# 'vpack_core_tests' is the subproject name
project(vpack_core_tests)

# 'lib' is the folder with Google Test sources
add_subdirectory(lib)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(Vpack_Core_Run tests.cpp src/text.hpp src/spacer.hpp src/uuid.h)
target_link_libraries(Vpack_Core_Run gtest gtest_main VpackCore)