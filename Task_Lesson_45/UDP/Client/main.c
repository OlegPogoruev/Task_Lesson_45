
#include "main.h"


#include <netpacket/packet.h>


#include <linux/if_ether.h>     /* The L2 protocols */ 




#include <sys/ioctl.h>
#include <net/if.h>  






void get_input_string(char *str_in);
unsigned short Checksum_IP_Header(char *data, unsigned int len);

int main(void)
{
//	struct sockaddr_in sock_client;		// initilaize struct of socket
	struct sockaddr_ll sock_client;		// initilaize struct of socket
	int fd;					// deskriptor socket
	int slen;				// len structure socket
	int if_count;
	char buf[BUFLEN];
	char temp_buf[BUFLEN];
	int recv_len;				// len received mesage
	
	union{unsigned char bait[2]; unsigned short word;}temp;  //Data union
	
	
        struct ifconf ifc;
        struct ifreq ifr[10];
        char ArIP_LOCAL_MASHIN[10][16];
        char IP_LOCAL_MASHIN[16];
        memset(&ifc, 0, sizeof(struct ifconf));
        if ( (fd=socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL))) == -1){perror("can't socket");return -1;}
        
       ifc.ifc_len = 10 * sizeof(struct ifreq);
       ifc.ifc_buf = (char *)ifr;
 
       /* SIOCGIFCONF is IP specific. see netdevice(7) */
       ioctl(fd, SIOCGIFCONF, (char *)&ifc);
       
       if_count = ifc.ifc_len / (sizeof(struct ifreq));
       for (unsigned int i = 0; i < if_count; i++) {
           printf("Interface %s : ", ifr[i].ifr_name);    
           inet_ntop(AF_INET, 
           &(((struct sockaddr_in *)&(ifr[i].ifr_addr))->sin_addr),
           ArIP_LOCAL_MASHIN[i], INET_ADDRSTRLEN);
           printf("%s\n", ArIP_LOCAL_MASHIN[i]);
//           printf("\n");
       }
    
	
	/////////////////////////////////////////////////////
	// zero out the structure
	memset((char *) &sock_client, 0, sizeof(sock_client));
	slen = sizeof(sock_client);				// len structure socket
	// init structure
	sock_client.sll_family = AF_PACKET;	//protocol family
	sock_client.sll_halen = 0x06;		//leng mac
	sock_client.sll_addr[0] = DS_MAC_ADDR0;		//mac addres
	sock_client.sll_addr[1] = DS_MAC_ADDR1;		//mac addres
	sock_client.sll_addr[2] = DS_MAC_ADDR2;		//mac addres
	sock_client.sll_addr[3] = DS_MAC_ADDR3;		//mac addres
	sock_client.sll_addr[4] = DS_MAC_ADDR4;		//mac addres
	sock_client.sll_addr[5] = DS_MAC_ADDR5;		//mac addres
	
	sock_client.sll_ifindex=ifr[1].ifr_ifindex;
	
	for(unsigned char i = 0; i < 20; ++i)
	{IP_LOCAL_MASHIN[i] = ArIP_LOCAL_MASHIN[1][i];}

	/////////////////////////////////////////////////////

	while(1)
	{
		printf("Enter message : ");
		get_input_string(&buf[ETHERNET_OFFSET + 28]);
		
		buf[0] = DS_MAC_ADDR0;  // mac addres destination 0
		buf[1] = DS_MAC_ADDR1;  // mac addres destination 1
		buf[2] = DS_MAC_ADDR2;  // mac addres destination 2
		buf[3] = DS_MAC_ADDR3;  // mac addres destination 3
		buf[4] = DS_MAC_ADDR4;  // mac addres destination 4
		buf[5] = DS_MAC_ADDR5;  // mac addres destination 5
		
		buf[6] = SR_MAC_ADDR0;  // mac addres sourse 0
		buf[7] = SR_MAC_ADDR1;  // mac addres sourse 1
		buf[8] = SR_MAC_ADDR2;  // mac addres sourse 2
		buf[9] = SR_MAC_ADDR3;  // mac addres sourse 3
		buf[10] = SR_MAC_ADDR4;  // mac addres sourse 4
		buf[11] = SR_MAC_ADDR5;  // mac addres sourse 5
		
		temp.word = ETHERNET_TYPE;    // ETHERNET_TYPE
		buf[12] = temp.bait[1];
		buf[13] = temp.bait[0];
		
		//////////////
		// IP Header//
		/////////////////////////////////////////////////////////////
		buf[ETHERNET_OFFSET + 0] = HEADER_IP_VERSIA_IP | HEADER_IP_LENGHT_IP_HEADER; //VERSIA IPv4 + LENGHT_IP_HEADER 
		buf[ETHERNET_OFFSET + 1] = HEADER_IP_TYPE_SERVICE;// Type of Service
		
		
		
		temp.word = 4*HEADER_IP_LENGHT_IP_HEADER + 8 + strlen(&buf[(ETHERNET_OFFSET + 28)]);    // Total Length
		buf[ETHERNET_OFFSET + 2] = temp.bait[1];
		buf[ETHERNET_OFFSET + 3] = temp.bait[0];
		
		temp.word = HEADER_IP_IDENTIFICATION;    // Identification
		buf[ETHERNET_OFFSET + 4] = temp.bait[1];
		buf[ETHERNET_OFFSET + 5] = temp.bait[0];
		
		temp.word = HEADER_IP_FLAGS_FR_OFFSET;    // Flags and Fragment Offset
		buf[ETHERNET_OFFSET + 6] = temp.bait[1];
		buf[ETHERNET_OFFSET + 7] = temp.bait[0];
		
		buf[ETHERNET_OFFSET + 8] = HEADER_IP_TIME_TO_LIVE;	// Time to Live
		buf[ETHERNET_OFFSET + 9] = HEADER_IP_TYPE_PROTOCOL;	// Type Protocol Up Level (UDP = 0x11)
		
		temp.word = Checksum_IP_Header((buf + ETHERNET_OFFSET), 4*HEADER_IP_LENGHT_IP_HEADER);
		buf[ETHERNET_OFFSET + 10] = temp.bait[1];	// Header Checksum
		buf[ETHERNET_OFFSET + 11] = temp.bait[0];	// Header Checksum
		
		//Source Address
//		buf[ETHERNET_OFFSET + 12] = 0x00;	// Source Address
//		buf[ETHERNET_OFFSET + 13] = 0x00;	// Source Address
//		buf[ETHERNET_OFFSET + 14] = 0x00;	// Source Address
//		buf[ETHERNET_OFFSET + 15] = 0x00;	// Source Address
		if (inet_aton(IP_LOCAL_MASHIN , (struct in_addr *)&buf[ETHERNET_OFFSET + 12]) == 0){fprintf(stderr, "inet_aton() failed\n");return -1;}
		
		//Destination Address
//		buf[ETHERNET_OFFSET + 16] = 0x00;	// Destination Address
//		buf[ETHERNET_OFFSET + 17] = 0x00;	// Destination Address
//		buf[ETHERNET_OFFSET + 18] = 0x00;	// Destination Address
//		buf[ETHERNET_OFFSET + 19] = 0x00;	// Destination Address
		if (inet_aton(SERVER , (struct in_addr *)&buf[ETHERNET_OFFSET + 16]) == 0){fprintf(stderr, "inet_aton() failed\n");return -1;}
		
		//////////////////////////////////////////////////////////////////////////////////////////
		
		
		temp.word = SOURCE_PORT;
		buf[ETHERNET_OFFSET + 4*HEADER_IP_LENGHT_IP_HEADER + 0] = temp.bait[1];
		buf[ETHERNET_OFFSET + 4*HEADER_IP_LENGHT_IP_HEADER + 1] = temp.bait[0];
		temp.word = DESTINATION_PORT;
		buf[ETHERNET_OFFSET + 4*HEADER_IP_LENGHT_IP_HEADER + 2] = temp.bait[1];
		buf[ETHERNET_OFFSET + 4*HEADER_IP_LENGHT_IP_HEADER + 3] = temp.bait[0];
		temp.word = 8 + strlen(&buf[ETHERNET_OFFSET + 28]);
		buf[ETHERNET_OFFSET + 4*HEADER_IP_LENGHT_IP_HEADER + 4] = temp.bait[1];
		buf[ETHERNET_OFFSET + 4*HEADER_IP_LENGHT_IP_HEADER + 5] = temp.bait[0];
		buf[ETHERNET_OFFSET + 4*HEADER_IP_LENGHT_IP_HEADER + 6] = 0;
		buf[ETHERNET_OFFSET + 4*HEADER_IP_LENGHT_IP_HEADER + 7] = 0;
		
		temp.word = /**/ETHERNET_OFFSET + 4*HEADER_IP_LENGHT_IP_HEADER + temp.word;
		//send the message
		if (sendto(fd, buf, temp.word , 0 , (struct sockaddr*)&sock_client,sizeof(sock_client))==-1){perror("can't sendto");return -1;}//receive a reply and print it
		strncpy (temp_buf, &buf[42],BUFLEN - 42);
		
		
		while(1)
		{
		    memset(buf,'\0', BUFLEN);//clear the buffer by filling null, it might have previously received data
		    //try to receive some data, this is a blocking call
		    if ((recv_len = recvfrom(fd, buf, BUFLEN, 0, (struct sockaddr *) &sock_client, &slen)) == -1){perror("can't recvfrom()");return -1;}
		    buf[recv_len] = 0;
		    
		    if((ETHERNET_OFFSET + 28 - 1) < recv_len)
		    {
		        if((SR_MAC_ADDR0 == (unsigned char)buf[0]) && (SR_MAC_ADDR1 == (unsigned char)buf[1]) && (SR_MAC_ADDR2 == (unsigned char)buf[2]) && (SR_MAC_ADDR3 == (unsigned char)buf[3]) && \
		            (SR_MAC_ADDR4 == (unsigned char)buf[4]) && (SR_MAC_ADDR5 == (unsigned char)buf[5]) && (DS_MAC_ADDR0 == (unsigned char)buf[6]) && (DS_MAC_ADDR1 == (unsigned char)buf[7]) && \
		                (DS_MAC_ADDR2 == (unsigned char)buf[8]) && (DS_MAC_ADDR3 == (unsigned char)buf[9]) && (DS_MAC_ADDR4 == (unsigned char)buf[10]) && (DS_MAC_ADDR5 == (unsigned char)buf[11]))  //makadress
		        {
		            char temp_str_1[4], temp_str_2[4];
		            unsigned int k = ETHERNET_OFFSET + 12;
		            if (inet_aton(IP_LOCAL_MASHIN , (struct in_addr *)temp_str_1) == 0){fprintf(stderr, "inet_aton() failed\n");return -1;}
		            if (inet_aton(SERVER , (struct in_addr *)temp_str_2) == 0){fprintf(stderr, "inet_aton() failed\n");return -1;}
		            if((temp_str_2[0] == buf[k + 0]) && (temp_str_2[1] == buf[k + 1]) && (temp_str_2[2] == buf[k + 2]) && (temp_str_2[3] == buf[k + 3]) && \
		                (temp_str_1[0] == buf[k + 4]) && (temp_str_1[1] == buf[k + 5]) && (temp_str_1[2] == buf[k + 6]) && (temp_str_1[3] == buf[k + 7]))
		            {
		                break;
		            }
		        }
		    }
		    
		
		}
		
//		printf("Len: %d\n" , recv_len);
		printf("Data: %s\n" , &buf[ETHERNET_OFFSET + 28]);
		
		if (strcmp (temp_buf, "exit")==0){break;}
	}

	close(fd);
	return 0;
}



void get_input_string(char *str_in)
{
    int ch = 0;
    int temp_count = 0;
    while ((ch = getchar()) != '\n')
    {
        (*(str_in + temp_count)) = ch;
        ++temp_count;
        if(temp_count > (BUFLEN - 2 ))
        {
//          (*(str_in + temp_count)) = '\n';
//          ++temp_count;
          break;
        }
    }
//    (*(str_in + temp_count)) = '\n';
    (*(str_in + temp_count)) = 0;    
}

//ut16 Checksum_IP_Header(ut8 *data, ut32 len)
unsigned short Checksum_IP_Header(char *data, unsigned int len)
{
	unsigned int count;
	union{unsigned char Bait[4]; unsigned short Word[2]; unsigned int Full;}temp = {0, 0, 0 ,0};  //temp value
	unsigned int Checksum = 0;
	
	for(count = 0; count < len; count = count + 2)
	{
		temp.Bait[1] = (*(data + count));
		temp.Bait[0] = (*(data + count + 1));
		Checksum = Checksum + temp.Full;
	}
	
	//padding with the most significant bits
	temp.Full = Checksum;
	if(0 != temp.Word[1])
	{
		Checksum = Checksum &0x0000FFFF;
		Checksum = Checksum + temp.Word[1];
	}
	//see if we need to add more, if necessary, then maximum 1
	temp.Full = Checksum;
	if(0 != temp.Word[1])
	{
		Checksum = Checksum &0x0000FFFF;
//		Checksum = Checksum + temp.Word[1];
		++Checksum;
	}
	Checksum = (~Checksum) & 0x0000FFFF;
	return Checksum;
}


