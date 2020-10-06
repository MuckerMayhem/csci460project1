/**
 * @file: ftp_client_command.cpp
 * @author: Name, Student Number, Section, CSCI 460, VIU
 * @version: 1.0.0
 * @modified: June 24, 2020
 *
 */

#include <string>
#include <string.h>
#include <iostream>
#include "ftp_client_command.hpp"
#include "ftp_client_control.hpp"
#include "ftp_client_ui.hpp"
#include "ftp_server_response.hpp"

//========================================================================================================= DONE
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

//========================================================================================================= DONE
void handleCommandUser(string username, string serverResponses[], int& serverResponseCount) {
    username = "USER " + username; 
    handleSimpleCommand(username, true, serverResponses, serverResponseCount); 
}

//========================================================================================================= DONE
void handleCommandPassword(string password, string serverResponses[], int& serverResponseCount) {
    password = "PASS " + password;
	handleSimpleCommand( password, true, serverResponses, serverResponseCount);
}

//=========================================================================================================
void handleCommandDirectory(string serverResponses[], int& serverResponseCount) {
    handleSimpleCommand("DIR", false, serverResponses, serverResponseCount);
}

//========================================================================================================= DONE
void handleCommandPrintDirectory(string serverResponses[], int& serverResponseCount) {
	handleSimpleCommand("PWD", false, serverResponses, serverResponseCount);
}

//=========================================================================================================
void handleCommandChangeDirectory(string path, string serverResponses[], int& serverResponseCount) {

}

//=========================================================================================================
void handleCommandChangeDirectoryUp(string serverResponses[], int& serverResponseCount) {
}
//=========================================================================================================
void handleCommandGetFile(string filename, string serverResponses[], int& serverResponseCount) {
}

//========================================================================================================= DONE
void handleCommandQuit(string serverResponses[], int& serverResponseCount) {
	handleSimpleCommand("QUIT", false, serverResponses, serverResponseCount);
	disconnectControlConnection();
}

//=========================================================================================================
void handlePassive(string serverResponses[], int& serverResponseCount) {
}

//=========================================================================================================
void handleNLIST(string serverResponses[], int& serverResponseCount) {
}

//=========================================================================================================
void handleRETR(string filename, string serverResponses[], int& serverResponseCount) {
}


//========================================================================================================= DONE
void handleSimpleCommand(string ftpCommand, bool checkAuthentication, string serverResponses[], int& serverResponseCount) {


    char buffer[BUFFER_SIZE] = {0};
    sendOnControl(ftpCommand.c_str(), BUFFER_SIZE);
    // Sends 'ftpCommand' request message to FTP server on the control connection.

    int received_bytes = receiveOnControl(buffer, BUFFER_SIZE);
    // Receives the response from the server against the request.

    if (received_bytes == 0 && checkAuthentication) {
        cout << "No response received!" << endl;
		exit(0);
    }

	serverResponses[0] = (string)buffer;
	serverResponseCount = 1;
	//begin looping through the received buffers until we see the first return code again
	//thus signalling we have received the entire message.
	if (buffer[3] == '-') {
		int i = 2;
		string code = serverResponses[0].substr(0, 3);

		receiveOnControl(buffer, BUFFER_SIZE);
		serverResponses[1] = (string)buffer;
		string next_code = serverResponses[1].substr(0, 3);
		serverResponseCount++;

		while(code != next_code) {
			receiveOnControl(buffer, BUFFER_SIZE);
			serverResponses[i] = (string)buffer;
			next_code = serverResponses[i].substr(0, 3);
			i++;
			serverResponseCount++;
		}
	}



    // char* tokenized = strtok(buffer," ");
    // int i = 0;
    // while(tokenized != NULL){
    //     serverResponses[i] += tokenized;
    //     serverResponses[i] += " ";
    //     tokenized = strtok(NULL," ");
    //     // i++;
    // }

    // cout << "SERV RESPONSE: " << serverResponses[0] << endl;

	serverResponseCount = 1;
    //https://fresh2refresh.com/c-programming/c-strings/c-strtok-function/
	return;
}

//=========================================================================================================
void getHostIPAndPortFromPassiveSuccessResponse(char* response, char* hostIP, int& hostPort) {
}

//=========================================================================================================
