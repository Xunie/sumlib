function( SUMLIB_ADD_TEST name )
    add_executable( test-${name}
        tests/${name}.cpp
    )

    target_link_libraries( test-${name}
        PUBLIC
            sumlib
    )

    add_test( NAME test-${name}
              COMMAND test-${name} )
endfunction()

SUMLIB_ADD_TEST( precision )
