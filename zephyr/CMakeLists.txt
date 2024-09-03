if(CONFIG_AFB_BINDER)
    set(WITH_ZEPHYR 1)
    add_subdirectory(.. afb-binding)
endif()