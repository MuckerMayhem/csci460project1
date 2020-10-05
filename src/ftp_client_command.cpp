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
	exit(0);
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
    int sent_bytes = sendOnControl(ftpCommand.c_str(), BUFFER_SIZE);
    // Sends 'ftpCommand' request message to FTP server on the control connection.

    int received_bytes = receiveOnControl(buffer, BUFFER_SIZE);
    // Receives the response from the server against the request.

    if (received_bytes == 0 && checkAuthentication) {
        printf("No response received!\n");
		exit(0);
    }

    char* tokenized = strtok(buffer," ");
    int i = 0;
    while(tokenized != NULL){
        serverResponses[i] = tokenized;
        tokenized = strtok(NULL," ");
        i++;
    }

	serverResponseCount = received_bytes;
    //https://fresh2refresh.com/c-programming/c-strings/c-strtok-function/
	return;
}

//=========================================================================================================
void getHostIPAndPortFromPassiveSuccessResponse(char* response, char* hostIP, int& hostPort) {
}

//=========================================================================================================
