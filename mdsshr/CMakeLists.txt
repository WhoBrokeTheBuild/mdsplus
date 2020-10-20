
SET(SOURCES
    librtl.c
    mdsmsg.c
    MDSprintf.c
    MdsCmprs.c
    MdsCompareXd.c
    MdsCompress.c
    MdsEvents.c
    MdsGet1DxA.c
    MdsGet1DxS.c
    MdsGetSetShotId.c
    MdsGetStdMsg.c
    MdsPk.c
    MdsSandbox.c
    MdsSerialize.c
    MdsThreadStatic.c
    MdsXdRoutines.c
    mds_dsc_string.c
    UdpEvents.c
    UdpEventSettings.c
)

CONFIGURE_FILE(
    ${CMAKE_CURRENT_SOURCE_DIR}/version.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/version.h
)

ADD_LIBRARY(
    MdsShr
    SHARED
    ${SOURCES}
)

SET_TARGET_PROPERTIES(
    MdsShr
    PROPERTIES
        PREFIX ""
)


TARGET_LINK_LIBRARIES(
    MdsShr
    PUBLIC
        libxml2::libxml2
)

TARGET_INCLUDE_DIRECTORIES(
    MdsShr
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_BINARY_DIR}/include
)