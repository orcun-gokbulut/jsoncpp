#[[*****************************************************************************
 JsonCpp - CMakeLists.txt
 ------------------------------------------------------------------------------
 Copyright (c) 2022, Yiğit Orçun GÖKBULUT. All rights Reserved.

 This library is free software; you can  redistribute it and/or modify it under
 the terms of the GNU  Lesser General  Public License as  published by the Free
 Software Foundation;  either version 2.1  of the License, or  (at your option)
 any later version.

 This library is  distributed in the hope  that it will be  useful, but WITHOUT
 ANY WARRANTY; without even the implied  warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the GNU Lesser General  Public License for more
 details.

 You should have received a copy of the GNU Lesser General Public License along
 with this library;  if not, write  to the Free  Software Foundation,  Inc., 51
 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

 Copyright Owner Information:
  Name: Yiğit Orçun GÖKBULUT
  Contact: orcun.gokbulut@gmail.com
  Github: https://www.github.com/orcun-gokbulut/ZE
*****************************************************************************]]

cmake_minimum_required(VERSION 3.10)

list(APPEND Sources Json.cpp)
list(APPEND PublicHeaders Json.h)
list(APPEND Sources JsonProperty.cpp)
list(APPEND Sources JsonTokenizer.cpp)
list(APPEND Headers JsonTokenizer.h)


if(NOT PROJECT_IS_TOP_LEVEL)
    add_library(jsoncpp)
else()
    add_library(jsoncpp SHARED)
endif()

target_sources(jsoncpp PRIVATE ${Sources} ${Headers} ${PublicHeaders})
set_target_properties(jsoncpp PROPERTIES PUBLIC_HEADER ${PublicHeaders})
target_include_directories(jsoncpp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if(PROJECT_IS_TOP_LEVEL)
    install(TARGETS jsoncpp)
endif()


if(JSONCPP_BUILD_TESTS)
    add_executable(jsoncpp.test Json.Test.cpp)
    target_link_libraries(jsoncpp.test jsoncpp Catch2::Catch2WithMain)
    add_test(NAME Json COMMAND $<TARGET_FILE:jsoncpp.test>)
endif()
