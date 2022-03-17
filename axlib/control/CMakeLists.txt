ADD_LIBRARY(axControl STATIC
    pid_controller.hh pid_controller.hpp
    )
SET_TARGET_PROPERTIES(axControl PROPERTIES LINKER_LANGUAGE CXX)
