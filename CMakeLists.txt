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

#DEFINITIONS AND FLAGS
include_directories(${PM_Root})
link_directories(${CMAKE_CURRENT_BINARY_DIR})

IF(MSVC)
	#SET(DEFS "_UNICODE;UNICODE;_MT;_USRDLL")
	#SET(DEFS_DEBUG "_DEBUG")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Oi /fp:fast /EHsc")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Ox /Ob2 /Ot /GL")
	#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Od /MDd")
	#set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /DEBUG")
	IF(CMAKE_CL_64)
		set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /bigobj")
	ENDIF()
ELSE()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
ENDIF()

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
add_executable(pm_test ${PM_TEST_Src} ${PM_Hdr})
set_target_properties(pm_test PROPERTIES COMPILE_DEFINITIONS "${DEFS}" COMPILE_DEFINITIONS_DEBUG "${DEFS_DEBUG}")

#INSTALL
INSTALL(FILES ${PM_Hdr} DESTINATION include/PM)