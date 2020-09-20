CC = g++
WFLAG = -Wall  		#Warning Flag
DFLAG = -ggdb		#Debugging Flag
CFLAG = -c		#Compiling Flag
SOURCES = src
INCLUDES = include
BUILDS = build
BINARY = bin
TESTS = test

all: ${BINARY}/ftpclient ${TESTS}/${BINARY}/ftpclienttest

${BINARY}/ftpclient: ${BUILDS}/ftp_client.o ${BUILDS}/ftp_client_ui.o ${BUILDS}/ftp_client_control.o \
${BUILDS}/ftp_client_connection.o ${BUILDS}/ftp_client_command.o
	${CC} ${WFLAG} ${DFLAG} -o ${BINARY}/ftpclient ${BUILDS}/ftp_client.o ${BUILDS}/ftp_client_ui.o ${BUILDS}/ftp_client_control.o \
	${BUILDS}/ftp_client_connection.o ${BUILDS}/ftp_client_command.o

${TESTS}/${BINARY}/ftpclienttest: ${TESTS}/${BUILDS}/ftp_client_test.o ${BUILDS}/ftp_client_connection.o \
		${BUILDS}/ftp_client_control.o ${BUILDS}/ftp_client_command.o
	${CC} ${WFLAG} -lboost_unit_test_framework -o ${TESTS}/${BINARY}/ftpclienttest ${TESTS}/${BUILDS}/ftp_client_test.o \
		${BUILDS}/ftp_client_connection.o ${BUILDS}/ftp_client_control.o ${BUILDS}/ftp_client_command.o


${BUILDS}/ftp_client.o: ${SOURCES}/ftp_client.cpp 
	${CC} ${WFLAG} ${DFLAG} ${CFLAG} -I${INCLUDES} -o ${BUILDS}/ftp_client.o ${SOURCES}/ftp_client.cpp

${BUILDS}/ftp_client_ui.o: ${SOURCES}/ftp_client_ui.cpp ${INCLUDES}/ftp_client_ui.hpp
	${CC} ${WFLAG} ${DFLAG} ${CFLAG} -I${INCLUDES} -o ${BUILDS}/ftp_client_ui.o ${SOURCES}/ftp_client_ui.cpp

${BUILDS}/ftp_client_control.o: ${SOURCES}/ftp_client_control.cpp ${INCLUDES}/ftp_client_control.hpp
	${CC} ${WFLAG} ${DFLAG} ${CFLAG} -I${INCLUDES} -o ${BUILDS}/ftp_client_control.o ${SOURCES}/ftp_client_control.cpp

${BUILDS}/ftp_client_connection.o: ${SOURCES}/ftp_client_connection.cpp ${INCLUDES}/ftp_client_connection.hpp
	${CC} ${WFLAG} ${DFLAG} ${CFLAG} -I${INCLUDES} -o ${BUILDS}/ftp_client_connection.o ${SOURCES}/ftp_client_connection.cpp

${BUILDS}/ftp_client_command.o: ${SOURCES}/ftp_client_command.cpp ${INCLUDES}/ftp_client_command.hpp
	${CC} ${WFLAG} ${DFLAG} ${CFLAG} -I${INCLUDES} -o ${BUILDS}/ftp_client_command.o ${SOURCES}/ftp_client_command.cpp

##
##${TESTS}/${BUILDS}/ftp_client_test.o: ${TESTS}/${SOURCES}/ftp_client_test.cpp ${INCLUDES}/ftp_client_connection.hpp \
##		${INCLUDES}/ftp_client_control.hpp ${INCLUDES}/ftp_client_command.hpp ${INCLUDES}/ftp_server_response.hpp
##	${CC} ${WFLAG} ${CFLAG} -I${INCLUDES} -o ${TESTS}/${BUILDS}/ftp_client_test.o ${TESTS}/${SOURCES}/ftp_client_test.cpp
##


.PHONY: run runserver test clean install uninstall

run: 	${BINARY}/ftpclient
	${BINARY}/ftpclient "127.0.0.1" 2020

runserver: ${TESTS}/${BINARY}/ftpserver
	${TESTS}/${BINARY}/ftpserver 2020

test:   ${TESTS}/${BINARY}/ftpclienttest ${TESTS}/${BINARY}/ftpserver
	${TESTS}/${BINARY}/ftpclienttest --log_level=all

clean:
	rm -rf ${BINARY}/* 
	rm -f ${TESTS}/${BINARY}/ftpclienttest
	rm -rf ${BUILDS}/*

install: ${BINARY}/ftpclient
	@echo Sorry, installtion has not been done.

uninstall:
	@echo Sorry, uninstalltion has not been done.



