# Install script for directory: F:/DungeonCraft/Server

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
  include("F:/DungeonCraft/Build/modules/mod-spell-regulator-master/cmake_install.cmake")
  include("F:/DungeonCraft/Build/modules/mod-pvp-titles-master/cmake_install.cmake")
  include("F:/DungeonCraft/Build/modules/mod-npc-talent-template-master/cmake_install.cmake")
  include("F:/DungeonCraft/Build/modules/mod-npc-free-professions-master/cmake_install.cmake")
  include("F:/DungeonCraft/Build/modules/mod-npc-buffer-master/cmake_install.cmake")
  include("F:/DungeonCraft/Build/modules/mod-learn-highest-talent-master/cmake_install.cmake")
  include("F:/DungeonCraft/Build/modules/mod-cfbg-master/cmake_install.cmake")
  include("F:/DungeonCraft/Build/modules/mod-ah-bot-master/cmake_install.cmake")
  include("F:/DungeonCraft/Build/deps/cmake_install.cmake")
  include("F:/DungeonCraft/Build/src/common/cmake_install.cmake")
  include("F:/DungeonCraft/Build/src/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "F:/DungeonCraft/Build/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
