# register_component
function(register_component)
    set(options "")
    set(oneValueArgs "")
    set(multiValueArgs SRCS INCLUDE_DIRS REQUIRES PRIV_REQUIRES)
    cmake_parse_arguments(COMP "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

# Use the current directory name as the component library name
    get_filename_component(comp_name ${CMAKE_CURRENT_LIST_DIR} NAME)

# Add the components to the global list for easy linking by the main program
    set_property(GLOBAL APPEND PROPERTY STM32_COMPONENTS ${comp_name})

# Create the static library for the component
    add_library(${comp_name} STATIC ${COMP_SRCS})

# Export the file path (PUBLIC corresponds to REQUIRES, PRIVATE corresponds to PRIV_REQUIRES)
    target_include_directories(${comp_name} PUBLIC ${COMP_INCLUDE_DIRS})

# Link the component with its dependencies
    target_link_libraries(${comp_name} PUBLIC ${COMP_REQUIRES} PRIVATE ${COMP_PRIV_REQUIRES})
endfunction()


