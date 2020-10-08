# Install script for directory: F:/DungeonCraft/Server/deps

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/AzerothCore")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("F:/DungeonCraft/Build/deps/threads/cmake_install.cmake")
  include("F:/DungeonCraft/Build/deps/acelite/cmake_install.cmake")
  include("F:/DungeonCraft/Build/deps/fmt/cmake_install.cmake")
  include("F:/DungeonCraft/Build/deps/zlib/cmake_install.cmake")
  include("F:/DungeonCraft/Build/deps/g3dlite/cmake_install.cmake")
  include("F:/DungeonCraft/Build/deps/recastnavigation/cmake_install.cmake")
  include("F:/DungeonCraft/Build/deps/SFMT/cmake_install.cmake")
  include("F:/DungeonCraft/Build/deps/utf8cpp/cmake_install.cmake")
  include("F:/DungeonCraft/Build/deps/openssl/cmake_install.cmake")
  include("F:/DungeonCraft/Build/deps/mysql/cmake_install.cmake")
  include("F:/DungeonCraft/Build/deps/readline/cmake_install.cmake")
  include("F:/DungeonCraft/Build/deps/jemalloc/cmake_install.cmake")
  include("F:/DungeonCraft/Build/deps/gsoap/cmake_install.cmake")

endif()

