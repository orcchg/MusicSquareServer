#   HTTP Server
#
#   Copyright (C) 2016  Maxim Alov
#
#   This program is free software; you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation; either version 3 of the License, or
#   (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program; if not, write to the Free Software Foundation,
#   Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
#
#   This program and text files composing it, and/or compiled binary files
#   (object files, shared objects, binary executables) obtained from text
#   files of this program using compiler, as well as other files (text, images, etc.)
#   composing this program as a software project, or any part of it,
#   cannot be used by 3rd-parties in any commercial way (selling for money or for free,
#   advertising, commercial distribution, promotion, marketing, publishing in media, etc.).
#   Only the original author - Maxim Alov - has right to do any of the above actions.

cmake_minimum_required( VERSION 2.8 )
PROJECT( MusicSquareServer )
SET( D_VERSION "1.2.1" )
MESSAGE( STATUS "Version ${D_VERSION}" )

# -----------------------------------------------------------------------------
# --------------------------- Common header -----------------------------------
# -----------------------------------------------------------------------------
INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR} )  # for config.h
INCLUDE_DIRECTORIES( ${PROJECT_SOURCE_DIR} include )

if( CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX )
    SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -std=c++11 -O3 -pthread" )
endif()

if( DEBUG )
    MESSAGE( STATUS "Build configuration: DEBUG" )
    ADD_DEFINITIONS( -DENABLED_LOGGING=1 )
    ADD_DEFINITIONS( -DD_VERSION="${D_VERSION}-DEBUG" )
else()
    MESSAGE( STATUS "Build configuration: RELEASE" )
    ADD_DEFINITIONS( -DENABLED_LOGGING=0 )
    ADD_DEFINITIONS( -DD_VERSION="${D_VERSION}-RELEASE" )
endif()

if( WITH_TEST )
    MESSAGE( STATUS "Build with tests" )
    ADD_SUBDIRECTORY( googletest )
    ADD_SUBDIRECTORY( test )
endif()

# -----------------------------------------------------------------------------
ADD_SUBDIRECTORY( common )
ADD_SUBDIRECTORY( database )
ADD_SUBDIRECTORY( parser )
ADD_SUBDIRECTORY( server )
ADD_SUBDIRECTORY( sqlite )

