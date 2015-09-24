#include <iostream>
#include <thread>
#include "pipeserver.h"
#include "pipeclient.h"

using namespace std;

void serverProcess(PipeServer<int> &pipe)
{
    int array[100];
    int k = 0;
    for (int &i : array)
        i = k++;

    pipe.open(2);
    pipe.waitForClient();
    std::cout << pipe.write(array, sizeof(array)) << " bytes written!\n";
}

void clientProcess(PipeClient<int> &pipe)
{
    int array[100] = {0};
    std::vector<int> data;

    while (!pipe.waitForServer(5000));
    pipe.connect();

    std::cout << "Bytes available: " << pipe.bytesAvailable() << std::endl;
    data = pipe.read(10);
    //std::cout << pipe.read(array, sizeof(array)) << " bytes read!\n";

    for (int i : data)
        std::cout << i << std::endl;
}


int main()
{
    PipeClient<int> client("testPipe");
    PipeServer<int> server("testPipe", 1024, 1024);

    std::thread clientThread([&](){clientProcess(client);});
    Sleep(1000);
    std::thread serverThread([&]()->void{serverProcess(server);});
    //serverProcess(server);
    //clientProcess(client);
    serverThread.join();
    clientThread.join();

    return 0;
}

