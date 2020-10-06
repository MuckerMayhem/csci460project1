/**
 * @file: ftp_client_ui.hpp
 * @author: Cole Houlihan, 655275501, Navodit Kaushal, 656 656 519, Section 2,
 * CSCI 460, VIU
 * @version: 1.0.0
 * @modified: October 4, 2020
 *
 */

#include <stdio.h>
#include <string.h>

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

#include "ftp_client_command.hpp"
#include "ftp_client_ui.hpp"
#include "ftp_server_response.hpp"

//=========================================================================================================
// DONE
void getUserCommand() {
	string input;
	cout << FTP_CLIENT_PROMPT;
	getline(cin, input);

	interpretAndHandleUserCommand(input);
	return;
}

//=========================================================================================================
// IN PROGRESS
void interpretAndHandleUserCommand(std::string command) {
	string serverResponse[FTP_RESPONSE_MAX_LENGTH];
	int serverResponseCount;
	string argument = " ";
	istringstream stream(command);
	stream >> command;
	stream >> argument;

	transform(command.begin(), command.end(), command.begin(), ::tolower);
	// https://stackoverflow.com/questions/3403844/tolower-function-for-c-strings

	// HELP ==================================================
	if (command == "help") {
		handleCommandHelp();

		// USER ==================================================
	} else if (command == "user") {
		if (argument != " ") {
			handleCommandUser(argument, serverResponse, serverResponseCount);
		} else {
			cout << "No username provided." << endl;
			cout << "please use the 'help' command for more information"
				 << endl;
		}

		// PASS ==================================================
	} else if (command == "pass") {
		if (argument != " ") {
			handleCommandPassword(argument, serverResponse,
								  serverResponseCount);
		} else {
			cout << "No password provided." << endl;
			cout << "please use the 'help' command for more information"
				 << endl;
		}

		// PWD ===================================================
	} else if (command == "pwd") {
		handleCommandPrintDirectory(serverResponse, serverResponseCount);

		// DIR ===================================================
	} else if (command == "dir") {
		handleSimpleCommand("DIR", false, serverResponse, serverResponseCount);

		// CWD ===================================================
	} else if (command == "cwd") {
		if (argument != " ") {
			handleCommandChangeDirectory(/*some path*/ argument, serverResponse,
										 serverResponseCount);
		} else {
			cout << "cwd requires argument: path" << endl;
			cout << "please use the 'help' command for more information"
				 << endl;
		}

		// CDUP ===================================================
	} else if (command == "cdup") {
		cout << "Changing directory UP" << endl;
		handleCommandChangeDirectoryUp(serverResponse, serverResponseCount);

		// GET  ===================================================
	} else if (command == "get") {
		handleCommandGetFile(argument, serverResponse, serverResponseCount);

		// QUIT ==================================================
	} else if (command == "quit") {
		cout << "ENDING PROGRAM" << endl;
		handleCommandQuit(serverResponse, serverResponseCount);

		// CATCH ALL ==============================================
	} else {
		cout << "No valid command received" << endl;
		cout << "please use the 'help' command for more information" << endl;
		return;
	}

	string response;
	int n_columns = sizeof(serverResponse[0]) / sizeof(serverResponse[0][0]);
	// https://stackoverflow.com/questions/17915759/how-to-count-items-of-2d-array

	stringstream temp;
	for (int i = 0; i < n_columns; i++) {
		// cout << "LINE " << i << " " << serverResponse[i] << endl;
		temp << " " << serverResponse[i];
		// https://www.gamedev.net/forums/topic/706449-creating-single-string-from-array-of-strings-c/
	}

	response = temp.str();  // the final string
	showFtpResponse(response);
	return;
}

//=========================================================================================================
// DONE
void showFtpResponse(std::string response) {
	cout << response << endl;
	return;
}