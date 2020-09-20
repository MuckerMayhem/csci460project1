/**
 *  @file: ftp_client_ui.hpp
 *  @author: Humayun Kabir, Instructor, CSCI 460, VIU
 *  @version: 1.0.0
 *  @modified: June 22, 2020
 */

#ifndef __FTP_CLIENT_UI_HEADER__
#define __FTP_CLIENT_UI_HEADER__

#include <string>

#define FTP_CLIENT_PROMT "CSCI460FTP>>"


void getUserCommand();
// Displays a command line prompt as follows:
//      CSCI460FTP>>
// Lets user to type command and reads the user command from the prompt.
// Interprets and handles user command by calling function interpreteAndHandleUserCommand().

void interpretAndHandleUserCommand(std::string command);
// Interprets and handles user command
// Calls appropriate handleCommandXXXX() function from 'ftp_client_command.hpp'.
// Displays all FTP server responses calling showFtpResponse() function.

void showFtpResponse(std::string response); 
// Displays server response to the user.
// For example, success response of user command
//      CSCI460FTP>>user csci460
// should be displayes as 
//      331 Username okay, need password.
//
// Success response of user command
//      CSCI460FTP>>dir
// should be  displayed as 
//      226 Directory listing (11) entries. Closing data connection.
//
//      D       .
//      D       ..
//      D       bin
//      F       makefile                2631
//      D       include
//      F       README                  0
//      D       build
//      D       resource
//      D       src

#endif
