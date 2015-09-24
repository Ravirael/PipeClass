#ifndef PIPECLIENT_H
#define PIPECLIENT_H

#include "pipe.h"

template <typename T>
class PipeClient : public Pipe<T>
{
public:
    PipeClient(const std::string &name):Pipe<T>(name)
    {

    }

    bool waitForServer(unsigned timeout)
    {
        return WaitNamedPipeA(this->path().c_str(), timeout);
    }

    bool connect()
    {
        this->mHandle = CreateFileA(this->path().c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

        return (this->mHandle != INVALID_HANDLE_VALUE);
    }
};

#endif // PIPECLIENT_H
