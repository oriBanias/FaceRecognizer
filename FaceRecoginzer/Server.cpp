#define NOMINMAX

#include "Server.h"
#include <iostream>
#include <numeric>
#include <iterator>
#include <cmath>
#include <algorithm>
#include <array>
#include <random>



Server::Server() : MessageHandler(SERVER_PORT)
{
}

Server::~Server()
{
}

void Server::HandleIncomingMsg(const void* pMsg)
{
	if (pMsg != nullptr)
	{
		const MsgHeader* pHeader = reinterpret_cast<const MsgHeader*>(pMsg);
		switch (pHeader->opcode)
		{
			case EMsgOpcode::CLIENT2SERVER_ADD_PERSON_REQUEST_OPCODE:
            {
                std::cout << "CLIENT2SERVER_ADD_PERSON_REQUEST_OPCODE messag arrived\n";
                bool addPersonStatus = true;
                const Client2ServerAddPersonRequestMsg* pRequestMsg = reinterpret_cast<const Client2ServerAddPersonRequestMsg*>(pMsg);

                std::string personName = pRequestMsg->personName;
                if (!personName.empty())
                {
                    std::pair<std::vector<int>, std::string> person;
                    std::vector<int> features;
                    features.insert(features.begin(), std::begin(pRequestMsg->features), std::end(pRequestMsg->features));
                    person.first = features;
                    person.second = personName;

                    addPersonStatus = AddPerson(person);
                }
                else
                {
                    addPersonStatus = false;
                }
                SendAddPersonResponseMsg(addPersonStatus);
                if (addPersonStatus == true)
                {
                    std::cout << "succeed adding person to the dataBase\n";

                }
                else
                {
                    std::cout << "failed adding person to the dataBase\n";
                }
                break;
            }
			case EMsgOpcode::CLIENT2SERVER_FIND_PERSON_REQUEST_OPCODE:
            {
                std::cout << "CLIENT2SERVER_FIND_PERSON_REQUEST_OPCODE messag arrived\n";
                std::vector<std::string> namesResult;
                const Client2ServerFindPersonRequestMsg* pRequestMsg = reinterpret_cast<const Client2ServerFindPersonRequestMsg*>(pMsg);
                std::vector<int> features;
                features.insert(features.begin(), std::begin(pRequestMsg->features), std::end(pRequestMsg->features));
                bool findPersonStatus = FindPerson(features, pRequestMsg->numOfMatches, namesResult);
                if ((findPersonStatus == true) && (namesResult.empty()) == false)
                {
                    std::cout << "Top " << pRequestMsg->numOfMatches << " matches that have been found are:\n";
                    char personName[MAX_PERSONS][PERSON_NAME] = { 0 };
                    for (int i = 0; (i < MAX_PERSONS) && (i < namesResult.size()); i++)
                    {
                        if (namesResult[i].empty() == false)
                        {
                            std::cout << namesResult[i] << " ";
                            (void)memcpy(personName[i], namesResult[i].c_str(), namesResult[i].size());
                        }
                    }

                    std::cout << std::endl;
                    SendFindPersonResponseMsg(personName);
                }
                break;
            }

			default:
                std::cout << "wrong opcode\n";
				break;
		}
	}
}

void Server::SendAddPersonResponseMsg(bool addPersonStatus)
{
	static const int msgLength = sizeof(Server2ClientAddPersonResponseMsg);
    static Server2ClientAddPersonResponseMsg pResponseMsg;
    memset(&pResponseMsg, 0, sizeof(pResponseMsg));
	pResponseMsg.header.opcode = EMsgOpcode::SEREVER2CLIENT_ADD_PERSON_RESPONSE_OPCODE;
	pResponseMsg.header.length = msgLength;
	pResponseMsg.addPersonStatus = addPersonStatus;
	SendMsg(&pResponseMsg, msgLength);
}

void Server::SendFindPersonResponseMsg(void* personName)
{
	static const int msgLength = sizeof(Server2ClientFindPersonResponseMsg);
    static Server2ClientFindPersonResponseMsg pResponseMsg;
    memset(&pResponseMsg, 0, sizeof(pResponseMsg));
	pResponseMsg.header.opcode = EMsgOpcode::SEREVER2CLIENT_FIND_PERSON_RESPONSE_OPCODE;
	pResponseMsg.header.length = msgLength;
	(void)memcpy(pResponseMsg.personName, personName, (MAX_PERSONS * PERSON_NAME * sizeof(char)));
    SendMsg(&pResponseMsg, msgLength);
}

bool Server::AddPerson(std::pair<std::vector<int>, std::string> person)
{
    if (person.first.empty())
    {
        return false;
    }

    m_personsDataBase.emplace_back(person);
    std::cout << person.second << " has been added into the data base" << std::endl;
    return true;
}


bool Server::FindPerson(std::vector<int> person, int numOfMatches, std::vector<std::string>& namesResult)
{
    if (person.empty() || numOfMatches < 1)
    {
        return false;
    }

    int size = m_personsDataBase.size();

    std::vector<int> persons;
    std::array<int, 266> scalar{}; // maximum adding 10 people
    std::array<std::string, 266> names{}; // maximum adding 10 people

    for (int i = 0; i < size; ++i)
    {
        auto scalarProduct = std::inner_product(person.begin(), person.end(), m_personsDataBase[i].first.begin(), 0);
        scalar[i] = scalarProduct;
        names[i] = m_personsDataBase[i].second;
    }

    // find most similiarity
    for (int i = 0; i < numOfMatches; ++i)
    {
        // max element
        auto maxElement = std::max_element(scalar.begin(), scalar.end());

        //max index
        auto maxElementIndex = std::distance(scalar.begin(), maxElement);

        namesResult.emplace_back(names[maxElementIndex]);

        scalar[maxElementIndex] = INT_MIN;
    }

    return true;
}

std::vector<int> Server::generate_data_base()
{
    static std::uniform_int_distribution<int> distribution(
        std::numeric_limits<unsigned char>::min(),
        std::numeric_limits<unsigned char>::max());
    static std::default_random_engine generator;

    std::vector<int> data(FEATURES_SIZE);
    std::generate(data.begin(), data.end(), []() { return distribution(generator); });
    return data;
}

std::string GenerateUniqueName()
{
    char letters[ALPHA_BET_SIZE] = { 'a','b','c','d','e','f','g','h','i','j','k','l','m','n','o','p','q','r','s','t','u','v','w','x','y','z' };

    std::string ran = "";
    for (int i = 0; i < MAX_NAME_SIZE; ++i)
    {
        ran = ran + letters[rand() % ALPHA_BET_SIZE];
    }

    return ran;
}

std::vector<std::pair<std::vector<int>, std::string>> Server::CreatingPersonsDataBase(int numberOfPersons)
{
    std::vector<std::pair<std::vector<int>, std::string>> persons;

    for (int i = 0; i < numberOfPersons; ++i)
    {
        std::pair<std::vector<int>, std::string> person;
        person.first = generate_data_base();
        person.second = GenerateUniqueName();

        persons.push_back(person);
    }

    return persons;
}
