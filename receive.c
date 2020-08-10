#include <stdio.h>
#include<stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include<unistd.h>
#include<arpa/inet.h>


#define MAX 80
#define PORT 8080
#define SA struct sockaddr

char block_cmd[200] = "echo ubuntu@123 | sudo -S iptables -A INPUT -s ";

int main()
{
    int sockfd, connfd, len;
    struct sockaddr_in serv_addr, cli;

    // socket create and verification
    //sockfd = socket(AF_INET, SOCK_STREAM, 0);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1)
    {
        printf("Socket creation failed...\n");
        exit(0);
    }
    else
        printf("Receiver Socket successfully created..\n");

    bzero((char *)&serv_addr, sizeof(serv_addr));

    // assign IP, PORT
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(PORT);

    int broadcast = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) < 0)
    {
        printf("Error in setting Broadcast option\n");
        // close(sockfd);
    }

    if (bind(sockfd, (SA *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("Error in BINDING");
        close(sockfd);
        return 0;
    }

    len = sizeof(SA);
    char buff[MAX];

    while(1){
        recvfrom(sockfd, buff, MAX, 0, (SA *)&serv_addr, &len);
        printf("%s received\n", buff);
        strcat (block_cmd, buff);
        strcat(block_cmd, " -j DROP");
        int status = system(block_cmd);
        printf("\n%s Blocked\n", buff);
        strcpy(block_cmd,"echo ubuntu@123 | sudo -S iptables -A INPUT -s ");
    }

    // close(sockfd);
}