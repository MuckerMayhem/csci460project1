/**
 * @file: ftp_client_control.cpp
 * @author: Name, Student Number, Section, CSCI 460, VIU
 * @version: 1.0.0
 * @modified: June 24, 2020
 *
 */



int controlSocket = -1; 		// Represents control connection to the server
int dataSocket = -1;			// Represents data connection to the server
bool isControlConnected = false; 	// Represents the status of the control connection
bool isDataConnected = false;	// Represents the status of the data connection



void connectToServerControl(const char* serverIP, int serverPort)
{

}
void connectToServerData(const char* serverIP, int serverPort)
{

}
int sendOnControl(const char* buffer, int length)
{

}
int sendOnData(const char* buffer, int length)
{

}
int receiveOnControl(char* buffer, int length)
{

}
int receiveOnData(char* buffer, int length)
{

}
void disconnectControlConnection()
{

}

void disconnectDataConnection()
{

}

bool isControlConnectionAvailable()
{

}

bool isDataConnectionAvailable()
{

}