cmake_minimum_required(VERSION 3.16)
project(star32 VERSION 0.0.1 LANGUAGES C)

set(CMAKE_DISABLE_IN_SOURCE_BUILD ON CACHE BOOL "Prevents cmake -S. -B.")
set(CMAKE_DISABLE_SOURCE_CHANGES ON CACHE BOOL "Prevent writing files to CMAKE_SOURCE_DIR under configure")

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)
set(CMAKE_POLICY_DEFAULT_CMP0075 NEW)

set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_FIND_FRAMEWORK FIRST)

include(CMakeDependentOption)
include(GenerateExportHeader)
include(FetchContent)

# Include astera (game engine backend)
add_subdirectory("${PROJECT_SOURCE_DIR}/dep/astera")

# If to build the `examples/` folder
cmake_dependent_option(STAR_BUILD_EXAMPLES
  "Build star32's examples" ON
  "CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME" ON)

# Build out the utility tools
cmake_dependent_option(STAR_BUILD_TOOLS
  "Build star32's tools" ON
  "CMAKE_BUILD_TYPE STREQUAL Debug" ON)

cmake_dependent_option(STAR_DEBUG_OUTPUT
  "Enable star32's internal debug output" ON
  "CMAKE_BUILD_TYPE STREQUAL Debug" ON)

# Enables ASAN & Pedantic output
option(STAR_DEBUG_ENGINE OFF)


# Get all of our source files into a list
file(GLOB sources CONFIGURE_DEPENDS ${PROJECT_SOURCE_DIR}/src/*.c)
add_library(${PROJECT_NAME} STATIC)

# Add our source files & nanovg
target_sources(${PROJECT_NAME} PRIVATE ${sources})

# Include headers
target_include_directories(${PROJECT_NAME}
  PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/dep/astera>
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INCLUDE_INSTALLDIR}>)

# Add all the preprocessor definitions for headers
target_compile_definitions(${PROJECT_NAME}
  PRIVATE
    $<$<BOOL:${STAR_DEBUG_ENGINE}>:STAR_DEBUG_OUTPUT>
    $<$<BOOL:${STAR_DEBUG_OUTPUT}>:STAR_DEBUG_OUTPUT>
    $<$<PLATFORM_ID:FreeBSD>:FreeBSD>
    $<$<PLATFORM_ID:OpenBSD>:OpenBSD>
    $<$<PLATFORM_ID:NetBSD>:NetBSD>
    $<$<PLATFORM_ID:Darwin>:OSX>
    PUBLIC
    $<$<BOOL:${STAR_PAK_WRITE}>:STAR_PAK_WRITE>)

# Set the C Standard to C99
set_property(TARGET ${PROJECT_NAME} PROPERTY C_STANDARD 99)

# Add warning output for the different compilers
target_compile_options(${PROJECT_NAME}
  PRIVATE
  $<$<NOT:$<C_COMPILER_ID:MSVC>>:-Wall -Wextra -pedantic>
  $<$<C_COMPILER_ID:MSVC>:/W4>)

# Enables ASAN & Pedantics
if(STAR_DEBUG_ENGINE)
  target_compile_options(${PROJECT_NAME}
    PUBLIC
    $<$<C_COMPILER_ID:MSVC>:/fsanitize=address>
    $<$<NOT:$<C_COMPILER_ID:MSVC>>: -g -fsanitize=address -fno-omit-frame-pointer -O2 -g>)

  target_link_options(${PROJECT_NAME}
    PUBLIC
    $<$<C_COMPILER_ID:MSVC>:/fsanitize=address>
    $<$<NOT:$<C_COMPILER_ID:MSVC>>:-g -fsanitize=address>)
endif()

# Link against the various libraries we need
target_link_libraries(${PROJECT_NAME}
  PUBLIC
  astera)

set_property(TARGET ${PROJECT_NAME} PROPERTY C_VISIBILITY_PRESET "hidden")
set_property(TARGET ${PROJECT_NAME} PROPERTY VERSION ${PROJECT_VERSION})

install(DIRECTORY include/ "${PROJECT_BINARY_DIR}/include/"
  TYPE INCLUDE
  FILES_MATCHING PATTERN "*.h")

if(STAR_BUILD_EXAMPLES)
  if(EXISTS "${PROJECT_SOURCE_DIR}/examples")
    add_subdirectory("${PROJECT_SOURCE_DIR}/examples")
  else()
    message(WARNING "Unable to find examples directory, disabling STAR_BUILD_EXAMPLES")
    set(STAR_BUILD_EXAMPLES OFF)
  endif()
endif()

if(STAR_BUILD_TOOLS)
  if(EXISTS "${PROJECT_SOURCE_DIR}/tools")
    add_subdirectory("${PROJECT_SOURCE_DIR}/tools")
  else()
    message(WARNING "Unable to find tools directory, disabling STAR_BUILD_TOOLS")
    set(STAR_BUILD_TOOLS OFF)
  endif()
endif()

