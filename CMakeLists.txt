#[[**************************************************************
 JsonCpp - CMakeLists.txt
 ----------------------------------------------------------------
 Copyright (c) 2022, Yiğit Orçun GÖKBULUT. All rights Reserved.

 Use of this source code is governed by a BSD-style
 license that can be found in the LICENSE file or at
 https://developers.google.com/open-source/licenses/bsd

 Copyright Owner Information:
  Name: Yiğit Orçun GÖKBULUT
  Contact: orcun.gokbulut@gmail.com
  Github: https://www.github.com/orcun-gokbulut
****************************************************************]]

cmake_minimum_required(VERSION 3.21)
project(jsoncpp VERSION 1.0.0 LANGUAGES CXX)

set(JSONCPP_BUILD_SHARED TRUE CACHE BOOL "Generate shared library.")
set(JSONCPP_BUILD_STATIC FALSE CACHE BOOL "Generate static library.")
set(JSONCPP_BUILD_TESTS FALSE CACHE BOOL "Enable unit tests")

if(JSONCPP_BUILD_TESTS)
    enable_testing()
endif()

add_subdirectory(Externals)
add_subdirectory(Sources)

set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_FILE_NAME "DEB-DEFAULT")
set(CPACK_DEBIAN_PACKAGE_NAME "libjsoncpp")
set(CPACK_DEBIAN_PACKAGE_SECTION "libdevel")
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS true)
set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "https://www.github.com/orcun-gokbulut/jsoncpp")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Y. Orçun Gökbulut <orcun.gokbulut@gmail.com>")

set(CPACK_DEB_COMPONENT_INSTALL true)
set(CPACK_DEBIAN_ENABLE_COMPONENT_DEPENDS true)
set(CPACK_DEBIAN_RUNTIME_PACKAGE_NAME "libjsoncpp")
set(CPACK_DEBIAN_RUNTIME_DESCRIPTION "Simple Json DOM Library")

set(CPACK_DEBIAN_DEV_PACKAGE_NAME "libjsoncpp-dev")
set(CPACK_DEBIAN_DEV_DESCRIPTION "Development files for JsonCpp library.")
set(CPACK_COMPONENT_DEV_DEPENDS "runtime")






include(CPack)