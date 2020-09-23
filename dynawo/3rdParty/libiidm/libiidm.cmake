# Copyright (c) 2015-2020, RTE (http://www.rte-france.com)
# See AUTHORS.txt
# All rights reserved.
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, you can obtain one at http://mozilla.org/MPL/2.0/.
# SPDX-License-Identifier: MPL-2.0
#
# This file is part of Dynawo, an hybrid C++/Modelica open source time domain simulation tool for power systems.

cmake_minimum_required(VERSION 3.12)

include($ENV{DYNAWO_HOME}/dynawo/cmake/CPUCount.cmake)
if(NOT DEFINED CPU_COUNT)
  message(FATAL_ERROR "CPUCount.cmake: file not found.")
endif()

set(packet_name        "libiidm")
set(packet_config_dir  "LibIIDM")
set(packet_install_dir "${CMAKE_INSTALL_PREFIX}/${packet_name}")
string(TOUPPER "${packet_name}" packet_uppername)
set(packet_RequiredVersion 1.2.0)

set(CMAKE_PREFIX_PATH "${CMAKE_INSTALL_PREFIX}/${packet_name}/${packet_config_dir}")

find_package(${packet_name} ${packet_RequiredVersion} EXACT QUIET CONFIG)
if(${packet_name}_FOUND)

  add_custom_target("${packet_name}" DEPENDS libxml2 boost)
  message(STATUS "Found ${packet_name} ${PACKAGE_VERSION}")

else()

  set(packet_git_repo    "https://github.com/powsybl/powsybl-iidm4cpp")

  include(${CMAKE_ROOT}/Modules/ExternalProject.cmake)
  ExternalProject_Add(

                      "${packet_name}"

    DEPENDS           libxml2 boost
    INSTALL_DIR       "${packet_install_dir}"

    GIT_REPOSITORY    "${packet_git_repo}"
    GIT_TAG           "v${packet_RequiredVersion}"
    GIT_PROGRESS      1

    UPDATE_COMMAND    ""

    DOWNLOAD_DIR      "${CMAKE_CURRENT_SOURCE_DIR}/${packet_name}"
    TMP_DIR           "${CMAKE_CURRENT_BINARY_DIR}/tmp_dir"
    STAMP_DIR         "${CMAKE_CURRENT_BINARY_DIR}/stamp_dir"
    BINARY_DIR        "${CMAKE_CURRENT_BINARY_DIR}/binary_dir"
    SOURCE_DIR        "${CMAKE_CURRENT_BINARY_DIR}/source_dir"

    CMAKE_CACHE_ARGS  "-DCMAKE_CXX_COMPILER:STRING=${CMAKE_CXX_COMPILER}"
                      "-DCXX11_ENABLED:BOOL=${CXX11_ENABLED}"

    CMAKE_ARGS        "-DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>"
                      "-DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}"
                      "-DIIDM_BUILD_SHARED:BOOL=$<BOOL:${BUILD_SHARED_LIBS}>"
                      "-DBUILD_XML:BOOL=ON"
                      "-DBUILD_SAMPLES:BOOL=$<BOOL:${LIBIIDM_BUILD_SAMPLES}>"
                      "-DLIBXML2_LIBRARY=${LIBXML2_LIBRARY}"            --> Ajout DG
                      "-DLIBXML2_INCLUDE_DIR=${LIBXML2_INCLUDE_DIR}"    --> Ajout DG
                      "-DBOOST_ROOT:PATH=${BOOST_ROOT}"
                      "-DMSVC_STATIC_RUNTIME_LIBRARY=${MSVC_STATIC_RUNTIME_LIBRARY}"

    BUILD_COMMAND     make -j ${CPU_COUNT} all
  )

  unset(packet_git_repo)

endif(${packet_name}_FOUND)

unset(packet_RequiredVersion)
unset(packet_upper_name)
unset(packet_install_dir)
unset(packet_config_dir)
unset(packet_name)
