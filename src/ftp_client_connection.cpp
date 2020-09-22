/**
 * @file: ftp_client_connection.cpp
 * @author: Cole Houlihan, 655275501, Navodit Kaushal, 656 656 519, Section 2, CSCI 460, VIU
 * @version: 1.0.0
 * @modified: June 24, 2020
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
	struct sockaddr_in server_address;

	server_address.sin_family = AF_INET;
	//AF_INET is the IPv4 protocol, SOCK_STREAM is the chosen type which provides 'reliable, two-way, connection based byte streams'
	
	server_address.sin_port = htons(serverPort); 
	// https://linux.die.net/man/3/htons
	// https://www.gta.ufrj.br/ensino/eel878/sockets/sockaddr_inman.html
	
	int result = inet_pton(AF_INET, serverIP, &server_address.sin_addr); 
	// https://man7.org/linux/man-pages/man3/inet_pton.3.html

	socketDescriptor = socket(server_address.sin_family, SOCK_STREAM, 0);

	if (socketDescriptor < 0) { // returns 0 on success, or -1 if it fails. 
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

	if (connect(socketDescriptor, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
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


