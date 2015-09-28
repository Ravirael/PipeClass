#include <iostream>
#include <thread>
#include "pipeserver.h"
#include "pipeclient.h"

using namespace std;

void serverProcess(PipeServer<int> &pipe)
{
    int array[100];
    int k = 0;


    pipe.open(2);
    pipe.waitForClient();
    for (int i = 0; i < 100; ++i)
        pipe.write(i);
    //std::cout << pipe.write(array, sizeof(array)) << " bytes written!\n";
    std::cout<< "Writing complete!";
}

void clientProcess(PipeClient<int> &pipe)
{
    int array[100] = {0};
    std::vector<int> data;

    while (!pipe.waitForServer(5000));
    pipe.connect();

   //std::cout << "Bytes available: " << pipe.bytesAvailable() << std::endl;

    while (pipe.bytesAvailable() < 400)
    {
        std::cout << "Waiting!" << std::endl;
    }

    //data = pipe.read(100);
    std::cout << (data = pipe.read(100)).size()*sizeof(int) << " bytes read!\n";

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

