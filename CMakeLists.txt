# Copyright (C) 2015-2020 IoT.bzh Company
# Author: José Bollo <jose.bollo@iot.bzh>
#
# SPDX-License-Identifier: LGPL-3.0-only

cmake_minimum_required(VERSION 3.0)

project(afb-binding C)
set(PROJECT_NAME afb-binding)
set(PROJECT_DESCRIPTION "Binding headers for Application Framework Binder")
set(PROJECT_VERSION 1.2)
set(PROJECT_URL "https://github.com/redpesk/afb-binding")

include(GNUInstallDirs)

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
        PATTERN *.odg EXCLUDE)
