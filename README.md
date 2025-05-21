BUILD Instructions:

**Build CMake_RemoteCommand:**
=================================

Windows (clean build):
----------------------
Start Menu → "x64 Native Tools Command Prompt for VS 2019" (or 2022, depending on your installation).

1) Navigate to project directory:
cd CMake_RemoteCommand\RemoteServer\

2) create build dir: 
mkdir build_windows
cd build_windows

3) Run CMake to generate Visual Studio project files:
cmake .. -G "Visual Studio 16 2019"  # Replace with your VS version if different

--------
e.g.
E:\Work\Proj_RemoteCommand\CMake_RemoteCommand\RemoteServer\build_windows>cmake .. -G "Visual Studio 16 2019"
-- Selecting Windows SDK version 10.0.19041.0 to target Windows 10.0.22631.
-- The C compiler identification is MSVC 19.29.30154.0
-- The CXX compiler identification is MSVC 19.29.30154.0
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: C:/Program Files (x86)/Microsoft Visual Studio/2019/Professional/VC/Tools/MSVC/14.29.30133/bin/Hostx64/x64/cl.exe - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Detecting CXX compiler ABI info
-- Detecting CXX compiler ABI info - done
-- Check for working CXX compiler: C:/Program Files (x86)/Microsoft Visual Studio/2019/Professional/VC/Tools/MSVC/14.29.30133/bin/Hostx64/x64/cl.exe - skipped
-- Detecting CXX compile features
-- Detecting CXX compile features - done
-- Configuring done
-- Generating done
-- Build files have been written to: E:/Work/Proj_RemoteCommand/CMake_RemoteCommand/RemoteServer/build_windows
--------


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

=================================================================================================

RUN Instructions:

WINDOWS

1) Run Server First:
Go to RemoteServer\build_windows\Release\
Using command prompt run: .\RemoteServer.exe
This will start the server.

e.g.
E:\Work\Proj_RemoteCommand\CMake_RemoteCommand\RemoteServer\build_windows\Release>.\RemoteServer.exe
Hello RemoteServer
Press Enter to exit...

2) Run the Client:
Open a seperate command prompt and run the client.
Example client (prebuilt using Visual Studio) is located at RemoteCommandClient\Debug\

e.g.

(base) PS E:\Work\Proj_RemoteCommand\RemoteCommandClient\Debug> .\RemoteCommandClient.exe 127.0.0.1
Server Address: 10.160.8.54
Enter command (e.g., time, echo something, vmdbg command, exit):


Now you can send basic commands as shown to sent to the server and get it executed on the Server and get the results back to the client. Instead of the loopback IP 127.0.0.1 you should enter the Server IP address. Ping the IP first to check the connectivity and verify Firewall permissisions etc.

==============================================================================================

