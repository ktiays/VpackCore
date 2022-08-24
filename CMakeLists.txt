cmake_minimum_required(VERSION 3.23)
project(VpackCore)

set(CMAKE_CXX_STANDARD 20)

add_library(VpackCore
        src/main.cpp
        src/layoutables/layoutable.hpp
        src/layout_result.hpp src/types.hpp
        src/optional.hpp
        src/layoutables/containers/container.hpp
        src/layoutables/item.hpp
        src/layoutables/measurable.hpp
        src/layoutables/utils/size_extractor.hpp
        src/utils/sequence.hpp
        src/layoutables/containers/horizontal_container.hpp
        src/types.cpp
        src/utils/indexed.hpp
        src/computer.hpp
        src/layoutables/containers/vertical_container.hpp
        src/layoutables/containers/stack_container.hpp
        VpackCore.hpp
        tests/src/view.hpp
        tests/src/container.hpp
        tests/src/some_view.hpp src/layoutables/utils/math.hpp src/layoutables/containers/hv_container.hpp src/layoutables/containers/decorated_container.hpp)

add_subdirectory(tests)