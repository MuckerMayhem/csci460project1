/**
 * @file: ftp_client_command.cpp
 * @author: Cole Houlihan,   655 275 501, F20N02, CSCI 460, VIU
 * @author: Navodit Kaushal, 656 656 519, F20N02, CSCI 460, VIU
 * @version: 1.0.3
 * @modified: October 17, 2020
 *
 */

#include <string>
#include <string.h>
#include <iostream>
#include <fstream>

#include "ftp_client_command.hpp"
#include "ftp_client_control.hpp"
#include "ftp_client_ui.hpp"
#include "ftp_server_response.hpp"

using namespace std;

void handleCommandHelp() {
    printf("Usage: csci460Ftp>> [ help | user | pass | pwd | dir | cwd | cdup | get | quit ] \n");
    printf("help                    Gives the list of FTP commands available and how to use them.\n");
    printf("user    <username>      Submits the <username> to FTP server for authentication.\n");
    printf("pass    <password>      Submits the <password> to FTP server for authentication.\n");
    printf("pwd                     Requests FTP server to print current directory.\n");
    printf("dir                     Requests FTP server to list the entries of the current directory.\n");
    printf("cwd     <dirname>       Requests FTP server to change current working directory.\n");
    printf("cdup                    Requests FTP server to change current directory to parent directory.\n");
    printf("get     <filename>      Requests FTP server to send the file with <filename>.\n");
    printf("quit                    Requests to end FTP session and quit.\n");
    return;
}
/**
 * Handles the 
 * @param
 * @return N/A
 */
void handleCommandUser(string username, string serverResponses[], int& serverResponseCount) {
    username = "USER " + username; 
    handleSimpleCommand(username, true, serverResponses, serverResponseCount); 
}

void handleCommandPassword(string password, string serverResponses[], int& serverResponseCount) {
    password = "PASS " + password;
	handleSimpleCommand( password, true, serverResponses, serverResponseCount);
}

void handleCommandDirectory(string serverResponses[], int& serverResponseCount) {
    handlePassive(serverResponses, serverResponseCount);
	handleNLIST(serverResponses, serverResponseCount);
}

void handleCommandPrintDirectory(string serverResponses[], int& serverResponseCount) {
	handleSimpleCommand("PWD", false, serverResponses, serverResponseCount);
}

void handleCommandChangeDirectory(string path, string serverResponses[], int& serverResponseCount) {
	path = "CWD " + path;
	handleSimpleCommand(path, false, serverResponses, serverResponseCount);
}

void handleCommandChangeDirectoryUp(string serverResponses[], int& serverResponseCount) {
	handleSimpleCommand("CDUP", false, serverResponses, serverResponseCount);
}

void handleCommandGetFile(string filename, string serverResponses[], int& serverResponseCount) {
	handlePassive(serverResponses, serverResponseCount);
	handleRETR(filename, serverResponses, serverResponseCount);
}

void handleCommandQuit(string serverResponses[], int& serverResponseCount) {
	handleSimpleCommand("QUIT", false, serverResponses, serverResponseCount);
	disconnectControlConnection();
}

void handlePassive(string serverResponses[], int& serverResponseCount) {
	
	string PASV = "PASV";
	sendOnControl(PASV.c_str(), PASV.length());

	char buffer[BUFFER_SIZE] = {0};

	int receivedCommand = receiveOnControl(buffer, BUFFER_SIZE);
	if (receivedCommand < 0) {
		cout << "Error reading response!" << endl;
		exit(0);
	}

	char hostIP[32];
	int hostPort = 0;
	getHostIPAndPortFromPassiveSuccessResponse(buffer, hostIP, hostPort);

	if (hostPort != 0) {
		connectToServerData(hostIP, hostPort);
		serverResponses[serverResponseCount] = buffer;
		serverResponseCount++;
	} else {
		cout << "Error in parsing hostPort or HostIP." << endl;
		exit(0);
	}

}

void handleNLIST(string serverResponses[], int& serverResponseCount) {

	char controlBuffer[BUFFER_SIZE] = {0};
	char dataBuffer[DATA_SOCKET_RECEIVE_BUFFER_SIZE] = {0};

	string NLST = "NLST";
	sendOnControl(NLST.c_str(), NLST.length());

	int NLISTBytes = receiveOnControl(controlBuffer, BUFFER_SIZE);
	if (NLISTBytes < 0) {
		cout << "Error reading from control socket!" << endl;
		exit(0);
	}
	serverResponses[serverResponseCount] = controlBuffer;
	serverResponseCount++;

	receiveOnControl(controlBuffer, BUFFER_SIZE);

	serverResponses[serverResponseCount] = controlBuffer;
	serverResponseCount++;

	int data_bytes = receiveOnData(dataBuffer, DATA_SOCKET_RECEIVE_BUFFER_SIZE);
	if (data_bytes < 0) {
		cout << "Error reading from data socket!" << endl;
		exit(0);
	}
	 serverResponses[serverResponseCount]= dataBuffer;
	 serverResponseCount++;

	disconnectDataConnection();
}

void handleRETR(string filename, string serverResponses[], int& serverResponseCount) {
	string commandFilename = "RETR " + filename;
	
	char controlBuffer[BUFFER_SIZE] = {0};
	char dataBuffer[DATA_SOCKET_RECEIVE_BUFFER_SIZE] = {0};

	sendOnControl(commandFilename.c_str(), BUFFER_SIZE);

	int controlResponseBytes = receiveOnControl(controlBuffer, BUFFER_SIZE);

	if (controlResponseBytes < 0) {
		cout << "Error on reading from control socket!" << endl;
		exit(0);
	}
	
	serverResponses[serverResponseCount] = controlBuffer;
	serverResponseCount++;

	int dataResponseBytes = receiveOnData(dataBuffer, DATA_SOCKET_RECEIVE_BUFFER_SIZE);
	if (dataResponseBytes < 0) {
		cout << "Error reading from data socket!" << endl;
		exit(0);
	}

	// serverResponses[serverResponseCount]= dataBuffer;
	// serverResponseCount++;


	ofstream receivedFile;
	receivedFile.open(filename);
	
	if (!receivedFile) {
		cout << "Failed to open file!" << endl;
		exit(0);
	}

	receivedFile << dataBuffer;

	receivedFile.close();

	controlResponseBytes = receiveOnControl(controlBuffer, BUFFER_SIZE);

	if (controlResponseBytes < 0) {
		cout << "Error on reading from control socket!" << endl;
		exit(0);
	}
	serverResponses[serverResponseCount] = controlBuffer;
	serverResponseCount++;

	disconnectDataConnection();
}


void handleSimpleCommand(string ftpCommand, bool checkAuthentication, string serverResponses[], int& serverResponseCount) {

    char buffer[BUFFER_SIZE] = {0};
    sendOnControl(ftpCommand.c_str(), BUFFER_SIZE);
    // Sends 'ftpCommand' request message to FTP server on the control connection.

    int receivedBytes = receiveOnControl(buffer, BUFFER_SIZE);
    if (receivedBytes == 0 && checkAuthentication) {
        cout << "No response received!" << endl;
		exit(0);
    }
	//need to save the buffer into our log before overwriting it.
	serverResponses[serverResponseCount] = string(buffer);
	serverResponseCount++;

	//begin looping through the received buffers until we see the first return code again
	//thus signalling we have received the entire message.
	if (buffer[3] == '-') {
		string code = string(buffer).substr(0, 3); //need code from original buffer

		string nextCode;
		do {
			receiveOnControl(buffer, BUFFER_SIZE);
			serverResponses[serverResponseCount] = string(buffer); 
			serverResponseCount++;
			nextCode = string(buffer).substr(0, 3);
		} while(code != nextCode);
	} 
	return;
}

void getHostIPAndPortFromPassiveSuccessResponse(char* response, char* hostIP, int& hostPort) {
	//if error, we should write -1 into char* hostIP IOT respect logic of
	//handlePassive()

	char* token = NULL;
	string result = "";
	int val1, val2;

	//strip out unecessary string content in response
	token = strtok(response, ",()");

	// 192,168, 1,65,  202, 143
	//  IP, IP,IP,IP, VAL1, VAL2
	//   0|  1| 2| 3|    4|   5|
	int parsedValsCount = 5;
	for(int i = 0; i < parsedValsCount+1; i ++){
		if(i < 4){
			//this is our hostIP content
			token = strtok(NULL, ",()");
			if (i == 3) {
				result = result + string(token);
			} else {
				result = result + string(token) + ".";	
			}
		} else {
			if ( i == 4){
				token = strtok(NULL, ",()");
				val1 = atoi(token);
			}
			if ( i == 5){
				token = strtok(NULL, ",()");
				val2 = atoi(token);
			}
		}
	}
	strcpy(hostIP, result.c_str()); //hostIP set

	// use i = 4 for int VAL1
	// use i = 5 for int VAL2
	// hostPort = (VAL1*256)+VAL2;
	hostPort = (val1 * 256) + val2;

	return;
}