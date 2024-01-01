cmake_minimum_required(VERSION 3.1)
enable_language(CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_GENERATOR_PLATFORM x64)
if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  if (WIN32)
    set(CMAKE_INSTALL_PREFIX "C:/Program Files/Kamioda Games/RandomNumberGenerationServer" CACHE PATH "..." FORCE)
  else()
  set(CMAKE_INSTALL_PREFIX "/usr/local/RandomNumberGenerationServer" CACHE PATH "..." FORCE)
  endif()
endif()
if(CMAKE_SIZEOF_VOID_P EQUAL 4)
    message(FATAL_ERROR "This project cannot build for 32 bit. Please choose a configuration using 64 bit")
endif()
find_package(Threads REQUIRED)

project(RandGeneratorServer)
set(RandGeneratorServer_src
    Main.cpp
    Random.cpp
    Random.hpp
)

if(MSVC)
  # Force to always compile with W4
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  # Update if necessary
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-long-long -pedantic")
endif()
## Define the executable
add_executable(randgensrv ${RandGeneratorServer_src})
target_include_directories(randgensrv PUBLIC ${OpenCV_INCLUDE_DIRS} json/include)
if(WIN32)
  target_link_libraries(randgensrv Ws2_32)
endif()