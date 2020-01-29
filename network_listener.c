#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<netinet/ip.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/ip_icmp.h>

#define dos_threshold 30

struct dictionary
{
	char* ip_adddress;
	unsigned int frequency;
}dict[20];

struct sockaddr_in source,dest;

//used to determine end
void initialize_data_structures()
{
	for(int i =0;i<20;i++)
			dict[i].frequency=0;
}

//function in psuedo stage
void store_and_compare(unsigned char *source_address)
{	
	int count = 0;
	int i = 0;
	int flag = 0; /*to determine if we already have the ip enterd in the dict*/
	printf("dbg 1");
	while(dict[i].frequency>0)
	{
		if(strcmp(dict[i].ip_adddress,source_address))
			{dict[i].frequency++;flag=1;continue;}
		i++;
		
	}
	if(flag==0)
	{
		strcpy(dict[i].ip_adddress, source_address);
		dict[i].frequency=1;
	}	

	if(count>dos_threshold)
		printf("Alert, DOS %s\n",source_address);
    
    //inter_process_comms()
}

void print_ip_header(unsigned char* Buffer, int Size)
{
	unsigned short iphdrlen;
		
	struct iphdr *iph = (struct iphdr *)Buffer;
	iphdrlen =iph->ihl*4;
	
	memset(&source, 0, sizeof(source));
	source.sin_addr.s_addr = iph->saddr;
	char *str = inet_ntoa(source.sin_addr);
	
	memset(&dest, 0, sizeof(dest));
	dest.sin_addr.s_addr = iph->daddr;

  printf("\n");
	printf("   |-Source IP        : %s\n",str);
	
	//uncomment when funcction finishes psuedo
	store_and_compare(str);
}


int main()
{
	struct sockaddr saddr;
	int saddr_size;


	initialize_data_structures();
	//only need to listen ports that we suspect for DOS(n00b)
	int fd = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP); //icmp listener

	//int fd1 = socket(PF_INET, SOCK_RAW, IPPROTO_TCP); //tcp listener

	//int fd2 = socket(PF_INET, SOCK_RAW, IPPROTO_UDP);	//udp listener

	unsigned char *buf = (unsigned char *)malloc(65536);
	int data_size; //might need 2 more vars for the other sockets..not sure

	while(1)
	{
		saddr_size = sizeof(saddr);
		if(data_size=recvfrom(fd,buf,65536,0,&saddr, &saddr_size)>0)
			{print_ip_header(buf,sizeof(buf));}
	}
  
	return 0;
}
