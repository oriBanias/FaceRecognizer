#ifndef FACE_CLIENT_H
#define FACE_CLIENT_H

#include "MsgHandler.h"

class Client : public MessageHandler
{
public:
	Client();
	virtual ~Client();
	bool Init()
	{
		return MessageHandler::Init(EPlatform::CLIENT);
	}

	void SendAddPersonRequestMsg(int features[FEATURES_SIZE], char personName[PERSON_NAME]);
	void SendFindPersonRequestMsg(int features[FEATURES_SIZE], int numOfMatches);

protected:
	virtual void HandleIncomingMsg(const void* pMsg);

private:

};

#endif // FACE_CLIENT_H