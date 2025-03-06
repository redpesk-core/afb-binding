# Copyright (C) 2015-2025 IoT.bzh Company
# Author: José Bollo <jose.bollo@iot.bzh>
#
# SPDX-License-Identifier: LGPL-3.0-only

cmake_minimum_required(VERSION 3.10)

project(afb-binding
	VERSION 4.1.11
	DESCRIPTION "Binding headers for Application Framework Binder"
	HOMEPAGE_URL "https://github.com/redpesk-core/afb-binding"
	LANGUAGES C)

set(LIBAFB_VERSION_MIN 5.0.11)

# install headers
# Detect the platform reliably
if(ZEPHYR_BASE)
	if (NOT CONFIG_AFB_ZEPHYR AND NOT CONFIG_AFB_LLEXT_BINDING)
		return()
	endif()
	set(WITH_ZEPHYR YES)
        zephyr_include_directories(${CMAKE_CURRENT_SOURCE_DIR}/afb)
else()
	set(WITH_ZEPHYR NO)
        include(GNUInstallDirs)
        install(DIRECTORY afb
                DESTINATION ${CMAKE_INSTALL_FULL_INCLUDEDIR})

        # install documentation and tutorials
        install(DIRECTORY docs tutorials
                DESTINATION ${CMAKE_INSTALL_FULL_DATADIR}/afb-binding
                PATTERN *.odg  EXCLUDE
                PATTERN *.so   EXCLUDE
                PATTERN .*.swp EXCLUDE
                PATTERN .sed   EXCLUDE
        )

	# install configuration file
	configure_file(afb-binding.pc.in afb-binding.pc @ONLY)
	install(FILES ${CMAKE_CURRENT_BINARY_DIR}/afb-binding.pc
		DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)
endif()
