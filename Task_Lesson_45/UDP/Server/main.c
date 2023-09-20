
#include "main.h"

void revers_string(char *str);

int main(void)
{
	int size;
	struct sockaddr_in sock_serv, sock_client;		// initilaize struct of socket
	int fd;							// deskriptor socket
	int slen;						// len structure socket
	int recv_len;						// len received mesage
	
	char buf[BUFLEN];
	
	//create a UDP socket
	if ((fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1){perror("can't open socket");return(-1);}
	/////////////////////////////////////////////////////
	// zero out the structure
	memset((char *) &sock_serv, 0, sizeof(sock_serv));
	// init structure
	sock_serv.sin_family = AF_INET;
	sock_serv.sin_port = htons(PORT);
	sock_serv.sin_addr.s_addr = htonl(INADDR_ANY);
	/////////////////////////////////////////////////////
	// zero out the structure
	memset((char *) &sock_client, 0, sizeof(sock_serv));
	slen = sizeof(sock_client);				// len structure socket
	/////////////////////////////////////////////////////
	
	
	//bind socket to port
	if( bind(fd , (struct sockaddr*)&sock_serv, sizeof(sock_serv) ) == -1){perror("can't open bind");return(-1);}
	
	printf("Waiting for data...");
//	fflush(stdout);
	
	
	//keep listening for data
	while(1)
	{
		//try to receive some data, this is a blocking call
		if ((recv_len = recvfrom(fd, buf, BUFLEN, 0, (struct sockaddr *) &sock_client, &slen)) == -1){perror("can't recvfrom()");return(-1);}
		buf[recv_len] = 0;
		
		//print details of the client/peer and the data received
		printf("Received packet from %s:%d\n", inet_ntoa(sock_client.sin_addr), ntohs(sock_client.sin_port));
		printf("Data: %s\n" , buf);
		
		revers_string(buf);
		
		//now reply the client with the same data
		if (sendto(fd, buf, recv_len, 0, (struct sockaddr*) &sock_client, slen) == -1){perror("can't sendto()");return(-1);}
		
		if (strcmp (buf, "tixe")==0){break;}
	}

	close(fd);
	return 0;
	
	
}


void revers_string(char *str)
{

	char temp_str[BUFLEN];
	int i = 0;
	int j = 0;
	while(0 != (*(str + i))){++i;}
	while(0 != i)
	{
	    --i;
	    temp_str[j] = (*(str + i));
	    ++j;
	}
	temp_str[j] = 0;
	strncpy (str, temp_str,BUFLEN);
}



