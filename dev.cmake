# confy
# Copyright (c) 2020 Borislav Stanimirov
#
# Distributed under the MIT Software License
# See accompanying file LICENSE.txt or copy at
# https://opensource.org/licenses/MIT
#
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(USE_ASAN "Confy-dev: build with address sanitizer on" OFF)
option(USE_CLANG_TIDY "Confy-dev: use clang tidy" OFF)

set(DEV_SAN_FLAGS "")
if(MSVC)
    set(DEV_WARNING_FLAGS "-D_CRT_SECURE_NO_WARNINGS /wd4251 /wd4275")
else()
    set(DEV_WARNING_FLAGS "-Wall -Wextra -Wno-unused-variable")
    if(USE_ASAN)
        set(DEV_SAN_FLAGS "-fsanitize=address,undefined,leak -g")
    endif()

    if(USE_CLANG_TIDY)
        set(CMAKE_CXX_CLANG_TIDY clang-tidy)
    endif()
endif()

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${DEV_WARNING_FLAGS} ${DEV_SAN_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${DEV_WARNING_FLAGS} ${DEV_SAN_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${DEV_SAN_FLAGS}")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${DEV_SAN_FLAGS}")

# all executables to bin
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
