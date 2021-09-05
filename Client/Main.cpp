#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <random>
#include <limits>
#include <chrono>
#include <thread>

#include <ws2tcpip.h>
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")

#include "ICD.h"
#include "Client.h"

int main(int argc, char* argv[])
{
    Client client;

    static int cycleNumber = 1;

    int numOfArgs = argc;

    bool res = client.Init();
    if (res == true)
    {
        int features[FEATURES_SIZE] = { 0 };

        if (strcmp(argv[1], "1") == 0) // Add person
        {
            for (int i = 2; i < argc; ++i)
            {
                srand(time(0));
                int random = (rand() % FEATURES_SIZE - 1);
                for (int j = 0; j < FEATURES_SIZE; ++j)
                {
                    features[j] = (rand() % FEATURES_SIZE);
                }

                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                client.SendAddPersonRequestMsg(features, argv[i]);
            }
        }

        if (strcmp(argv[1], "2") == 0) // Find person
        {
            int numOfRequestedMatches = atoi(argv[2]);

            srand(time(0));
            int random = (rand() % FEATURES_SIZE - 1);
            for (int j = 0; j < FEATURES_SIZE; ++j)
            {
                features[j] = (rand() % FEATURES_SIZE);
            }

            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            client.SendFindPersonRequestMsg(features, numOfRequestedMatches);
            client.HandleRcvMsgs();

        }
    }
    else
    {
        std::cout << "init failed\n";
    }

    return 0;
}

/*
int main(int argc, char* argv[])
{
    Client client;

    static int cycleNumber = 1;

    bool res = client.Init();
    if (res == true)
    {
        while (true)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));


            // Add person request
            int features[FEATURES_SIZE] = { 0 };
            char personName[PERSON_NAME] = "ori";
            client.SendAddPersonRequestMsg(features, personName);

            // Find person request
            if (cycleNumber % 10 == 0)
            {
                client.SendFindPersonRequestMsg(features, 3);
                client.HandleRcvMsgs();

            }

            cycleNumber++;
        }
    }
    else
    {
        std::cout << "init failed\n";
    }

    return 0;
}
*/
