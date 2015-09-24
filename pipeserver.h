#ifndef PIPESERVER_H
#define PIPESERVER_H

#include "pipe.h"

template <typename T>
class PipeServer : public Pipe<T>
{
    unsigned mInBufferSize, mOutBufferSize, mTimeout;
public:
    PipeServer(const std::string &name, unsigned inBufferSize, unsigned outBufferSize, unsigned timeout = 0):
        Pipe<T>(name), mInBufferSize(inBufferSize), mOutBufferSize(outBufferSize), mTimeout(timeout)
    {
        
    }

    bool waitForClient()
    {
        return ConnectNamedPipe(this->mHandle, NULL);

    }
    
    bool open(unsigned maxInstances = 255,
              unsigned openMode = PIPE_ACCESS_DUPLEX,
              unsigned pipeMode = PIPE_WAIT)
    {
        this->mHandle = CreateNamedPipeA(this->path().c_str(),
                                        openMode,
                                        pipeMode,
                                        maxInstances,
                                        mOutBufferSize,
                                        mInBufferSize,
                                        mTimeout,
                                        NULL);

        return (this->mHandle != INVALID_HANDLE_VALUE);

    }

    bool disconnectPipe()
    {
        return DisconnectNamedPipe(this->mHandle);
    }
};

#endif // PIPESERVER_H
