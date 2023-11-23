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
list(APPEND Sources JsonQueryTokenizer.cpp)
list(APPEND Headers JsonQueryTokenizer.h)
list(APPEND Sources JsonExceptions.cpp)
list(APPEND PublicHeaders JsonExceptions.h)

if(JSONCPP_BUILD_STATIC)
    add_library(jsoncpp-static)
    target_sources(jsoncpp-static PRIVATE ${Sources} ${Headers} ${PublicHeaders})
    set_target_properties(jsoncpp-static PROPERTIES PUBLIC_HEADER "${PublicHeaders}")
    set_target_properties(jsoncpp-static PROPERTIES OUTPUT_NAME "jsoncpp")
    target_include_directories(jsoncpp-static PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

    if(PROJECT_IS_TOP_LEVEL)
        install(
            TARGETS jsoncpp-static
            COMPONENT dev-static
        )
    endif()
endif()

if(JSONCPP_BUILD_SHARED)
    add_library(jsoncpp-shared SHARED)
    target_sources(jsoncpp-shared PRIVATE ${Sources} ${Headers} ${PublicHeaders})
    target_include_directories(jsoncpp-shared PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

    set_target_properties(jsoncpp-shared PROPERTIES PUBLIC_HEADER "${PublicHeaders}")
    set_target_properties(jsoncpp-shared PROPERTIES OUTPUT_NAME "jsoncpp")
    set_target_properties(jsoncpp-shared PROPERTIES SOVERSION 1)

    if(PROJECT_IS_TOP_LEVEL)
        install(
            TARGETS jsoncpp-shared
                RUNTIME
                    COMPONENT runtime
                PUBLIC_HEADER
                    COMPONENT dev
                    DESTINATION include
                LIBRARY
                    COMPONENT runtime
                    DESTINATION lib/${CMAKE_LIBRARY_ARCHITECTURE}
                    NAMELINK_COMPONENT dev
        )
    endif()
endif()

if(JSONCPP_BUILD_TESTS AND JSONCPP_CATCH2_AVAILABLE)
    add_executable(jsoncpp.test Json.Test.cpp)
    target_link_libraries(jsoncpp.test jsoncpp-shared Catch2 Catch2WithMain)
    add_test(NAME Json COMMAND $<TARGET_FILE:jsoncpp.test>)
endif()
