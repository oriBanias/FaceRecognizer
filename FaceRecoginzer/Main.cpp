#include "Server.h"

int main()
{
	Server server;

	auto res = server.Init();
	if (res == true)
	{
		while (true)
		{
			server.HandleRcvMsgs();
		}
	}

	return 0;
}