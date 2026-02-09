file(GLOB sub_dirs RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*")

foreach(dir ${sub_dirs})
    if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${dir} AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${dir}/CMakeLists.txt)
        message(STATUS "Found BSP component: ${dir}")
        add_subdirectory(${dir})
    endif()
endforeach()