#ifndef FACE_SERVER_H
#define FACE_SERVER_H

#include "Common/MsgHandler.h"
#include "ICD.h"
#include <vector>
#include <string>


class Server : public MessageHandler
{
public:
	static const int NUM_OF_INITALIZE_PERSONS_IN_DATABASE = 64;
	Server();
	virtual ~Server();

	bool Init()
	{
		m_personsDataBase = CreatingPersonsDataBase(NUM_OF_INITALIZE_PERSONS_IN_DATABASE);
		return MessageHandler::Init(EPlatform::SERVER);
	}

protected:
	// Logic
	bool AddPerson(std::pair<std::vector<int> , std::string> person);
	bool FindPerson(std::vector<int> person, int numOfMatches, std::vector<std::string>& namesResult);

	// Messages
	virtual void HandleIncomingMsg(const void* pMsg);
	void SendAddPersonResponseMsg(bool addPersonStatus);
	void SendFindPersonResponseMsg(void* personName);

	std::vector<std::pair<std::vector<int>, std::string>> CreatingPersonsDataBase(int numberOfPersons);
	std::vector<int> generate_data_base();


	std::vector<std::pair<std::vector<int>, std::string>> m_personsDataBase;

private:

};

#endif // FACE_SERVER_H