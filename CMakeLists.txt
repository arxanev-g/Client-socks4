cmake_minimum_required(VERSION 3.14)
project(shad-cpp)

set(CMAKE_CXX_STANDARD             20)
set(CMAKE_MODULE_PATH              "${CMAKE_SOURCE_DIR}/tools/cmake")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_EXPORT_COMPILE_COMMANDS  ON)

find_package(Catch REQUIRED)
find_package(TBB REQUIRED)

find_package(PNG)
find_package(JPEG)
find_package(FFTW)

set(Clang_DIR "/usr/lib/llvm-16/lib/cmake/clang")
set(LLVM_DIR "/usr/lib/llvm-16/lib/cmake/llvm")
find_package(Clang CONFIG)

include_directories(tools/commons)
include(tools/cmake/TestSolution.cmake)
include(tools/cmake/BuildFlags.cmake)

function(add_if_exists name)
  if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${name})
    add_subdirectory(${name})
  endif()
endfunction()


if (UNIX AND NOT APPLE)
  include(tools/cmake/Protobuf.cmake)
  find_package(glog)

  if (Protobuf_FOUND AND glog_FOUND)
    message("Networking tasks are enabled")

    add_if_exists(tools/cactus)

    add_if_exists(socks4)
  else()
    message(WARNING "Networking tasks disabled. "
      "Protobuf:${Protobuf_FOUND} glog:${glog_FOUND}"
    )
  endif()

else()
  message(WARNING "For networking tasks only Linux is supported. "
    "See instructions for remote toolchain setup."
  )
endif()