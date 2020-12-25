cmake_minimum_required( VERSION 3.0 )

project( sumlib )

include( CTest )

add_library( sumlib INTERFACE )

target_include_directories( sumlib
    INTERFACE
        include
)

option( SUMLIB_BUILD_EXAMPLES "These are some examples showcasing the library." OFF )
if( SUMLIB_BUILD_EXAMPLES )
    add_executable( example
        examples/example.cpp
    )

    target_link_libraries( example PRIVATE sumlib )
    set_property( TARGET example
        PROPERTY
            CXX_STANDARD 17
    )


    add_executable( example2
        examples/example2.cpp
    )

    target_link_libraries( example2 PRIVATE sumlib )
    set_property( TARGET example2
        PROPERTY
            CXX_STANDARD 17
    )
endif()

include( cmake/tests.cmake )
