#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netinet/ip.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/ip_icmp.h>
#include <time.h>
#include "dictionary.h"

#define dos_threshold 10 //lets just say for now that legitimate threshold is 10 (ofcourse we can this value)

int ip_id = 0; //To keep count of different IP's, yes it starts with zero
time_t now_time; //To check the current time

struct sockaddr_in source, dest;

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
	if(!dict_has(dicti, source_address)){
		dict_add(dicti, source_address, ip_id++);
		time(&dict[ip_id-1].seconds); //here we registering the time of IP when it came
		dict[ip_id-1].frequency++;

	}
	else{
		int loc_id;
		loc_id=dict_get(dicti, source_address);
		dict[loc_id].frequency++;
		time(&now_time);
		
		// printf("time it took %ld and freq is %d \n",(now_time - dict[loc_id].seconds), dict[loc_id].frequency);
		if((now_time - dict[loc_id].seconds) < 2){ //This condition checks the previous ip time and current time of IP
			if(dict[loc_id].frequency > dos_threshold){ //if the time is < 2 seconds and frequency of IP is greater than threshold then flag the IP
				printf("ALERT DOS  its freq  %d  and  ip is  %s\n", dict[loc_id].frequency, source_address); //Im just alerting here actually we should block that IP
			}
		}
		else{  //if the time is > 2 seconds 
			time(&dict[loc_id].seconds); //we have to register the latest time of IP
			dict[loc_id].frequency = 0; //we have to set the frequency of IP to zero
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

int main()
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
			print_ip_header(buf, sizeof(buf), dicti);
		}
	}

	return 0;
}