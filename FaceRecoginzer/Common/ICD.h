#ifndef FACE_ICD_H
#define FACE_ICD_H

#pragma pack(push,1)

enum class EMsgOpcode
{
	CLIENT2SERVER_ADD_PERSON_REQUEST_OPCODE = 1,
	CLIENT2SERVER_FIND_PERSON_REQUEST_OPCODE,
	SEREVER2CLIENT_ADD_PERSON_RESPONSE_OPCODE,
	SEREVER2CLIENT_FIND_PERSON_RESPONSE_OPCODE
};

// channel parameters
const unsigned short SERVER_PORT = 5555;

const int FEATURES_SIZE = 256;
const int PERSON_NAME = 20;
const int MAX_PERSONS = 10; // supporting maximum 10 persons
const int ALPHA_BET_SIZE = 26;
const int MAX_NAME_SIZE = 5;

struct MsgHeader
{
	EMsgOpcode opcode;
	int length;
};

struct Client2ServerAddPersonRequestMsg
{
	MsgHeader header;
	int features[FEATURES_SIZE];
	char personName[PERSON_NAME];
};

struct Client2ServerFindPersonRequestMsg
{
	MsgHeader header;
	int features[FEATURES_SIZE];
	int numOfMatches;
};

struct Server2ClientAddPersonResponseMsg
{
	MsgHeader header;
	bool addPersonStatus;
};

struct Server2ClientFindPersonResponseMsg
{
	MsgHeader header;
	char personName[MAX_PERSONS][PERSON_NAME];
};

#pragma pack(pop)

#endif // FACE_ICD_H