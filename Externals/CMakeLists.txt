if(IS_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/Catch2")
    set(JSONCPP_CATCH2_AVAILABLE True PARENT_SCOPE)
    add_subdirectory(Catch2)
endif()
