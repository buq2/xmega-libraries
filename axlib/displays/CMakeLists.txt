SET(DISPLAY_FILES
    display_buffer.hh display_buffer.cpp
    fonts/fontlibrary.h fonts/fontlibrary.c
    )
IF (USE_MCU)
    SET(DISPLAY_FILES
        ${DISPLAY_FILES}
        display_sharp.hh display_sharp.cpp
        )
ENDIF()

ADD_LIBRARY(axDisplays
    ${DISPLAY_FILES}
    )

TARGET_LINK_LIBRARIES(axDisplays
    axCore
    )
