add_subdirectory(common)
add_subdirectory(network)

set(TARGETS
    main
)

foreach(name ${TARGETS})
    add_executable(${name} ${name}.cpp)
    target_link_libraries(${name}
    PUBLIC
        # src
        common
        network

        # package
        Threads::Threads
        OpenMP::OpenMP_CXX

        # external
    )

endforeach(name ${TARGETS})
