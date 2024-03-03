#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PASSWORD_DEFAULT "password"
#define ANONYMOUS_USER "anonymous"

typedef struct{
    char host[512];
    char user[256];
    char password[256];
    char path[1024];
    char filename[512];
    char ip[20];
} URL;

int parse(char * url, URL * parsed);