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

set(sources
    Json.cpp
    JsonTokenizer.cpp
)

set(headers
    Json.h
)

add_library(jsoncpp SHARED)
target_sources(jsoncpp PRIVATE ${sources} ${headers})
set_target_properties(jsoncpp PROPERTIES PUBLIC_HEADER ${headers})
target_include_directories(
    jsoncpp
    PUBLIC
        $<INSTALL_INTERFACE:include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}
)
install(TARGETS jsoncpp)


set(BUILD_EXAMPLE false CACHE BOOL "Build test program.")
if (${BUILD_EXAMPLE})
    add_executable(jsoncpp.test Json.test.cpp)
    target_link_libraries(jsoncpp.test jsoncpp)
endif()
