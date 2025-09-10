
# Open Database Connectivity (ODBC)
# apt install -y unixodbc-dev

find_package(ODBC)

if(FALSE AND ODBC_FOUND)
    ###
    ### MdsODBC
    ###

    add_library(
        MdsODBC
        mdsodbc.c
    )

    target_link_libraries(
        MdsODBC
        PUBLIC
            ODBC::ODBC
    )

    mdsplus_add_static_copy(MdsODBC _static_target)

    install(TARGETS MdsODBC ${_static_target})
endif()