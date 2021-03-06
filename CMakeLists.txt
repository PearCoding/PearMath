#
# Copyright (c) 2014-2017, OEmercan Yazici <pearcoding AT gmail.com>
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

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(PearMath LANGUAGES C CXX)

get_directory_property(hasParent PARENT_DIRECTORY)

#SETS
SET(VENDOR "PearMath project 2014-2017")
SET(VERSION_MAJOR 1)
SET(VERSION_MINOR 2)
SET(VERSION_HEX 0x0102)
SET(PM_Version ${VERSION_MAJOR}.${VERSION_MINOR})
IF(hasParent)
	SET(PM_Version ${PM_Version} PARENT_SCOPE)
ENDIF()

SET(SIMD_FEATURES SSE SSE2 SSE3 SSSE3 SSE4_1 SSE4_2
	AES AVX AVX2
	AVX512 AVX512DQ AVX512IFMA AVX512PF AVX512ER AVX512CD AVX512BW AVX512VL)

#OPTIONS
option(PM_USE_SIMD "Enable SIMD support." ON)
option(PM_USE_SVML "Enable Intel SVML library support if available." OFF)
FOREACH(feature ${SIMD_FEATURES})
	option(PM_FORCE_${feature} "Force the use of ${feature} even when detection fails." OFF)
ENDFOREACH()
option(PM_ONLY_LIBRARY "Compile only library" OFF)
option(PM_TEST_COMBINATIONS "When building tests, also build available feature combinations." OFF)

#CHECKS
MESSAGE(STATUS "Building PearMath ${PM_Version}")

IF(MINGW)
	MESSAGE(WARNING "Please don't use MinGW with PearMath, use the MSVC version instead")
ENDIF()

IF(MSYS)
	MESSAGE(WARNING "Please don't use MSYS with PearMath, use the MSVC version instead")
ENDIF()

IF(CYGWIN)
	MESSAGE(WARNING "Please don't use CYGWIN with PearMath, use the MSVC version instead")
ENDIF()

IF(MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Oi /fp:fast /EHsc")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Ox /Ob2 /Ot /GL")
	IF(CMAKE_CL_64)
		set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /bigobj")
	ENDIF()
ELSEIF(CMAKE_COMPILER_IS_GNUCXX)
	execute_process(COMMAND ${CMAKE_C_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
	IF (GCC_VERSION VERSION_GREATER 4.7 OR GCC_VERSION VERSION_EQUAL 4.7)
		message(STATUS "C++11 activated.")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
	ELSEIF (GCC_VERSION VERSION_GREATER 4.3 OR GCC_VERSION VERSION_EQUAL 4.3)
		message(WARNING "C++0x activated. If you get any errors update to a compiler which fully supports C++11")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++0x")
	ELSE ()
		message(WARNING "C++11 needed. Build may fail.")   
	ENDIF ()
ELSEIF(CMAKE_CXX_COMPILER_ID MATCHES "Intel")
	IF(WIN32)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Qstd=c++11")
	ELSE()
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
	ENDIF()
ELSEIF()
    message(WARNING "Unknown Compiler. Build may fail.")   
ENDIF()

IF(PM_USE_SIMD)
	TRY_RUN(RUN_RESULT_VAR COMPILE_RESULT_VAR
		${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/tools/check_support.cpp 
		RUN_OUTPUT_VARIABLE CHECK_SUPPORT)

	FOREACH(feature ${SIMD_FEATURES})
		IF(PM_FORCE_${feature})
			MESSAGE(STATUS "${feature}\t= [FORCED]")
			SET(PM_WITH_${feature} 1)
			IF(hasParent)
				SET(PM_WITH_${feature} 1 PARENT_SCOPE)
			ENDIF()
			SET(DEFS "${DEFS};PM_WITH_${feature}")
		ELSEIF("${CHECK_SUPPORT}" MATCHES ".*${feature}=YES.*")
			MESSAGE(STATUS "${feature}\t= [SUPPORTED]")
			SET(PM_WITH_${feature} 1)
			IF(hasParent)
				SET(PM_WITH_${feature} 1 PARENT_SCOPE)
			ENDIF()
			SET(DEFS "${DEFS};PM_WITH_${feature}")
		ELSE()
			MESSAGE(STATUS "${feature}\t= [NOT SUPPORTED]")
		ENDIF()
	ENDFOREACH()

	SET(PM_WITH_SIMD 1)
	IF(hasParent)
		SET(PM_WITH_SIMD 1 PARENT_SCOPE)
	ENDIF()
	SET(DEFS "${DEFS};PM_WITH_SIMD")
ENDIF()

#DEFINITIONS AND FLAGS
include_directories(src)
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

	IF(PM_WITH_AVX)
		set(SSE_FLAG "-mavx")
	ENDIF()
	
	set(PM_COMPILE_FLAGS "${SSE_FLAG}")
	IF(hasParent)
		set(PM_COMPILE_FLAGS "${PM_COMPILE_FLAGS}" PARENT_SCOPE)
	ENDIF()
ENDIF()

IF(PM_USE_SVML)
	SET(DEFS "${DEFS};PM_USE_SVML")
ENDIF()

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${PM_COMPILE_FLAGS}")

IF(hasParent)
	SET(PM_DEFS "${DEFS}" PARENT_SCOPE)
	SET(PM_DEFS_DEBUG "${DEFS_DEBUG}" PARENT_SCOPE)
ENDIF()
#CONFIGURE
#include_directories(${CMAKE_CURRENT_BINARY_DIR})

SET(PM_Hdr
  src/PearMath.h
  src/MathColor.inl
  src/MathFrame.inl
  src/MathMatrix.inl
  src/MathQuaternion.inl
  src/MathVector.inl)

FUNCTION(PUSH_TEST name files local_defs)
	add_executable(pm_test_${name} ${files} src/tests/test.h ${PM_Hdr})
	set_target_properties(pm_test_${name} PROPERTIES VERSION ${PM_Version})
	target_compile_definitions(pm_test_${name} PRIVATE "${local_defs}" "$<$<CONFIG:Debug>:${DEFS_DEBUG}>")
	add_test(NAME ${name} COMMAND pm_test_${name})
ENDFUNCTION(PUSH_TEST)

FUNCTION(TEST_SUITE name files)
	IF(PM_TEST_COMBINATIONS)
		PUSH_TEST(${name}_serial ${files} "")
		IF(PM_WITH_SIMD)
			PUSH_TEST(${name}_sse2 ${files} "PM_WITH_SSE;PM_WITH_SSE2;PM_WITH_SIMD")
			IF(PM_WITH_SSE3)
				PUSH_TEST(${name}_sse3 ${files} "PM_WITH_SSE;PM_WITH_SSE2;PM_WITH_SSE3;PM_WITH_SIMD")
			ENDIF()

			IF(PM_WITH_SSSE3)
				PUSH_TEST(${name}_ssse3 ${files} "PM_WITH_SSE;PM_WITH_SSE2;PM_WITH_SSSE3;PM_WITH_SIMD")
			ENDIF()

			IF(PM_WITH_SSE4_1)
				PUSH_TEST(${name}_sse4_1 ${files} "PM_WITH_SSE;PM_WITH_SSE2;PM_WITH_SSE4_1;PM_WITH_SIMD")
			ENDIF()

			IF(PM_WITH_SSE4_2)
				PUSH_TEST(${name}_sse4_2 ${files} "PM_WITH_SSE;PM_WITH_SSE2;PM_WITH_SSE4_2;PM_WITH_SIMD")
			ENDIF()

			IF(PM_WITH_AVX)
				PUSH_TEST(${name}_avx ${files} "PM_WITH_SSE;PM_WITH_SSE2;PM_WITH_AVX;PM_WITH_SIMD")
			ENDIF()
		ENDIF()
	ENDIF()
	PUSH_TEST(${name} ${files} "${DEFS}")
ENDFUNCTION(TEST_SUITE)

#DEPENDIES, LIBARIES AND EXECUTABLES
IF(NOT PM_ONLY_LIBRARY)
	enable_testing()
	TEST_SUITE(frame src/tests/frame.cpp)
	TEST_SUITE(matrix2 src/tests/matrix2.cpp)
	TEST_SUITE(matrix3 src/tests/matrix3.cpp)
	TEST_SUITE(matrix4 src/tests/matrix4.cpp)
	TEST_SUITE(quaternion src/tests/quaternion.cpp)
	TEST_SUITE(vector2d src/tests/vector2d.cpp)
	TEST_SUITE(vector3d src/tests/vector3d.cpp)
	TEST_SUITE(vector4d src/tests/vector4d.cpp)
ENDIF(NOT PM_ONLY_LIBRARY)

#INSTALL
INSTALL(FILES ${PM_Hdr} DESTINATION include/PM)
