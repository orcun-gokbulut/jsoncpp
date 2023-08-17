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
project(JsonCpp VERSION 0.0.1 LANGUAGES CXX)

add_subdirectory(Externals)

set(JSONCPP_BUILD_SHARED FALSE CACHE BOOL "Generate shared library.")

if(JSONCPP_CATCH2_AVAILABLE)
    set(JSONCPP_BUILD_TESTS FALSE CACHE BOOL "Enable unit tests")
endif()

enable_testing()
add_subdirectory(Sources)
