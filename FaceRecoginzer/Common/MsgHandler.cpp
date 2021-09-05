#include "MsgHandler.h"
#include "ICD.h"
#include <ws2tcpip.h>
#include <iostream>

#pragma comment(lib,"ws2_32.lib")


MessageHandler::MessageHandler(unsigned short port)
    : m_socket(0),
        m_port(port),
        m_rcvBuffer()
{
    (void)memset(&m_rcvBuffer, 0, sizeof(m_rcvBuffer));
}

MessageHandler::~MessageHandler()
{

}

bool MessageHandler::Init(EPlatform platform)
{
    bool initSuccess = true;
    WSADATA wsaData;
    struct sockaddr_in address;

    // Initialize Winsock
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
        printf("WSAStartup failed: %d\n", iResult);
        initSuccess = false;
    }

    if (platform == EPlatform::SERVER)
    {
        int opt = 1;
        int addrlen = sizeof(address);
        SOCKET msgSocket;

        if (initSuccess == true)
        {
            msgSocket = socket(AF_INET, SOCK_STREAM, 0);//Protocol, socket type,
            if (SOCKET_ERROR == msgSocket)
            {
                printf("Socket() error:%d", WSAGetLastError());
                initSuccess = false;
            }
        }

        if (initSuccess == true)
        {
            if (setsockopt(msgSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEADDR,
                (const char*)&opt, sizeof(opt)))
            {
                std::cout << "socket failed" << std::endl;
                initSuccess = false;
            }
            else
            {
                address.sin_family = AF_INET;
                address.sin_addr.s_addr = INADDR_ANY;
                address.sin_port = htons(m_port);
            }
        }

        if (initSuccess == true)
        {
            if (bind(msgSocket, (struct sockaddr*)&address,
                sizeof(address)) < 0)
            {
                std::cout << "bind failed" << std::endl;
                initSuccess = false;
            }
        }

        if (initSuccess == true)
        {
            if (listen(msgSocket, 3) < 0)
            {
                std::cout << "listen" << std::endl;
                initSuccess = false;
            }
        }

        if (initSuccess == true)
        {
            if ((m_socket = accept(msgSocket, (struct sockaddr*)&address,
                (socklen_t*)&addrlen)) < 0)
            {
                std::cout << "accept" << std::endl;
                initSuccess = false;
            }
        }

    }
    else // EPlatform::CLIENT
    {
        if ((m_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        {

            std::cout << "Socket creation error" << std::endl;
            initSuccess = false;
        }

        address.sin_family = AF_INET;
        address.sin_port = htons(SERVER_PORT);

        if (inet_pton(AF_INET, "127.0.0.1", &address.sin_addr) <= 0) //LOCAL
        {
            std::cout << "Address is invalid ... " << std::endl;
            initSuccess = false;
        }

        if (connect(m_socket, (struct sockaddr*)&address, sizeof(address)) < 0)
        {
            std::cout << "Connection Failed. Try again! ..." << std::endl;
            initSuccess = false;
        }
    }

    return initSuccess;
}

void MessageHandler::HandleRcvMsgs()
{
    std::cout << "waiting for incoming messages\n";
    (void)memset(&m_rcvBuffer, 0, sizeof(m_rcvBuffer));

    if (recv(m_socket, m_rcvBuffer, MAX_RCV_MSG_BUFFER, 0) > 0)
    {
        HandleIncomingMsg(reinterpret_cast<const void*>(&m_rcvBuffer));
    }
}

bool MessageHandler::SendMsg(const void* pMsg, int msgLength)
{
    return (send(m_socket, reinterpret_cast<const char*>(pMsg), msgLength, 0) > 0);
}

