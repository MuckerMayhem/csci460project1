/**
 * @file: ftp_client_command.cpp
 * @author: Cole Houlihan,   655 275 501, F20N02, CSCI 460, VIU
 * @author: Navodit Kaushal, 656 656 519, F20N02, CSCI 460, VIU
 * @version: 1.0.3
 * @modified: October 17, 2020
 *
 */
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <cerrno>
#include <iostream>
#include <clocale>

#include "ftp_client_connection.hpp"

using namespace std;

void connectToServer(int& socketDescriptor, bool& isConnected, const char* serverIP, int serverPort) {
	struct sockaddr_in serverAddress;

	serverAddress.sin_family = AF_INET;
	
	serverAddress.sin_port = htons(serverPort); 
	// https://linux.die.net/man/3/htons
	// https://www.gta.ufrj.br/ensino/eel878/sockets/sockaddr_inman.html
	
	//AF_INET is the IPv4 protocol, SOCK_STREAM is the chosen type which provides 'reliable, two-way, connection based byte streams'
	int result = inet_pton(AF_INET, serverIP, &serverAddress.sin_addr); 
	// https://man7.org/linux/man-pages/man3/inet_pton.3.html

	socketDescriptor = socket(serverAddress.sin_family, SOCK_STREAM, 0); // returns 0 on success, or -1 if it fails. 

	if (socketDescriptor < 0) {
		cout << "An error occured, connection was not established!\n";
		cout << "Error: " << strerror(errno) << endl;
		return;
	}
	// reference: https://man7.org/linux/man-pages/man2/socket.2.html
	// https://pubs.opengroup.org/onlinepubs/009695399/functions/socket.html

	if (result == -1) {
		cout << "Error occured in address conversion!" << endl;
		cout << "Error: " << strerror(errno) << endl; //error code: EAFNOSUPPORT
		return;
	}

	if (result == 0) {
		cout << "Invalid address supplied!" << endl;
		return;
	}

	if (connect(socketDescriptor, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) {
		cout << "Connection failed!" << endl;
		cout << "Error: " << strerror(errno) << endl;
		return;
	}   // https://linux.die.net/man/3/connect

	isConnected = true;
	return;
}


void disconnectFromServer(int& socketDescriptor, bool& isConnected) {
	if (shutdown(socketDescriptor, 2) < 0) {//https://www.gnu.org/software/libc/manual/html_node/Closing-a-Socket.html
		cout << "Error occured, shutdown of socket may have failed." << endl;
		cout << "Error: " << strerror(errno) << endl; 
		// https://en.cppreference.com/w/cpp/error/errno
		// https://man7.org/linux/man-pages/man3/strerror.3.html
		return;
	}
	isConnected = false;
	return;
}

int sendToServer(int sockDescriptor, const char* message, int messageLength) {
	return send(sockDescriptor, message, messageLength, 0);
}

int receiveFromServer(int sockDescriptor, char* message, int messageLength) {
	return recv(sockDescriptor, message, messageLength, 0);
}


