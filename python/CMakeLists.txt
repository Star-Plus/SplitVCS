cmake_minimum_required(VERSION 3.14)
project(pyscm LANGUAGES CXX)

if (POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)
endif()

set(PYBIND11_FINDPYTHON NEW)

include(FetchContent)
FetchContent_Declare(
        pybind11
        URL https://github.com/pybind/pybind11/archive/refs/heads/master.zip
)
FetchContent_MakeAvailable(pybind11)

pybind11_add_module(pyscm bindings.cpp)
target_link_libraries(pyscm PRIVATE scm_lib)
