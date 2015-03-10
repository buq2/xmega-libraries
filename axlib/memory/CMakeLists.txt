ADD_LIBRARY(axMemory
    flash_s25fl216k.hh flash_s25fl216k.cpp
    )

TARGET_LINK_LIBRARIES(axMemory
    lufa
    axCore
    )
