**Build CMake_RemoteCommand:**

Windows (clean build):
----------------------

1) Navigate to project directory:
cd CMake_RemoteCommand\RemoteServer\

2) create build dir: 
mkdir build
cd build

3) Run CMake to generate Visual Studio project files:
cmake .. -G "Visual Studio 17 2022"  # Replace with your VS version if different

4) Build the project:
cmake --build . --config Release

5) Executable will be in a path like:
.\Release\RemoteServer.exe

For rebuilding, ignore step 2, 3 and just build the project using step 4.


Linux (Ubuntu with GCC and Make):
---------------------------------
1) Install dependencies if not already:
sudo apt update
sudo apt install build-essential cmake

2) Go to project dir:
cd CMake_RemoteCommand\RemoteServer\

4) Create build dir (if already not present)
mkdir build
cd build

5) Run CMake to generate MakeFiles (first time):
cmake ..

6)  Build the project:
make
   
7) The executable (likely RemoteServer) will be in the build/ directory.
