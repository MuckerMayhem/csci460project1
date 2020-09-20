/**
 *  @file: ftp_client_control.hpp
 *  @author: Humayun Kabir, Instructor, CSCI 460, VIU
 *  @version: 1.0.0
 *  @modified: June 22, 2020
 */


#ifndef __FTP_CLIENT_CONTROL_HEADER__
#define __FTP_CLIENT_CONTROL_HEADER__

#define FTP_CLIENT_DEAFULT_SERVER_IP "127.0.0.1"
#define FTP_CLIENT_DEFAULT_SERVER_PORT 21
#define BUFFER_SIZE 512

/*
 * Implemention of this header file will require to declare some variables
 * as follows:
 */

// int controlSocket = -1; 		-- Represents control connection to the server
// int dataSocket = -1;			-- Represents data connection to the server
// bool isControlConnected = false; 	-- Represents the status of the control connection
// bool isDataConnected = false;	-- Represents the status of the data connection



void connectToServerControl(const char* serverIP, int serverPort);
// Sends a control connection request on severPort of a sever whose IP address is equal to serverIP.
// If the control connection is successful sets its value to controlSocket and 
// sets isControlConnected to true.
// Calls connectToServer() function from 'ftp_client_connection.cpp' to do the above.

void connectToServerData(const char* serverIP, int serverPort);
// Sends a data connection request on severPort of a sever whose IP address is equal to serverIP.
// If the data connection is successful sets its value to dataSocket and 
// sets isDataConnected to true.
// Calls connectToServer function from 'ftp_client_connection.cpp' to do the above.

int sendOnControl(const char* buffer, int length);
// Sends the info in the buffer of length equal to 'length' on control connection.
// Returns the actual number of bytes sent.
// Calls sendToServer() function from 'ftp_client_connection.cpp' to do the above.

int sendOnData(const char* buffer, int length);
// Sends the info in the buffer of length equal to 'length' on data connection.
// Returns the actual number of bytes sent.
// Calls sendToServer() function from 'ftp_client_connection.cpp' to do the above.

int receiveOnControl(char* buffer, int length);
// Receives the info in the buffer of length equal to 'length' on control connection.
// Returns the actual number of bytes received.
// Calls receiveFromServer() fucntion from 'ftp_client_connection.cpp' to do the above.

int receiveOnData(char* buffer, int length);
// Receives the info in the buffer of length equal to 'length' on data connection.
// Returns the actual number of bytes received.
// Calls receiveFromServer() function from 'ftp_client_connection.cpp' to do the above.

void disconnectControlConnection();
// Closes the opened control connection and sets isControlConnected to false;
// Calls disconnectFromServer() function from 'ftp_client_connection.cpp' to do the above

void disconnectDataConnection();
// Closes the opened data connection and sets isDataConnected to false;
// Call disconnectFromServer() function from 'ftp_client_connection.cpp' to do the above.

bool isControlConnectionAvailable();
// Returns the value of isControlConnected.

bool isDataConnectionAvailable();
// Returns the value of isDataConnected.

#endif
