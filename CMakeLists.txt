# Copyright (c) Borislav Stanimirov
# SPDX-License-Identifier: MIT
#
cmake_minimum_required(VERSION 3.24 FATAL_ERROR)

project(confy
    LANGUAGES CXX
)

# cpm
if(NOT CPM_SOURCE_CACHE AND NOT DEFINED ENV{CPM_SOURCE_CACHE})
    set(CPM_SOURCE_CACHE "${CMAKE_SOURCE_DIR}/.cpm" CACHE PATH "CPM source cache")
    message(STATUS "Setting cpm cache dir to: ${CPM_SOURCE_CACHE}")
endif()
include(./get_cpm.cmake)

#######################################
# cmake lib
CPMAddPackage(gh:iboB/icm@1.5.5)

list(APPEND CMAKE_MODULE_PATH
    "${icm_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake"
)

set(ICM_DEV_CXX_STANDARD 20)

include(icm_dev_mode)

#######################################
# options

option(BUILD_SHARED_LIBS "${PROJECT_NAME}: build shared libraries" ON)
option(CONFY_BUILD_TESTS "${PROJECT_NAME}: build tests" ${ICM_DEV_MODE})
option(CONFY_BUILD_EXAMPLES "${PROJECT_NAME}: build examples" ${ICM_DEV_MODE})
option(CONFY_BUILD_SANDBOX "${PROJECT_NAME}: build sandbox project (dev experiments)" ${ICM_DEV_MODE})
mark_as_advanced(CONFY_BUILD_TESTS CONFY_BUILD_EXAMPLES CONFY_BUILD_SANDBOX)

#######################################
# code
add_subdirectory(code)

if(CONFY_BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()

if(CONFY_BUILD_EXAMPLES)
    add_subdirectory(example)
endif()

if(CONFY_BUILD_SANDBOX)
    add_subdirectory(sandbox)
endif()
