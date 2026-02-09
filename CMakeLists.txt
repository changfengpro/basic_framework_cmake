cmake_minimum_required(VERSION 3.22)

#
# This file is generated only once,
# and is not re-generated if converter is called multiple times.
#
# User is free to modify the file as much as necessary
#

include (cmake/project.cmake)
include (cmake/gcc-arm-none-eabi.cmake)

# Set the project name
set(CMAKE_PROJECT_NAME basic_framework)

# Enable compile command to ease indexing with e.g. clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Core project settings
project(${CMAKE_PROJECT_NAME})
message("Build type: " ${CMAKE_BUILD_TYPE})

# Define component search path
set(COMPONENT_DIRS "Component" "Core" "Drivers" "Application")

foreach(dir ${COMPONENT_DIRS})
    get_filename_component(abs_dir ${dir} ABSOLUTE)
    if(EXISTS ${abs_dir})
        # Search for CMakeLists.txt in the subdirectories 
        file(GLOB all_subdirs RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "${dir}/*")
        foreach(sub ${all_subdirs})
            if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${sub} AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${sub}/CMakeLists.txt)
                add_subdirectory(${sub})
            endif()
        endforeach()
    endif()
endforeach()

# Setup compiler settings
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)


# Define the build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

# Enable CMake support for ASM and C languages
enable_language(C ASM)

# Create an executable object type
add_executable(${CMAKE_PROJECT_NAME})

# Add STM32CubeMX generated sources
add_subdirectory(cmake/stm32cubemx)

# Link directories setup
target_link_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined library search paths
)

# Add sources to executable
target_sources(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user sources here
)

# Add include paths
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined include paths
)

# Add project symbols (macros)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
    # Add user defined symbols
)

# Remove wrong libob.a library dependency when using cpp files
list(REMOVE_ITEM CMAKE_C_IMPLICIT_LINK_LIBRARIES ob)

get_property(TOTAL_COMPONENTS GLOBAL PROPERTY STM32_COMPONENTS)

# Add linked libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
    stm32cubemx
    ${TOTAL_COMPONENTS}
    # Add user defined libraries
)

