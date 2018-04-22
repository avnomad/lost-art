# Copyright (C) 2014, 2018 Vaptistis Anogeianakis <nomad@cornercase.gr>
#
# This file is part of LostArt.
#
# LostArt is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# LostArt is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with LostArt.  If not, see <http://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.5 FATAL_ERROR)
project(LostArt LANGUAGES CXX)

enable_testing()

set(CMAKE_CXX_STANDARD 14)
add_compile_options(-Wno-logical-op-parentheses)

# Locate dependencies
find_package(Boost REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLUT 2.8.1 REQUIRED) # freeglut 2.8.0 had a problem with reported
								  # mouse coordinates and window size.
add_definitions(${EIGEN3_DEFINITIONS})

include_directories(SYSTEM ${Boost_INCLUDE_DIRS} ${EIGEN3_INCLUDE_DIRS}
	${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIRS} ${GLUT_INCLUDE_DIR})
include_directories("source")

# Tests
aux_source_directory("tests" TESTS) # CMake won't automatically detect new files!
add_custom_target(unit-tests COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure COMMENT "Running Unit Tests...")

foreach(test_file ${TESTS})
	get_filename_component(test_name ${test_file} NAME_WE)
	add_executable(${test_name} EXCLUDE_FROM_ALL ${test_file})
	add_test(NAME ${test_name} COMMAND ${test_name})

	set_target_properties(${test_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY tests)
	add_dependencies(unit-tests ${test_name})
endforeach(test_file)

# Binaries
aux_source_directory("source" SOURCES) # CMake won't automatically detect new files!
add_executable(lost-art ${SOURCES})

set_target_properties(lost-art PROPERTIES RUNTIME_OUTPUT_DIRECTORY binaries)
target_compile_options(lost-art PRIVATE -pthread)
target_link_libraries(lost-art -pthread ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} ${GLUT_glut_LIBRARY})

# Resources
configure_file(templates/CMakeLists.txt binaries/CMakeLists.txt COPYONLY) # the fist file needs to be named...
configure_file(templates/glut-application.cpp binaries/ COPYONLY) # ... for the directory to be created.
configure_file(source/geometry.hpp binaries/ COPYONLY)
