/**
 * @file: ftp_client_control.cpp
 * @author: Cole Houlihan, 655275501, Navodit Kaushal, 656 656 519, Section 2, CSCI 460, VIU
 * @version: 1.0.0
 * @modified: June 24, 2020
 *
 */

#include "ftp_client_control.hpp"
#include "ftp_client_connection.hpp"


int controlSocket = -1; 			// Represents control connection to the server
int dataSocket = -1;				// Represents data connection to the server
bool isControlConnected = false; 	// Represents the status of the control connection
bool isDataConnected = false;		// Represents the status of the data connection



void connectToServerControl(const char* serverIP, int serverPort) {
	connectToServer(controlSocket, isControlConnected, serverIP, serverPort);
}

void connectToServerData(const char* serverIP, int serverPort) {	
	connectToServer(dataSocket, isControlConnected, serverIP, serverPort);
}

int sendOnControl(const char* buffer, int length) {
	return sendToServer(controlSocket, buffer, length);
}

int sendOnData(const char* buffer, int length) {
	return sendToServer(dataSocket, buffer, length);
}

int receiveOnControl(char* buffer, int length) {
	return receiveFromServer(controlSocket, buffer, length);
}
int receiveOnData(char* buffer, int length) {
	return receiveFromServer(dataSocket, buffer, length);
}

void disconnectControlConnection() {
	disconnectFromServer(controlSocket, isControlConnected);
}

void disconnectDataConnection() {
	disconnectFromServer(dataSocket, isDataConnected);
}

bool isControlConnectionAvailable() {
	return isControlConnected;
}

bool isDataConnectionAvailable() {
	return isDataConnected;
}