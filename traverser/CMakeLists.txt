
SET(SOURCES
    main-uil.c
    bxutils-uil.c
    CallbacksUil.c
    ReadInt.c
    ReadString.c
    GetNciString.c
    GetSupportedDevices.c
)

ADD_EXECUTABLE(
    traverser
    ${SOURCES}
)

TARGET_LINK_LIBRARIES(
    traverser
    PRIVATE
        MdsShr
        TreeShr
        TdiShr
        Mdsdcl
        XmdsShr
        Motif::Motif
)