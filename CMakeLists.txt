#
# Copyright (c) 2016 Rokas Kupstys
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
cmake_minimum_required(VERSION 2.8.12.1)
project(AtomicIMUIs)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(CMake/Utilities.cmake)
if (MSVC)
    replace_cflag(/MD "" "CMAKE_C_FLAGS;CMAKE_CXX_FLAGS")
    replace_cflag(/MDd "" "CMAKE_C_FLAGS;CMAKE_CXX_FLAGS")
    replace_cflag(/MD /MT "CMAKE_C_FLAGS_RELEASE;CMAKE_C_FLAGS_RELWITHDEBINFO;CMAKE_C_FLAGS_MINSIZEREL;CMAKE_CXX_FLAGS_RELEASE;CMAKE_CXX_FLAGS_RELWITHDEBINFO;CMAKE_CXX_FLAGS_MINSIZEREL")
    replace_cflag(/MDd /MTd "CMAKE_C_FLAGS_DEBUG;CMAKE_CXX_FLAGS_DEBUG")
endif ()

include_directories(dep)
add_subdirectory(dep)
add_subdirectory(ImGuiDemo)
add_subdirectory(NuklearDemo)

symlink(${CMAKE_SOURCE_DIR}/dep/AtomicGameEngine/Resources/CoreData ${CMAKE_BINARY_DIR}/bin/CoreData)
symlink(${CMAKE_SOURCE_DIR}/SampleData ${CMAKE_BINARY_DIR}/bin/SampleData)
