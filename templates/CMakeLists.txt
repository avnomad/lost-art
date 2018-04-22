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

# Locate dependencies
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLUT 2.8.1 REQUIRED) # freeglut 2.8.0 had a problem with reported
								  # mouse coordinates and window size.
include_directories(SYSTEM ${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIRS} ${GLUT_INCLUDE_DIR})

# Binaries
add_executable(untitled-application glut-application.cpp)
target_compile_options(untitled-application PRIVATE -pthread)
target_link_libraries(untitled-application -pthread ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} ${GLUT_glut_LIBRARY})

# Run target
if(CMAKE_HOST_WIN32) # Host-specific prefixes and suffixes to run command in the background.
	set(PREFIX start)
else()
	set(SUFFIX &)
endif()

add_custom_target(run COMMAND ${PREFIX} untitled-application ${SUFFIX} USES_TERMINAL)
