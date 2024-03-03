#include <stdio.h> 
#include <stdlib.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "downloadApplication.h"

int main(int argc, char * argv[]){

    if (argc != 2){
        fprintf(stderr, "usage: download ftp:://[<user>:<password>@]<host>/<url-path>\n");
        exit(1);
    }

    URL url;

    if (parse(argv[1], &url) < 0){
        fprintf(stderr, "Parsing url error\n");
        exit(1);
    }


    if(download(&url) < 0){
        fprintf(stderr, "Downloading file error\n");
        exit(1);
    }

    return 0;
}