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

cmake_minimum_required(VERSION 3.21)
project(JsonCpp VERSION 0.0.1 LANGUAGES CXX)

add_subdirectory(Externals)

if(PROJECT_IS_TOP_LEVEL AND JSONCPP_CATCH2_AVAILABLE)
    set(JSONCPP_BUILD_TESTS FALSE CACHE BOOL "Enable unit tests")
endif()

enable_testing()
add_subdirectory(Sources)
