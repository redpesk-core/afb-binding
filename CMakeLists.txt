# Copyright (C) 2015-2024 IoT.bzh Company
# Author: José Bollo <jose.bollo@iot.bzh>
#
# SPDX-License-Identifier: LGPL-3.0-only

cmake_minimum_required(VERSION 3.0)

project(afb-binding
	VERSION 4.1.9
	DESCRIPTION "Binding headers for Application Framework Binder"
	HOMEPAGE_URL "https://github.com/redpesk-core/afb-binding"
	LANGUAGES C)

include(GNUInstallDirs)

set(LIBAFB_VERSION_MIN 5.0.11)

# install headers
install(DIRECTORY afb
        DESTINATION ${CMAKE_INSTALL_FULL_INCLUDEDIR})

# install configuration file
configure_file(afb-binding.pc.in afb-binding.pc @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/afb-binding.pc
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)

# install documentation and tutorials
install(DIRECTORY docs tutorials
        DESTINATION ${CMAKE_INSTALL_FULL_DATADIR}/afb-binding
        PATTERN *.odg  EXCLUDE
        PATTERN *.so   EXCLUDE
        PATTERN .*.swp EXCLUDE
        PATTERN .sed   EXCLUDE
)
