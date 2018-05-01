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
project("Untitled Application" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)

# Compiler-specific options
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
	add_compile_options(-pthread)
	link_libraries(-pthread)
endif()

# Locate dependencies
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLUT 2.8.1 REQUIRED) # freeglut 2.8.0 had a problem with reported
								  # mouse coordinates and window size.
include_directories(SYSTEM ${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIRS} ${GLUT_INCLUDE_DIR})

# Binaries
add_executable(untitled-application glut-application.cpp)
target_link_libraries(untitled-application ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} ${GLUT_glut_LIBRARY})

# Run target
add_custom_target(run COMMAND untitled-application USES_TERMINAL)
