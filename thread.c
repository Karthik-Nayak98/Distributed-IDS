#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/ip.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/ip_icmp.h>
#include <time.h>
#include <pthread.h>
#include "dictionary.h"

#define dos_threshold 10 //lets just say for now that legitimate threshold is 10 (ofcourse we can change this value)

Dictionary *brute_dict; // dictionary for brute force attack

//broadcast code------------
#define PORT 8080
#define MAX 80
#define SA struct sockaddr

int ip_id = 0;   //To keep count of different IP's, yes it starts with zero
time_t now_time; //To check the current time
int cryto_id = 0;

struct sockaddr_in source, dest;
char buff[MAX] = "";
char block_cmd[200] = "echo ubuntu@123 | sudo -S iptables -A INPUT -s "; //ubuntu@123 is my system password should change based on the system
int sockfd, connfd;
int broadcast;

struct sockaddr_in servaddr, cli;

struct dictionary
{
    char *ip_adddress;
    unsigned int frequency;
    time_t seconds;
} dict[20]; // If we want more than 20 IP's to monitor then just change the dict[20]

void initialize_data_structures()
{
    for (int i = 0; i < 20; i++)
    {
        dict[i].frequency = 0;
        dict[i].seconds = 0;
    }
}

void store_and_compare(unsigned char *source_address, Dictionary *dicti)
{
    if (!dict_has(dicti, source_address))
    {
        dict_add(dicti, source_address, ip_id++);
        time(&dict[ip_id - 1].seconds); //here we registering the time of IP when it came
        dict[ip_id - 1].frequency++;
    }
    else
    {
        int loc_id;
        loc_id = dict_get(dicti, source_address);
        dict[loc_id].frequency++;
        time(&now_time);

        // printf("time it took %ld and freq is %d \n",(now_time - dict[loc_id].seconds), dict[loc_id].frequency);
        if ((now_time - dict[loc_id].seconds) < 2)
        { //This condition checks the previous ip time and current time of IP
            if (dict[loc_id].frequency > dos_threshold)
            {                                                                                                //if the time is < 2 seconds and frequency of IP is greater than threshold then flag the IP
                printf("ALERT DOS  its freq  %d  and  ip is  %s\n", dict[loc_id].frequency, source_address); //Im just alerting here actually we should block that IP
                
                strcpy(buff,source_address);
				sendto(sockfd, buff, strlen(buff) + 1, 0, (SA *)&servaddr, sizeof(servaddr)); ///-----broadcstes attack ip  -----
                
                //--------------------------------blocking -------------------------------
                strcat (block_cmd, buff);
				strcat(block_cmd, " -j DROP");
				int status = system(block_cmd);
				strcpy(block_cmd,"echo ubuntu@123 | sudo -S iptables -A INPUT -s ");
            }
        }
        else
        {                                //if the time is > 2 seconds
            time(&dict[loc_id].seconds); //we have to register the latest time of IP
            dict[loc_id].frequency = 0;  //we have to set the frequency of IP to zero
        }
    }
}

void print_ip_header(unsigned char *Buffer, int Size, Dictionary *dicti)
{
    unsigned short iphdrlen;

    struct iphdr *iph = (struct iphdr *)Buffer;
    iphdrlen = iph->ihl * 4;

    memset(&source, 0, sizeof(source));
    source.sin_addr.s_addr = iph->saddr;
    char *str = inet_ntoa(source.sin_addr);

    memset(&dest, 0, sizeof(dest));
    dest.sin_addr.s_addr = iph->daddr;

    store_and_compare(str, dicti);
}

// ------------------------------  CryptoJack  ---------------------------
void *cryptoJack(void *vagrp)
{
    FILE *fp;
    float cpuPercent;
    unsigned int pid;
    char procName[1000];

    char cmd[10000] = {0};
    char base[1000] = "/usr/bin/bash /home/karthik/processKill.sh ";

    sprintf(cmd, "%s", base);
    
    system("ps -eo pid,comm,%cpu --sort=-%cpu | head -11 | tail -10 > /home/karthik/processDetails.log");

    fp = fopen("/home/karthik/processDetails.log", "r");

    while (fscanf(fp, "%u %s %f", &pid, procName, &cpuPercent) != EOF)
    {
        if (cpuPercent > 90.0)
        {
            sprintf(cmd, "%s %u %s", cmd, pid, procName);
            system(cmd);
            strcpy(cmd, "usr/bin/bash /home/karthik/processKill.sh ");
        }
    }
}

// ----------------------------  Denial Of Service  ------------------
void *denialOfService(void *vargp)
{
    struct sockaddr saddr;
    int saddr_size;

    initialize_data_structures();

    Dictionary *dicti = dict_new();

    int fd = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP);
    unsigned char *buf = (unsigned char *)malloc(65536);
    int data_size;

    while (1)
    {
        saddr_size = sizeof(saddr);
        if (data_size = recvfrom(fd, buf, 65536, 0, &saddr, &saddr_size) > 0)
        {
            printf("Raw socket created\n");
            print_ip_header(buf, sizeof(buf), dicti);
        }
        else
            printf("Raw socket failed\n");
    }
}

// --------------------------  Brute Force  ----------------
void *bruteforce(void *vargp)
{
    int count;
    char ip[16], substring[16];

    char *command = "awk -f codes.awk /var/log/auth.log | grep -o \'[0-9]\\+\\.[0-9]\\+\\.[0-9]\\+\\.[0-9]\\+ [A-Z][a-z]\\+ [0-9]\\+\' | sort -r | uniq -c > output.log";
    system(command);
    system("cat output.log | sed 's/^[ \t]*//' > logfile.log");

    FILE *fp = fopen("logfile.log", "r");

    if (fp != NULL)
    {
	    while (fscanf(fp, "%d %s", &count, substring) != EOF)
		{
		    if (count > 8)
		    {
		        if(!dict_has(brute_dict, substring))
		        {
		            dict_add(brute_dict, substring, cryto_id++); // add ip to brute force dictionary
		            strcpy(buff, substring);
		            sendto(sockfd, buff, strlen(buff) + 1, 0, (SA *)&servaddr, sizeof(servaddr)); // broadcast code

		            //this code is for blocking ip-------
		            strcat(block_cmd, buff);
		            strcat(block_cmd, " -j DROP");
		            int status = system(block_cmd);
		            strcpy(block_cmd, "echo ubuntu@123 | sudo -S iptables -A INPUT -s ");
		            //blocking ip-----     

		            printf("%s blocked\n", substring);
		        }
		    }
	    }
    }
}

int main()
{
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1)
    {
        printf("Socket creation unsuccessful..\n");
        exit(0);
    }

    printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));

    broadcast = 1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(broadcast)) < 0)
    {
        printf("Error in setting Broadcast option..\n");
        // close(sockfd);
    }

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("192.168.43.255"); // this ip is lan's broadcast ip.....
    servaddr.sin_port = htons(PORT);

    brute_dict = dict_new();

    pthread_t thread1, thread2, thread3;


    pthread_create(&thread2, NULL, denialOfService, NULL);

    while(1) {
        pthread_create(&thread1, NULL, cryptoJack, NULL);
        sleep(2);
        pthread_create(&thread3, NULL, bruteforce, NULL);
        sleep(2);
    }
    return 0;
}
