#
# Copyright (c) 2014, OEmercan Yazici <pearcoding AT gmail.com>
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

SET(SIMD_FEATURES SSE SSE2 SSE3 SSSE3 SSE4_1 SSE4_2
	AES AVX AVX2
	AVX512 AVX512DQ AVX512IFMA AVX512PF AVX512ER AVX512CD AVX512BW AVX512VL)

#OPTIONS
option(PM_USE_SIMD "Enable SIMD support." ON)
option(PM_USE_SVML "Enable Intel SVML library support if available." OFF)
option(PM_USE_CPP11 "Enable C++11 support." ON)
FOREACH(feature ${SIMD_FEATURES})
	option(PM_FORCE_${feature} "Force the use of ${feature} even when detection fails." OFF)
ENDFOREACH()
option(PM_ONLY_LIBRARY "Compile only library" OFF)

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

IF(PM_USE_SIMD)
	TRY_RUN(RUN_RESULT_VAR COMPILE_RESULT_VAR
		${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/tools/check_support.cpp 
		RUN_OUTPUT_VARIABLE CHECK_SUPPORT)

	FOREACH(feature ${SIMD_FEATURES})
		IF(PM_FORCE_${feature})
			MESSAGE(STATUS "${feature}\t= [FORCED]")
			SET(PM_WITH_${feature} 1)
			SET(PM_WITH_${feature} 1 PARENT_SCOPE)
			SET(DEFS "${DEFS};PM_WITH_${feature}")
		ELSEIF("${CHECK_SUPPORT}" MATCHES ".*${feature}=YES.*")
			MESSAGE(STATUS "${feature}\t= [SUPPORTED]")
			SET(PM_WITH_${feature} 1)
			SET(PM_WITH_${feature} 1 PARENT_SCOPE)
			SET(DEFS "${DEFS};PM_WITH_${feature}")
		ELSE()
			MESSAGE(STATUS "${feature}\t= [NOT SUPPORTED]")
		ENDIF()
	ENDFOREACH()

	SET(PM_WITH_SIMD 1)
	SET(PM_WITH_SIMD 1 PARENT_SCOPE)
	SET(DEFS "${DEFS};PM_WITH_SIMD")
ENDIF()

#DEFINITIONS AND FLAGS
include_directories(${PM_Root})
link_directories(${CMAKE_CURRENT_BINARY_DIR})

SET(DEFS "${DEFS};")
SET(DEFS_DEBUG "${DEFS_DEBUG};PM_DEBUG")

IF((NOT MSVC) AND PM_WITH_SIMD)
	SET(SSE_FLAG "-msse2")

	IF(PM_WITH_SSE3)
		set(SSE_FLAG "-msse3")
	ENDIF()

	IF(PM_WITH_SSSE3)
		set(SSE_FLAG "-mssse3")
	ENDIF()

	IF(PM_WITH_SSE4_1)
		set(SSE_FLAG "-msse4.1")
	ENDIF()

	IF(PM_WITH_SSE4_2)
		set(SSE_FLAG "-msse4.2")
	ENDIF()

	IF(PM_WITH_AES)
		set(SSE_FLAG "-maes")
	ENDIF()

	IF(PM_WITH_AVX)
		set(SSE_FLAG "-mavx")
	ENDIF()
	
	set(PM_COMPILE_FLAGS "${SSE_FLAG}")
	set(PM_COMPILE_FLAGS "${PM_COMPILE_FLAGS}" PARENT_SCOPE)
ENDIF()

IF(PM_USE_SVML)
	SET(DEFS "${DEFS};PM_USE_SVML")
ENDIF()

IF(PM_USE_CPP11)
	SET(DEFS "${DEFS};PM_WITH_CPP11")
ENDIF()

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${PM_COMPILE_FLAGS}")
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
	enable_testing()
	add_executable(pm_test ${PM_TEST_Src} ${PM_Hdr})
	target_compile_definitions(pm_test${name} PRIVATE "${DEFS}" "$<$<CONFIG:Debug>:${DEFS_DEBUG}>")
	add_test(NAME pearmath COMMAND pm_test)
ENDIF(NOT PM_ONLY_LIBRARY)

#INSTALL
INSTALL(FILES ${PM_Hdr} DESTINATION include/PM)
