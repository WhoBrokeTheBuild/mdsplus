
SET(SOURCES
    mdsdclThreadStatic.c
    cmdExecute.c
    cmdHelp.c
    cmdParseLex.c
    cmdParse.c
    mdsdclDeltatimeToSeconds.c
    mdsdcl_commands.c
    mdsdclVersionInfo.c
    mdsdcl_show_version.c
)

Set(VERSION_SOURCE
    MdsVersionInfo.c
)

ADD_LIBRARY(
    Mdsdcl
    SHARED
    ${SOURCES}
)

SET_TARGET_PROPERTIES(
    Mdsdcl
    PROPERTIES
        PREFIX ""
)

TARGET_INCLUDE_DIRECTORIES(
    Mdsdcl
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/include
)

TARGET_LINK_LIBRARIES(
    Mdsdcl
    PUBLIC
        MdsShr
        TreeShr
        Readline::Readline
)