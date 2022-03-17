SET(CORE_FILES
    core.hh core.cpp core.hh
    )
IF (USE_MCU)
    SET(CORE_FILES
        ${CORE_FILES}
        io.hh io.cpp
        pwm.hh pwm.cpp
        )
ENDIF()

ADD_LIBRARY(axCore STATIC
    ${CORE_FILES}
    )
