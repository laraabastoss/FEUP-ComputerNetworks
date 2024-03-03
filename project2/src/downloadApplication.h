#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>

#include "parse.h"

int download(URL * urlArgs);
int createSocketConnection(char * ip, int port);
int readResponse(int socketfd, char * response, int * code);
int sendCommand(int flag, char * command, int socketfd, char * arguments);
int login(int socketfd, URL * urlArgs);
int passive(int socketfd, char * response_ip, int * response_port);
