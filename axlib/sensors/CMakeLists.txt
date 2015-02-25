ADD_LIBRARY(axSensors
    altimeter_mpl3115a2.hh altimeter_mpl3115a2.cpp
    altimeter_ms5805_02ba01.hh altimeter_ms5805_02ba01.cpp
    gps_sim33ela.hh gps_sim33ela.cpp
    thermo_max6675.hh thermo_max6675.cpp
    )

TARGET_LINK_LIBRARIES(axSensors
    lufa)
