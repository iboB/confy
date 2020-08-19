set(src
    include/confy/impl/api.h

    include/confy/confy.hpp

    confy.cpp
)

if(CONFY_STATIC_LIB)
    add_library(confy STATIC ${src})
else()
    add_library(confy SHARED ${src})

    if(UNIX)
        target_compile_options(confy PRIVATE -fvisibility=hidden)
    endif()

    target_compile_definitions(confy
        PUBLIC -DCONFY_SHARED=1
    )
endif()

target_compile_definitions(confy
    PRIVATE -DCONFY_IMPLEMENT=1
)

target_include_directories(confy PUBLIC include)

# option(CONFY_GENERATE_SINGLE_HEADER "Confy-dev: generate single header" ${dev_mode})
