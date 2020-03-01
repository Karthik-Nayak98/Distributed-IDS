#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>

#define PORT 8080
#define MAX 80
#define SA struct sockaddr

int main()
{
	char buff[MAX]="cryptojacker.out";
	int sockfd, connfd;

	struct sockaddr_in servaddr, cli;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd==0){printf("Unsuccessful");exit(0);}
	
	printf("Socket successfully created..\n"); 
    bzero(&servaddr, sizeof(servaddr)); 
  
    // assign IP, PORT 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
    servaddr.sin_port = htons(PORT); 

    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0)
    { 
        printf("connection with the server failed...\n"); 
        exit(0); 
    } 

	write(sockfd, buff, sizeof(buff));
    
    close(sockfd);
	return 0;
}
