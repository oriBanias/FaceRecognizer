#ifndef FACE_MSG_HANDLER_H
#define FACE_MSG_HANDLER_H

#include "ICD.h"
#include <winsock2.h>
#pragma comment(lib,"ws2_32.lib")

enum class EPlatform
{
	CLIENT = 0,
	SERVER
};

class MessageHandler
{
public:
	MessageHandler(unsigned short port);
	virtual ~MessageHandler();
	bool Init(EPlatform platform);
	void HandleRcvMsgs();

	bool SendMsg(const void* pMsg, int msgLength);

protected:
	static const int MAX_RCV_MSG_BUFFER = 2048;

	virtual void HandleIncomingMsg(const void* pMsg) = 0;

	SOCKET m_socket;
	unsigned short  m_port;
	char m_rcvBuffer[MAX_RCV_MSG_BUFFER];

private:

};

#endif // FACE_MSG_HANDLER_H