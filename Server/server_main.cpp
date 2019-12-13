#include <conio.h>

#include "Server.h"

int main(int argc, char** argv)
{
	Server server;
	int ch;

	while (1)
	{
		if (_kbhit())
		{
			ch = _getch();

			if (ch == 27) break;
		}
		server.Update();
	}

	return 0;
}