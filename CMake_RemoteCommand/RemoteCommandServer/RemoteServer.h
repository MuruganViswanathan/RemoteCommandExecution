// RemoteServer.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>

// TODO: Reference additional headers your program requires here.

// For remote command
#include <string>
#include <map>
#include <ctime>
#include <iostream>
#include <fstream>


class UShell 
{
protected:

    // Added for remote execution
    bool mExitRequested = false;

public:
    void Run();

    // Added for remote execution    
    void RequestExit() { mExitRequested = true; }
    std::string ExecuteCommandFromRemote(const std::string& input);
    
};



// // ==============================
// // [REMOTECOMMAND] Basic Logger 

class FileLogger
{
public:
    // Constructor to open the log file
    FileLogger()
    {
#ifdef WIN32
        std::string path = "D:\\AppDataA20FDA\\Log\\RemoteCommandLog.txt";
#endif

#ifdef __linux__
        std::string path = "/mrcs/d/AppDataA20FDA/Log/RemoteCommandLog.txt";
#endif

        // Open the log file in append mode
        logFile.open(path, std::ios::app);
        if (!logFile.is_open())
        {
            std::cerr << "[REMOTECOMMAND] Failed to open log file!!" << std::endl;
        }
    }

    // Method to log a message (multiple times)
    void log(const std::string& message)
    {
        if (logFile.is_open())
        {
            logFile << message << std::endl;
        }
        else
        {
            std::cerr << "[REMOTECOMMAND] Log file is not open!" << std::endl;
        }
    }

    // Destructor to close the file
    ~FileLogger()
    {
        if (logFile.is_open())
        {
            logFile.close();
        }
    }

    FileLogger(const FileLogger&) = delete;
    FileLogger& operator=(const FileLogger&) = delete;


private:
    std::ofstream logFile;
};

// [REMOTECOMMAND] End
// ==============================


