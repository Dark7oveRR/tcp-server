#include <iostream>
#include <string>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

using namespace std;

void main() {
	string Host = "127.0.0.1"; // Ip Address of the server!!
	int port = 5004; // Server Port

	// Initialize winsock
	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0) {
		cerr << "Can't start winsockl, error #" << wsResult << endl;
		return;
	}

	// Create socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		cerr << "Can't create socket, Error #" << WSAGetLastError() << endl;
		WSACleanup();
		return;
	}

	// Fill structure in!
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, Host.c_str(), &hint.sin_addr);

	// connect to server!
	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR) {
		cerr << "Can't connect to server, error #" << WSAGetLastError() << endl;
		closesocket(sock);
		WSACleanup();
		return;
	}

	// loop send and receive data!
	char buf[4096];
	string userInput;

	do {
		// promt the user for text!
		cout << "> ";
		getline(cin, userInput);

		if (userInput.size() > 0) { // make sure the user has typed something or gonna crash server!
			// send the text!
			int sendResult = send(sock, userInput.c_str(), userInput.size() + 1, 0);
			if (sendResult != SOCKET_ERROR) {
				// Wait for response
				ZeroMemory(buf, 4096);
				int bytesReceived = recv(sock, buf, 4096, 0);
				if (bytesReceived) {
					// response to console!
					cout << "SERVER> " << string(buf, 0, bytesReceived) << endl;
				}
			}
		}
	} while (userInput.size() > 0);

	// close down everything
	closesocket(sock);
	WSACleanup();
}