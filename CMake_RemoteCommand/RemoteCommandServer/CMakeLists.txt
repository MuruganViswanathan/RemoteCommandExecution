# CMakeList.txt : CMake project for RemoteServer, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("RemoteServer")

# Add source to this project's executable.
add_executable (RemoteServer "RemoteServer.cpp" "RemoteServer.h")

# TODO: Add tests and install targets if needed.
