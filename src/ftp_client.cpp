/**
 *  @file: ftp_client.hpp
 *  @author: Humayun Kabir, Instructor, CSCI 460, VIU
 *  @version: 1.0.0
 *  @modified: June 22, 2020
 *
 *  FTP Client Application.
 *  Connects to a FTP Server Application running at specific port and IP Address.
 *  Accepts FTP commands from the user and sends the command to the server.
 *  Receives the response from the server and presents server response to the user.
 *  Capable of handling following FTP requests:
 *  	USER <username>
 *  	PASS <password>
 *  	PWD
 *  	CWD  <dir>
 *  	CDUP
 *  	PASV
 *  	NLIST
 *  	RETR <filename>
 *  	QUIT
 */

#include <iostream>
#include <iomanip>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include "ftp_client_ui.hpp"
#include "ftp_client_control.hpp"
#include "ftp_server_response.hpp"

using namespace std;

/*
 * Forward declaration of helper functions
 */
void usage(const char* prog);				// Prints usage text of FTP Client Application
char* baseName(const char* pathname);			// Returns the name of the code base of FTP Client Application

/**
 * The main function of FTP Client Application.
 * Accepts FTP Server IP Address and Port Number as command line parameters.
 * Sends a connection request to a FTP server, identified by the port and the IP address.
 * Once the connection is established, accepts user cammands.
 * Converts each user command to an appropriate FTP request and sends the request to FTP server.
 * Receives the response from FTP server and shows the response to the user.
 */
int main(int argc, char** argv) {
    if(argc<3) {
        char* base_name = baseName(argv[0]);
        usage(base_name);
        return 1;
    }
    
    //Retrieve FTP Server IP and Port from the commandline arguments
    char* serverIP = argv[1];
    int serverPort  = atoi(argv[2]);
 
    //Send a connection request to the FTP server
    connectToServerControl(serverIP, serverPort);
   
    //Check whether the connection established or not
    if(isControlConnectionAvailable()) {
        usleep(5000);
	//Receive server response and show it to the user
        char* response = new char[FTP_RESPONSE_MAX_LENGTH];
        memset(response, 0, FTP_RESPONSE_MAX_LENGTH);
        if(receiveOnControl(response, FTP_RESPONSE_MAX_LENGTH) != -1) {
            showFtpResponse(response);
        }
        delete [] response;
	//Accept and handle user command
        while(1) {
            getUserCommand();
        }
    }
    else {
        cout<<"Can't connect to the FTP server"<<endl;
    }
    return 0;
}

/**
 * Shows usage text of FTP client applicaltion.
 * @param: base - passes the name of the base code of FTP client application.
 */
void usage(const char* base) {
    cout<<setw(6)<<" "<<"Usage:  "<<base<<"  <ftp_server_ip>  "<<"<ftp_server_port>"<<endl;
}

/**
 * Gives the name of the base code of FTP client application from its path name.
 * @param: pathname - passes the path name of FTP client application base code.
 * @return: the name of the base code of FTP client application.
 */
char* baseName(const char* pathname) {
    char* path_name = const_cast<char*>(pathname);
    char* base_name = path_name;
    if((base_name=strrchr(path_name, '/')) != NULL) {
        ++base_name;
    }
    return base_name;
}
