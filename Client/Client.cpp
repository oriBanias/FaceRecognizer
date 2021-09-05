#include "Client.h"
#include <iostream>

Client::Client() : MessageHandler(SERVER_PORT)
{
}

Client::~Client()
{
}

void Client::HandleIncomingMsg(const void* pMsg)
{
	if (pMsg != nullptr)
	{
		const MsgHeader* pHeader = reinterpret_cast<const MsgHeader*>(pMsg);
		switch (pHeader->opcode)
		{
			case EMsgOpcode::SEREVER2CLIENT_ADD_PERSON_RESPONSE_OPCODE:
			{
				std::cout << "SEREVER2CLIENT_ADD_PERSON_RESPONSE_OPCODE messag arrived\n";
				const Server2ClientAddPersonResponseMsg* pResponseMsg = reinterpret_cast<const Server2ClientAddPersonResponseMsg*>(pMsg);
				std::cout << "STATUS: " << pResponseMsg->addPersonStatus << std::endl;
				break;
			}

			case EMsgOpcode::SEREVER2CLIENT_FIND_PERSON_RESPONSE_OPCODE:
			{
				std::cout << "SEREVER2CLIENT_FIND_PERSON_RESPONSE_OPCODE messag arrived\n";
				const Server2ClientFindPersonResponseMsg* pResponseMsg = reinterpret_cast<const Server2ClientFindPersonResponseMsg*>(pMsg);
				std::cout << "Found persons:" << std::endl;
				for (int i = 0; i < MAX_PERSONS; i++)
				{
					if (strlen(pResponseMsg->personName[i]) > 0)
					{
						std::cout << "Person #" << i << ": " << pResponseMsg->personName[i] << std::endl;
					}
				}
				break;
			}

			default:
				std::cout << "wrong opcode\n";
				break;
		}
	}
}

void Client::SendAddPersonRequestMsg(int features[FEATURES_SIZE], char personName[PERSON_NAME])
{
	static const int msgLength = sizeof(Client2ServerAddPersonRequestMsg);
	static Client2ServerAddPersonRequestMsg pResponseMsg;
	pResponseMsg.header.opcode = EMsgOpcode::CLIENT2SERVER_ADD_PERSON_REQUEST_OPCODE;
	pResponseMsg.header.length = msgLength;
	(void)memcpy(pResponseMsg.features, features, FEATURES_SIZE * sizeof(int));
	(void)memcpy(pResponseMsg.personName, personName, PERSON_NAME * sizeof(char));
	SendMsg(&pResponseMsg, msgLength);
}

void Client::SendFindPersonRequestMsg(int features[FEATURES_SIZE], int numOfMatches)
{
	static const int msgLength = sizeof(Client2ServerFindPersonRequestMsg);
	static Client2ServerFindPersonRequestMsg pResponseMsg;
	pResponseMsg.header.opcode = EMsgOpcode::CLIENT2SERVER_FIND_PERSON_REQUEST_OPCODE;
	pResponseMsg.header.length = msgLength;
	(void)memcpy(pResponseMsg.features, features, FEATURES_SIZE * sizeof(int));
	pResponseMsg.numOfMatches = numOfMatches;
	SendMsg(&pResponseMsg, msgLength);
}
