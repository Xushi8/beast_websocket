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
        TBB::tbb
        mimalloc


        # Boost::filesystem

        ${RE2_LIB}

        ${GMP_LIB}
        ${MPFR_LIB}

        ${PQ_LIB}
        ${PQXX_LIB}

        # external
    )

endforeach(name ${TARGETS})

if(BOOST_WEBSOCKET_USE_GUI)
    add_subdirectory(gui)

    add_executable(template_project_gui template_project_gui.cpp)
    target_link_libraries(template_project_gui
    PUBLIC
        # src
        common
        network
        gui

        # package
        Threads::Threads
        OpenMP::OpenMP_CXX
        TBB::tbb
        mimalloc

        Qt6::Widgets

        # Boost::filesystem

        ${RE2_LIB}

        ${GMP_LIB}
        ${MPFR_LIB}

        ${PQ_LIB}
        ${PQXX_LIB}

        # external
    )
endif()