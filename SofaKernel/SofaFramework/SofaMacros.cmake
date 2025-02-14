include(CMakePackageConfigHelpers)
include(CMakeParseLibraryList)


# - Create an imported target from a library path and an include dir path.
#   Handle the special case where LIBRARY_PATH is in fact an existing target.
#   Handle the case where LIBRARY_PATH contains the following syntax supported by cmake:
#                                      "optimized /usr/lib/foo.lib debug /usr/lib/foo_d.lib"
#
# sofa_create_imported_target(TARGETNAME LIBRARY_PATH INCLUDE_DIRS)
#  TARGETNAME_Target  - (output) variable which contains the name of the created target.
#                       It is usually contains TARGETNAME with one notable exception.
#                       If LIBRARY_PATH is an existing target, TARGETNAME_Target
#                       contains LIBRARY_PATH instead.
#  TARGETNAME         - (input) the name of the target to create.
#  NAMESPACE          - (input) the namespace where the target is put.
#  LIBRARY_PATH       - (input) the path to the library ( .so or .lib depending on the platform)
#  INCLUDE_DIRS       - (input) include directories associated with the library,
#                       which are added as INTERFACE_INCLUDE_DIRECTORIES for the target.
#
# The typical usage scenario is to convert the absolute paths to a system library that cmake return
# after a find_package call into an imported target. By using the cmake target mechanism, it is
# easier to redistribute a software that depends on system libraries, whose locations are not
# known before hand on the consumer system.
#
# For further reference about this subject :
# http://public.kitware.com/pipermail/cmake-developers/2014-March/009983.html
# Quoted from https://github.com/Kitware/CMake/blob/master/Help/manual/cmake-packages.7.rst
# "Note that it is not advisable to populate any properties which may contain paths,
#  such as :prop_tgt:`INTERFACE_INCLUDE_DIRECTORIES` and :prop_tgt:`INTERFACE_LINK_LIBRARIES`,
#  with paths relevnt to dependencies. That would hard-code into installed packages the
#  include directory or library paths for dependencies as found on the machine the package
#  was made on."
#
# Example:
#
# add_library( SHARED myLib )
# find_package(PNG REQUIRED)
# sofa_create_target( PNG MyNamespace "${PNG_LIBRARY}" "${PNG_INCLUDE_DIRS}" )
# target_link_libraries( myLib PUBLIC ${PNG_Target} )
#
macro(sofa_create_target TARGETNAME NAMESPACE LIBRARY_PATH INCLUDE_DIRS)
    # message("TARGETNAME ${TARGETNAME}")
    set(NAMESPACE_TARGETNAME "${NAMESPACE}::${TARGETNAME}")
    # message("LIBRARY_PATH ${LIBRARY_PATH}")
    parse_library_list( "${LIBRARY_PATH}" FOUND LIB_FOUND DEBUG LIB_DEBUG OPT LIB_OPT GENERAL LIB_GEN )

    # message("FOUND ${LIB_FOUND} DEBUG: ${LIB_DEBUG} OPT: ${LIB_OPT} GEN: ${LIB_GEN}")
    if(LIB_FOUND)
        if(NOT TARGET ${TARGETNAME} )
            set(${TARGETNAME}_Target ${NAMESPACE_TARGETNAME} )
            if(NOT TARGET ${NAMESPACE_TARGETNAME} )
                add_library( ${NAMESPACE_TARGETNAME} UNKNOWN IMPORTED )
                set_target_properties( ${NAMESPACE_TARGETNAME} PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${INCLUDE_DIRS}" )
                if( NOT ${LIB_DEBUG} STREQUAL "")
                    set_target_properties( ${NAMESPACE_TARGETNAME} PROPERTIES IMPORTED_LOCATION_DEBUG "${LIB_DEBUG}" )
                endif()
                if( NOT ${LIB_OPT} STREQUAL "")
                    set_target_properties( ${NAMESPACE_TARGETNAME} PROPERTIES IMPORTED_LOCATION "${LIB_OPT}" )
                elseif( NOT ${LIB_GEN} STREQUAL "" )
                    set_target_properties( ${NAMESPACE_TARGETNAME} PROPERTIES IMPORTED_LOCATION "${LIB_GEN}" )
                endif()
            endif()
        else()
            message( SEND_ERROR "sofa_create_target error. ${TARGETNAME} is an already an existing TARGET.\
                                 Choose a different name.")
        endif()
    else()
        if(NOT TARGET "${LIBRARY_PATH}" )
            # message("${LIBRARY_PATH} is not a TARGET")
            if(NOT TARGET ${TARGETNAME} )
                # message("${TARGETNAME} is not a TARGET")
                set(${TARGETNAME}_Target ${NAMESPACE_TARGETNAME} )
                if(NOT TARGET ${NAMESPACE_TARGETNAME} )
                    # message("${NAMESPACE_TARGETNAME} is not a TARGET")
                    add_library( ${NAMESPACE_TARGETNAME} UNKNOWN IMPORTED )
                    set_target_properties( ${NAMESPACE_TARGETNAME} PROPERTIES IMPORTED_LOCATION "${LIBRARY_PATH}" )
                    set_target_properties( ${NAMESPACE_TARGETNAME} PROPERTIES INTERFACE_INCLUDE_DIRECTORIES "${INCLUDE_DIRS}" )
                endif()
            else()
                message( SEND_ERROR "sofa_create_target error. ${TARGETNAME} is an already an existing TARGET.\
                                     Choose a different name.")
            endif()
        else()
            # message("${LIBRARY_PATH} is a TARGET")
            set(${TARGETNAME}_Target ${LIBRARY_PATH} )
        endif()

    endif()
endmacro()



macro(sofa_add_generic directory name type)
    if(EXISTS "${CMAKE_CURRENT_LIST_DIR}/${directory}" AND IS_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/${directory}")

        string(TOUPPER ${type}_${name} option)

        # optional parameter to activate/desactivate the option
        #  e.g.  sofa_add_application( path/MYAPP MYAPP APPLICATION ON)
        set(active OFF)
        if(${ARGV3})
            if( ${ARGV3} STREQUAL ON )
                set(active ON)
            endif()
        endif()

        option(${option} "Build the ${name} ${type}." ${active})
        if(${option})
            message("Adding ${type} ${name}")
            add_subdirectory(${directory} ${name})
            #Check if the target has been successfully added
            if(TARGET ${name})
                set_target_properties(${name} PROPERTIES FOLDER ${type}s) # IDE folder
                set_target_properties(${name} PROPERTIES DEBUG_POSTFIX "_d")
            endif()
        endif()

        # Add current target in the internal list only if not present already
        get_property(_allTargets GLOBAL PROPERTY __GlobalTargetList__)
        get_property(_allTargetNames GLOBAL PROPERTY __GlobalTargetNameList__)

        # if(NOT ${name} IN_LIST _allTargets) # ONLY CMAKE >= 3.3 and policy to NEW
        list (FIND _allTargets ${name} _index)
        if(NOT ${_index} GREATER -1)
            set_property(GLOBAL APPEND PROPERTY __GlobalTargetList__ ${name})
        endif()

        #if(NOT ${option} IN_LIST _allTargetNames)# ONLY CMAKE >= 3.3 and policy to NEW
        list (FIND _allTargetNames ${option} _index)
        if(NOT ${_index} GREATER -1)
            set_property(GLOBAL APPEND PROPERTY __GlobalTargetNameList__ ${option})
        endif()
    else()
        message("${type} ${name} (${CMAKE_CURRENT_LIST_DIR}/${directory}) does not exist and will be ignored.")
    endif()
endmacro()


macro(sofa_add_plugin directory plugin_name)
    sofa_add_generic( ${directory} ${plugin_name} "Plugin" ${ARGV2} )
endmacro()


macro(sofa_add_plugin_experimental directory plugin_name)
    sofa_add_generic( ${directory} ${plugin_name} "Plugin" ${ARGV2} )
    message("-- ${plugin_name} is an experimental feature, use it at your own risk.")
endmacro()


macro(sofa_add_module directory module_name)
    sofa_add_generic( ${directory} ${module_name} "Module" ${ARGV2} )
endmacro()

macro(sofa_add_module_experimental directory module_name)
    sofa_add_generic( ${directory} ${module_name} "Module" ${ARGV2} )
    message("-- ${module_name} is an experimental feature, use it at your own risk.")
endmacro()

macro(sofa_add_application directory app_name)
    sofa_add_generic( ${directory} ${app_name} "Application" ${ARGV2} )
endmacro()


### External projects management
# Thanks to http://crascit.com/2015/07/25/cmake-gtest/
#
# Use this macro (subdirectory or plugin version) to add out-of-repository projects.
# Usage:
# Add repository configuration in MyProjectDir/ExternalProjectConfig.cmake.in
# Call sofa_add_subdirectory_external(MyProjectDir MyProjectName)
# Or sofa_add_plugin_external(MyProjectDir MyProjectName [ON,OFF])
# See plugins/SofaHighOrder for example
#
macro(sofa_add_generic_external directory name type)
    if(NOT EXISTS "${CMAKE_CURRENT_LIST_DIR}/${directory}" OR NOT IS_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/${directory}")
        message("(${CMAKE_CURRENT_LIST_DIR}/${location}) does not exist and will be ignored.")
        return()
    endif()

    set(location "${CMAKE_CURRENT_LIST_DIR}/${directory}")

    string(TOUPPER ${type}_${name} option)

    # optional parameter to activate/desactivate the option
    set(active OFF)
    if(${ARGV3})
        set(active ON)
    endif()

    if("${type}" STREQUAL "Subdirectory")
        string(TOUPPER MODULE_${PROJECT_NAME} module_name)
        string(TOUPPER plugin_${PROJECT_NAME} plugin_name)
        string(TOUPPER ${name} uppername)
        if(DEFINED ${module_name})
            set(option ${module_name}_FETCH_${uppername})
        elseif(DEFINED ${plugin_name})
            set(option ${plugin_name}_FETCH_${uppername})
        else()
            set(option SOFA_FETCH_${uppername})
        endif()
        option(${option} "Fetch ${name} repository." ${active})
    else()
        option(${option} "Fetch and build the ${name} ${type}." ${active})
    endif()

    # Setup temporary directory
    set(${name}_TEMP_DIR "${CMAKE_BINARY_DIR}/external_directories/fetched/${name}/" )

    # Fetch
    if(${option})
        message("Fetching ${type} ${name}")

        message("Checking for ${${name}_TEMP_DIR}")
        if(NOT EXISTS ${${name}_TEMP_DIR})
            message("Creating ${${name}_TEMP_DIR}")
            file(MAKE_DIRECTORY ${${name}_TEMP_DIR})
        endif()

        # Download and unpack  at configure time
        configure_file(${location}/ExternalProjectConfig.cmake.in ${${name}_TEMP_DIR}/CMakeLists.txt)
        file(COPY ${location}/ExternalProjectConfig.cmake.in DESTINATION ${${name}_TEMP_DIR})

        #execute script to get src
        message("Pulling ${name}... ")
        execute_process(COMMAND "${CMAKE_COMMAND}" -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER} -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER} -G "${CMAKE_GENERATOR}" .
            WORKING_DIRECTORY "${${name}_TEMP_DIR}/" )
        execute_process(COMMAND "${CMAKE_COMMAND}" --build .
            WORKING_DIRECTORY  "${${name}_TEMP_DIR}/" )

        if(EXISTS "${location}/.git")
            message("... Done")
            # add .gitignore for Sofa
            file(WRITE "${location}/.gitignore" "*")
            file(COPY ${${name}_TEMP_DIR}/ExternalProjectConfig.cmake.in DESTINATION ${location})
        else()
            message("... error while pulling ${name}")
        endif()
    endif()

    # Add
    if(EXISTS "${location}/.git" AND IS_DIRECTORY "${location}/.git")
        configure_file(${location}/ExternalProjectConfig.cmake.in ${${name}_TEMP_DIR}/CMakeLists.txt)
        if("${type}" STREQUAL "Subdirectory")
            add_subdirectory("${location}" "${name}")
        elseif("${type}" STREQUAL "Plugin")
            sofa_add_plugin("${name}" "${name}" ${active})
        endif()
    endif()
endmacro()

macro(sofa_add_subdirectory_external directory name)
    sofa_add_generic_external(${directory} ${name} "Subdirectory" ${ARGV2})
endmacro()

macro(sofa_add_plugin_external directory name)
    sofa_add_generic_external(${directory} ${name} "Plugin" ${ARGV2})
endmacro()



# Declare a (unique, TODO?) directory containing the python scripts of
# a plugin.  This macro:
# - creates rules to install all the .py scripts in ${directory} to
#   lib/python2.7/site-packages/${plugin_name}
# - creates a etc/sofa/python.d/${plugin_name} file in the build tree
#   pointing to the source tree
# - creates a etc/sofa/python.d/${plugin_name} file in the install
#   tree, containing a relative path to the installed script directory
#
# Assumes relative path.
macro(sofa_set_python_directory plugin_name directory)
    ## Install python scripts, preserving the file tree
    file(GLOB_RECURSE PYTHON_FILES "${CMAKE_CURRENT_SOURCE_DIR}/${directory}/*.py")
    file(GLOB_RECURSE JSON_FILES   "${CMAKE_CURRENT_SOURCE_DIR}/${directory}/*.json")
    LIST(APPEND ALL_FILES ${PYTHON_FILES} ${JSON_FILES})
    foreach(python_file ${ALL_FILES})
        file(RELATIVE_PATH script "${CMAKE_CURRENT_SOURCE_DIR}/${directory}" "${python_file}")
        get_filename_component(path ${script} DIRECTORY)
        install(FILES ${directory}/${script}
                DESTINATION "lib/python2.7/site-packages/${path}"
                COMPONENT headers)
    endforeach()

    ## Python configuration file (build tree)
    file(WRITE "${CMAKE_BINARY_DIR}/etc/sofa/python.d/${plugin_name}"
         "${CMAKE_CURRENT_SOURCE_DIR}/${directory}")
    ## Python configuration file (install tree)
     file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/installed-SofaPython-config"
         "lib/python2.7/site-packages")
     install(FILES "${CMAKE_CURRENT_BINARY_DIR}/installed-SofaPython-config"
             DESTINATION "etc/sofa/python.d"
             RENAME "${plugin_name}"
             COMPONENT headers)
endmacro()



##########################################################
#################### INSTALL MACROS ######################
##########################################################


macro(sofa_install_targets package_name the_targets install_include_subdir)
    install(TARGETS ${the_targets}
            EXPORT ${package_name}Targets
            RUNTIME DESTINATION bin COMPONENT applications
            LIBRARY DESTINATION lib COMPONENT libraries
            ARCHIVE DESTINATION lib COMPONENT libraries
            PUBLIC_HEADER DESTINATION include/${install_include_subdir} COMPONENT headers)

    if(NOT "${install_include_subdir}" STREQUAL "") # Handle multi-dir install (no PUBLIC_HEADER)
        foreach(target ${the_targets})
            get_target_property(public_header ${target} PUBLIC_HEADER)
            if("${public_header}" STREQUAL "public_header-NOTFOUND")
                #message("Full install (no PUBLIC_HEADER): ${CMAKE_CURRENT_SOURCE_DIR}")
                # the trailing slash is IMPORTANT, see https://cmake.org/pipermail/cmake/2009-December/033850.html
                install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
                        DESTINATION include/${install_include_subdir}
                        COMPONENT headers
                        FILES_MATCHING PATTERN "*.h" PATTERN "*.inl")
            endif()
        endforeach()
    endif()

    ## Install rules for the resources
    get_filename_component(PARENT_DIR ${CMAKE_CURRENT_SOURCE_DIR} DIRECTORY)
    get_filename_component(PARENT_DIR_NAME ${PARENT_DIR} NAME)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/examples/")
        install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/examples/ DESTINATION share/sofa/${PARENT_DIR_NAME}/${PROJECT_NAME} COMPONENT resources)
    endif()
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/scenes/")
        install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/scenes/ DESTINATION share/sofa/${PARENT_DIR_NAME}/${PROJECT_NAME} COMPONENT resources)
    endif()
endmacro()


# sofa_write_package_config_files(Foo <version> <build-include-dirs>)
#
# Create CMake package configuration files
# - In the build tree:
#   - ${CMAKE_BINARY_DIR}/cmake/FooConfig.cmake
#   - ${CMAKE_BINARY_DIR}/cmake/FooConfigVersion.cmake
# - In the install tree:
#   - lib/cmake/Foo/FooConfigVersion.cmake
#   - lib/cmake/Foo/FooConfig.cmake
#   - lib/cmake/Foo/FooTargets.cmake
#
# This macro factorizes boilerplate CMake code for the different
# packages in Sofa.  It assumes that there is a FooConfig.cmake.in
# file template in the same directory.  For example, if a package Foo
# depends on Bar and Baz, and creates the targets Foo and Qux, here is
# a typical FooConfig.cmake.in:
#
# @PACKAGE_INIT@
#
# find_package(Bar REQUIRED)
# find_package(Baz REQUIRED)
#
# if(NOT TARGET Qux)
# 	include("${CMAKE_CURRENT_LIST_DIR}/FooTargets.cmake")
# endif()
#
# check_required_components(Foo Qux)
macro(sofa_write_package_config_files package_name version)

    ## <package_name>Targets.cmake
    install(EXPORT ${package_name}Targets DESTINATION lib/cmake/${package_name} COMPONENT headers)

    ## <package_name>ConfigVersion.cmake
    set(filename ${package_name}ConfigVersion.cmake)
    write_basic_package_version_file(${filename} VERSION ${version} COMPATIBILITY ExactVersion)
    configure_file("${CMAKE_CURRENT_BINARY_DIR}/${filename}"
                   "${CMAKE_BINARY_DIR}/cmake/${filename}" COPYONLY)
    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${filename}" DESTINATION lib/cmake/${package_name} COMPONENT headers)

    ### <package_name>Config.cmake
    configure_package_config_file(${package_name}Config.cmake.in
                                  "${CMAKE_BINARY_DIR}/cmake/${package_name}Config.cmake"
                                  INSTALL_DESTINATION lib/cmake/${package_name})
    install(FILES "${CMAKE_BINARY_DIR}/cmake/${package_name}Config.cmake"
            DESTINATION lib/cmake/${package_name} COMPONENT headers)

endmacro()


macro(sofa_create_package package_name version the_targets include_subdir)
    sofa_install_targets("${package_name}" "${the_targets}" "${include_subdir}")
    sofa_write_package_config_files("${package_name}" "${version}")
endmacro()



# Get path of all library versions (involving symbolic links) for a specified library
macro(sofa_install_libraries)
    foreach(library ${ARGN})
        if(EXISTS ${library})
            get_filename_component(LIBREAL ${library} REALPATH)
            get_filename_component(LIBREAL_NAME ${LIBREAL} NAME_WE)
            get_filename_component(LIBREAL_PATH ${LIBREAL} PATH)

            file(GLOB_RECURSE SHARED_LIBS
                "${LIBREAL_PATH}/${LIBREAL_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX}*"
                "${LIBREAL_PATH}/${LIBREAL_NAME}[0-9]${CMAKE_SHARED_LIBRARY_SUFFIX}*"
                "${LIBREAL_PATH}/${LIBREAL_NAME}[0-9][0-9]${CMAKE_SHARED_LIBRARY_SUFFIX}*"
            )
            file(GLOB_RECURSE STATIC_LIBS
                "${LIBREAL_PATH}/${LIBREAL_NAME}${CMAKE_STATIC_LIBRARY_SUFFIX}*"
                "${LIBREAL_PATH}/${LIBREAL_NAME}[0-9]${CMAKE_STATIC_LIBRARY_SUFFIX}*"
                "${LIBREAL_PATH}/${LIBREAL_NAME}[0-9][0-9]${CMAKE_STATIC_LIBRARY_SUFFIX}*"
            )

            if(WIN32)
                install(FILES ${SHARED_LIBS} DESTINATION bin COMPONENT applications)
            else()
                install(FILES ${SHARED_LIBS} DESTINATION lib COMPONENT applications)
            endif()
            install(FILES ${STATIC_LIBS} DESTINATION lib COMPONENT libraries)
        endif()
    endforeach()
endmacro()

macro(sofa_install_get_libraries library)
    message(WARNING "sofa_install_get_libraries() is deprecated. Please use sofa_install_libraries() instead.")
    sofa_install_libraries(${library})
endmacro()


macro(sofa_install_libraries_from_targets)
    foreach(target ${ARGN})
        get_target_property(target_location ${target} LOCATION_${CMAKE_BUILD_TYPE})
        sofa_install_libraries(${target_location})
    endforeach()
endmacro()


macro(sofa_copy_libraries)
    foreach(library ${ARGN})
        if(EXISTS ${library})
            get_filename_component(LIB_NAME ${library} NAME_WE)
            get_filename_component(LIB_PATH ${library} PATH)

            file(GLOB SHARED_LIB
                "${LIB_PATH}/${LIB_NAME}${CMAKE_SHARED_LIBRARY_SUFFIX}"
                "${LIB_PATH}/${LIB_NAME}[0-9]${CMAKE_SHARED_LIBRARY_SUFFIX}"
                "${LIB_PATH}/${LIB_NAME}[0-9][0-9]${CMAKE_SHARED_LIBRARY_SUFFIX}")

            if(CMAKE_CONFIGURATION_TYPES) # Multi-config generator (MSVC)
                foreach(CONFIG ${CMAKE_CONFIGURATION_TYPES})
                    file(COPY ${SHARED_LIB} DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CONFIG}")
                endforeach()
            else()                      # Single-config generator (nmake)
                file(COPY ${SHARED_LIB} DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
            endif()
        endif()
    endforeach()
endmacro()


macro(sofa_copy_libraries_from_targets)
    foreach(target ${ARGN})
        if(CMAKE_CONFIGURATION_TYPES) # Multi-config generator (MSVC)
            foreach(CONFIG ${CMAKE_CONFIGURATION_TYPES})
                get_target_property(target_location ${target} LOCATION_${CONFIG})
                sofa_copy_libraries(${target_location})
            endforeach()
        else() # Single-config generator (nmake)
            get_target_property(target_location ${target} LOCATION_${CMAKE_BUILD_TYPE})
            sofa_copy_libraries(${target_location})
        endif()
    endforeach()
endmacro()



## to store which sources have been used for installed binaries
## these should be internal files and not delivered, but this is definitively useful
## when storing backups / demos across several repositories (e.g. sofa + plugins)
macro( sofa_install_git_version name sourcedir )
INSTALL( CODE
"
    find_package(Git REQUIRED)

    # the current commit hash should be enough
    # except if the git history changes...
    # so adding more stuff to be sure

    # get the current working branch
    execute_process(
      COMMAND \${GIT_EXECUTABLE} rev-parse --abbrev-ref HEAD
      WORKING_DIRECTORY ${sourcedir}
      OUTPUT_VARIABLE SOFA_GIT_BRANCH
      OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    # get the current commit info (hash, author, date, comment)
    execute_process(
      COMMAND \${GIT_EXECUTABLE} log --format=medium -n 1
      WORKING_DIRECTORY ${sourcedir}
      OUTPUT_VARIABLE SOFA_GIT_INFO
      OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    string( TOLOWER \"${name}\" name_lower )
    if( name_lower STREQUAL \"sofa\" )
        file(WRITE  \"${CMAKE_INSTALL_PREFIX}/git.version\" \"######## ${name} ########\nBranch: \${SOFA_GIT_BRANCH}\n\${SOFA_GIT_INFO}\n############\n\n\" )
    else()
        file(APPEND \"${CMAKE_INSTALL_PREFIX}/git.version\" \"######## ${name} ########\nBranch: \${SOFA_GIT_BRANCH}\n\${SOFA_GIT_INFO}\n############\n\n\" )
    endif()
"
)
endmacro()
