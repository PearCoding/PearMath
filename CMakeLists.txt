#
# Copyright (c) 2014, Ömercan Yazici <pearcoding AT gmail.com>
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without modification,
# are permitted provided that the following conditions are met:
#
#    1. Redistributions of source code must retain the above copyright notice,
#       this list of conditions and the following disclaimer.
#
#    2. Redistributions in binary form must reproduce the above copyright
#       notice, this list of conditions and the following disclaimer in the
#       documentation and/or other materials provided with the distribution.
#
#    3. Neither the name of the copyright owner may be used
#       to endorse or promote products derived from this software without
#       specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER BE LIABLE FOR
# ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
# ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE
#

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(PearMath CXX)

#SETS
SET(VENDOR "PearMath project 2014")
SET(VERSION_MAJOR 1)
SET(VERSION_MINOR 0)
SET(VERSION_HEX 0x0100)
SET(PM_Version ${VERSION_MAJOR}.${VERSION_MINOR} PARENT_SCOPE)
SET(PM_TARGET "PearMath")

SET(PM_Root ${CMAKE_CURRENT_SOURCE_DIR}/src)

SET(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})
SET(CMAKE_DEBUG_POSTFIX  "_d")

#OPTIONS
option(PM_USE_SIMD "Enable SIMD support." ON)
option(PM_FORCE_SSE3 "Force the use of SSE3 when detection fails." OFF)
option(PM_FORCE_SSSE3 "Force the use of SSSE3 when detection fails." OFF)
option(PM_FORCE_SSE4 "Force the use of SSE4 when detection fails." OFF)

#CHECKS
MESSAGE(STATUS "Building PearMath ${PM_Version}")
MESSAGE(STATUS "Root: ${PM_Root}")

IF(MINGW)
	MESSAGE(WARNING "Please don't use MinGW with PearMath, use the MSVC version instead")
ENDIF()

IF(MSYS)
	MESSAGE(WARNING "Please don't use MSYS with PearMath, use the MSVC version instead")
ENDIF()

IF(CYGWIN)
	MESSAGE(WARNING "Please don't use CYGWIN with PearMath, use the MSVC version instead")
ENDIF()

#PACKAGES
option(PM_ONLY_LIBRARY "Compile only library" OFF)

#DEFINITIONS AND FLAGS
include_directories(${PM_Root})
link_directories(${CMAKE_CURRENT_BINARY_DIR})

SET(DEFS "${DEFS};")
SET(DEFS_DEBUG "${DEFS_DEBUG};PM_DEBUG")

IF(PM_USE_SIMD)
	IF(PM_FORCE_SSE3)
		SET(DEFS "${DEFS};PM_FORCE_SSE3")
	ENDIF()
	
	IF(PM_FORCE_SSSE3)
		SET(DEFS "${DEFS};PM_FORCE_SSSE3")
	ENDIF()
	
	IF(PM_FORCE_SSE4)
		SET(DEFS "${DEFS};PM_FORCE_SSE4")
	ENDIF()
ELSE()
	SET(DEFS "${DEFS};PM_NO_SIMD")
ENDIF()

IF(NOT MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse2" PARENT_SCOPE)

	IF(PM_FORCE_SSE3)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse3" PARENT_SCOPE)
	ENDIF()

	IF(PM_FORCE_SSSE3)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mssse3" PARENT_SCOPE)
	ENDIF()

	IF(PM_FORCE_SSE4)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse4.1" PARENT_SCOPE)
	ENDIF()
ENDIF()

SET(PM_DEFS "${DEFS}" PARENT_SCOPE)
SET(PM_DEFS_DEBUG "${DEFS_DEBUG}" PARENT_SCOPE)
#CONFIGURE
#include_directories(${CMAKE_CURRENT_BINARY_DIR})

SET(PM_Hdr
  ${PM_Root}/PearMath.h
  ${PM_Root}/MathColor.inl
  ${PM_Root}/MathFrame.inl
  ${PM_Root}/MathMatrix.inl
  ${PM_Root}/MathQuaternion.inl
  ${PM_Root}/MathVector.inl
  ${PM_Root}/Rect.h
  ${PM_Root}/Region.h)

SET(PM_TEST_Src
  ${PM_Root}/tests/main.cpp)

#DEPENDIES, LIBARIES AND EXECUTABLES
IF(NOT PM_ONLY_LIBRARY)
	add_executable(pm_test ${PM_TEST_Src} ${PM_Hdr})
	set_target_properties(pm_test PROPERTIES COMPILE_DEFINITIONS "${DEFS}" COMPILE_DEFINITIONS_DEBUG "${DEFS_DEBUG}")
ENDIF(NOT PM_ONLY_LIBRARY)

#INSTALL
INSTALL(FILES ${PM_Hdr} DESTINATION include/PM)
