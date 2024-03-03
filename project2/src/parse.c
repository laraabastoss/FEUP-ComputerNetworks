#include "parse.h"

int parse(char * url, URL * parsed){
    char * protocol = strtok(url, "/");
    char * arguments = strtok(NULL, "/");
    char * path = strtok(NULL, "");

    if (protocol == NULL || arguments == NULL || path == NULL){
        fprintf(stderr, "Invalid URL\n");
        return -1;
    }


    if (strchr(arguments, '@') != NULL){
        char * login = strtok(arguments, "@");
        char * host = strtok(NULL, "@");

        char * user = strtok(login, ":");
        char * password = strtok(NULL, ":");

        strcpy(parsed->user, user);

        if(password)
            strcpy(parsed->password, password);
        else
            strcpy(parsed->password, PASSWORD_DEFAULT);

        strcpy(parsed->host, host);
    }
    else{
        strcpy(parsed->user, ANONYMOUS_USER);
        strcpy(parsed->password, PASSWORD_DEFAULT);
        strcpy(parsed->host, arguments);
    }

    strcpy(parsed->path, path);
    char *lastSlash = strrchr(parsed->path, '/');

    if (lastSlash != NULL) {

        strcpy(parsed->filename, lastSlash + 1);
    } else {
        strcpy(parsed->filename, parsed->path);
    }

    if (strlen(parsed->host) == 0 || strlen(parsed->path) == 0){
        fprintf(stderr, "Invalid URL\n");
        return -1;
    }

 
    struct hostent *host;
    if ((host = gethostbyname(parsed->host)) == NULL){
        fprintf(stderr, "Invalid hostname\n");
        return -1;
    }


    strcpy(parsed->ip, inet_ntoa(*((struct in_addr *) host->h_addr)));
    printf("\nUser: %s\n", parsed->user);
    printf("Password: %s\n", parsed->password);
    printf("Host: %s\n", parsed->host);
    printf("Path: %s\n", parsed->path);
    printf("File name: %s\n", parsed->filename);
    printf("IP Address : %s\n\n", parsed->ip);
    return 0;
}