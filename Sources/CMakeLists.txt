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

cmake_minimum_required(VERSION 3.10)

list(APPEND Sources Json.cpp)
list(APPEND PublicHeaders Json.h)
list(APPEND Sources JsonProperty.cpp)
list(APPEND Sources JsonTokenizer.cpp)
list(APPEND Headers JsonTokenizer.h)
list(APPEND Sources JsonExceptions.cpp)
list(APPEND PublicHeaders JsonExceptions.h)


if(NOT PROJECT_IS_TOP_LEVEL OR JSONCPP_BUILD_STATIC)
    add_library(jsoncpp)
else()
    add_library(jsoncpp SHARED)
endif()

target_sources(jsoncpp PRIVATE ${Sources} ${Headers} ${PublicHeaders})
set_target_properties(jsoncpp PROPERTIES PUBLIC_HEADER "${PublicHeaders}")
target_include_directories(jsoncpp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if(PROJECT_IS_TOP_LEVEL)
    install(TARGETS jsoncpp)
endif()

if(JSONCPP_BUILD_TESTS AND JSONCPP_CATCH2_AVAILABLE)
    add_executable(jsoncpp.test Json.Test.cpp)
    target_link_libraries(jsoncpp.test jsoncpp Catch2 Catch2WithMain)
    add_test(NAME Json COMMAND $<TARGET_FILE:jsoncpp.test>)
endif()
