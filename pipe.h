#ifndef PIPE_H
#define PIPE_H

#include <string>
#include <sstream>
#include <exception>
#include <stdexcept>
#include <vector>
#include <windows.h>

template <typename T>
class Pipe
{

protected:
    HANDLE mHandle;
    std::string mName;
    const static std::string mPrefix;

    void throwError(const std::string &what)
    {
        std::string exc = what + " " + lastError();
        throw std::runtime_error(exc);
    }

public:
    Pipe(const std::string &name):mName(name)
    {
        
    }
    
    std::string name()
    {
        return mName;
    }

    unsigned write(T *buffer, unsigned bytesToBeWritten)
    {
        long unsigned bytesWritten;
        if (!WriteFile(mHandle, static_cast<LPVOID>(buffer), bytesToBeWritten, &bytesWritten, NULL))
        {
            throwError("Data writing error!");
        }
        return bytesWritten;
    }

    unsigned write(const std::vector<T> &vector)
    {
        if (!WriteFile(mHandle, static_cast<LPVOID>(vector.data()), vector.data()*sizeof(T), &bytesWritten, NULL))
        return write(vector.data(), vector.size()*sizeof(T));
    }

    template <typename U>
    unsigned write(U variable)
    {
        return write(static_cast<T*>(&variable), sizeof(U));
    }

    unsigned read(T *buffer, unsigned bufferSizeInBytes)
    {
        long unsigned bytesRead;
        if (!ReadFile(mHandle, static_cast<LPVOID>(buffer), bufferSizeInBytes, &bytesRead, NULL))
        {
            throwError("Data reading error!");
        }
        return bytesRead;
    }

    std::vector<T> read(unsigned elementsToRead)
    {
        std::vector<T> vector(elementsToRead);
        long unsigned bytesRead;
        if (!ReadFile(mHandle, static_cast<LPVOID>(vector.data()), vector.size()*sizeof(T), &bytesRead, NULL))
        {
            throwError("Data reading error!");
        }

        vector.resize(bytesRead/sizeof(T));
        return vector;
    }

    unsigned peek(T *buffer, unsigned bufferSizeInBytes)
    {
        unsigned bytesRead;
        if (!PeekNamedPipe(mHandle, static_cast<LPVOID>(buffer), bufferSizeInBytes, &bytesRead, NULL ,NULL))
        {
            throwError("Data peeking error!");
        }
        return bytesRead;
    }

    std::vector<T> peek(unsigned elementsToRead)
    {
        std::vector<T> vector(elementsToRead);
        long unsigned bytesRead;
        if (!PeekNamedPipe(mHandle, static_cast<LPVOID>(vector.data()), vector.size()*sizeof(T), &bytesRead, NULL, NULL))
        {
            throwError("Data peeking error!");
        }

        vector.resize(bytesRead/sizeof(T));
        return vector;
    }

    unsigned bytesAvailable()
    {
        long unsigned bytes;
        if (!PeekNamedPipe(mHandle, NULL, 0, 0, &bytes ,NULL))
        {
            throwError("Data peeking error!");
        }
        return bytes;
    }

    std::string lastError()
    {

        DWORD   dwLastError = GetLastError();
        char   lpBuffer[1024] = "?";
            FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM,                 // It´s a system error
                             NULL,                                      // No string to be formatted needed
                             dwLastError,                               // Hey Windows: Please explain this error!
                             MAKELANGID(LANG_NEUTRAL,SUBLANG_DEFAULT),  // Do it in the standard language

                             lpBuffer,              // Put the message here
                             sizeof(lpBuffer)-1,                     // Number of bytes to store the message
                             NULL);

        return std::string(lpBuffer);
    }

    std::string path()
    {
        return mPrefix+mName;
    }

    bool close()
    {
        return CloseHandle(mHandle);
    }

    virtual ~Pipe()
    {
        close();
    }
};

template <typename T>
const std::string Pipe<T>::mPrefix = "\\\\.\\pipe\\";

#endif // PIPE_H
