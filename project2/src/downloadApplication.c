#include "downloadApplication.h"

int download(URL * urlArgs){
    int socketfd=-1;
    printf("%i",socketfd);
    if ((socketfd = createSocketConnection(urlArgs->ip, 21) )<0){
        fprintf(stderr, "Initializing socket error\n");
        return -1;
    }
    printf("%i",socketfd);

    char response[1024];
    int code = 0;

    memset(response, 0, sizeof(response));

    if (readResponse(socketfd, response, &code) < 0) return -1;
    printf("a\n");
    if (code != 220) {
        fprintf(stderr, "Response code error\n");
        return -1;
    }
    printf("b\n");

    if (login(socketfd, urlArgs) < 0) {
        fprintf(stderr, "Login error\n");
        return -1;
    }
      printf("login\n");

    char ip[20];
    int port;

    if (passive(socketfd, ip, &port) < 0) {
        fprintf(stderr, "Setting passive mode error\n");
        return -1;
    }
       printf("passive\n");
    int data;
    if((data = createSocketConnection(ip, port)) < 0){
        fprintf(stderr, "Data connection error\n");
        return -1;
    }

           printf("connection\n");
    if (sendCommand(1, "RETR", socketfd, urlArgs->path) < 0){
        fprintf(stderr, "Sending RETR command error\n");
        return -1;
    }

    code = 0;
    memset(response, 0, sizeof(response));

    if (readResponse(socketfd, response, &code) < 0) return -1;
    if (code != 150) {
        fprintf(stderr, "Response code error\n");
        return -1;
    }

    int fd;
    if ((fd = open(urlArgs->filename,O_WRONLY | O_CREAT, 0777))< 0) {
        fprintf(stderr, "Opening file error\n");
        return -1;
    }

    char buf[1024];
    int bytesRead;

    do{
        bytesRead = read(data, buf, 1024);
        if (write(fd,buf,bytesRead)<0) return -1;
    }while(bytesRead);

    if (close(fd) < 0) {
        fprintf(stderr, "Error closing file!\n");
        return -1;
    }

    code = 0;
    memset(response, 0, sizeof(response));

    if (readResponse(socketfd, response, &code) < 0) return -1;
     if (code != 226) {
        fprintf(stderr, "Response code error\n");
        return -1;
    }

    if (close(socketfd) <0){
        fprintf(stderr,"Closing socket error\n");
        return -1;
    }

    if (close(data) <0){
        fprintf(stderr,"Closing socket error\n");
        return -1;
    }

    return 0;
}

int createSocketConnection(char * ip, int port){

    int sockfd;
    struct sockaddr_in address;

    bzero((char*) &address, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(ip);
    address.sin_port = htons(port);

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        fprintf(stderr, "Opening socket error\n");
        return -1;
    }
    printf("%i\n",sockfd);

    if(connect(sockfd, (struct sockaddr *) &address, sizeof(address)) < 0){
        fprintf(stderr, "Server connection error\n");
        return -1;
    }

    printf("%i\n",sockfd);
    return sockfd;
    
}

int readResponse(int socketfd, char * response, int * code){
    FILE * socket = fdopen(socketfd, "r");
    printf("%i",socketfd);
    char * buf=NULL;
    int totalBytes = 0;
    size_t readBytes = 0;

    printf("antes do while\n");

    while(getline(&buf, &readBytes, socket) > 0){
        printf("antes\n");
        strncat(response, buf, readBytes - 1);
        printf("depois\n");
        totalBytes += readBytes;
        printf("%s", buf);
        if (buf[3] == ' '){
            sscanf(buf, "%d", code);
            break;
        }
    }

      printf("depois do while\n");

    free(buf);
    return totalBytes;
}

int sendCommand(int flag, char * command, int socketfd, char * arguments){
    char cmd[256];
    strcpy(cmd, command);

    if (flag){
        strcat(cmd, " ");
        strcat(cmd, arguments);
    }

    strcat(cmd, "\r\n");

    if ((write(socketfd, cmd, strlen(cmd)) != strlen(cmd))){
        fprintf(stderr, "Writing message error\n");
        return -1;
    }

    return 0;
}

int login(int socketfd, URL * urlArgs){
    if (sendCommand(1, "USER", socketfd, urlArgs->user) < 0){
        fprintf(stderr, "Sending USER command error\n");
        return -1;
    }

    char response[1024];
    int code = 0;

    memset(response, 0, sizeof(response));
    code = 0;

    if (readResponse(socketfd, response, &code) < 0) return -1;
    if (code != 331 && code != 230) {
        fprintf(stderr, "Response code error\n");
        return -1;
    }


    if (sendCommand(1, "PASS", socketfd, urlArgs->password) < 0){
        fprintf(stderr, "Sending PASS command error\n");
        return -1;
    }

    memset(response, 0, sizeof(response));
    code = 0;

    if (readResponse(socketfd, response, &code) < 0) return -1;
    if (code != 230) {
        fprintf(stderr, "Response code error\n");
        return -1;
    }

    return 0;
}

int passive(int socketfd, char * response_ip, int * response_port){
    char response[1024];
    int code = 0;
    memset(response, 0, sizeof(response));

    if (sendCommand(0, "PASV", socketfd, NULL) < 0){
        fprintf(stderr, "Sending PASV command error\n");
        return -1;
    }

    if (readResponse(socketfd, response, &code) < 0) return -1;
    if (code != 227) {
        fprintf(stderr, "Response code error\n");
        return -1;
    }

    strtok(response, "(");
    char * arguments = strtok(NULL, ")");

    int ip[4], port[2];
    sscanf(arguments, "%d, %d, %d, %d, %d, %d", &ip[0], &ip[1], &ip[2], &ip[3], &port[0], &port[1]);

    sprintf(response_ip, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3]);
    *response_port = port[0] * 256 + port[1];

    return 0;
}
