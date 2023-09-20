
#include <stdio.h>
#include <string.h>

#include <unistd.h>

#include <arpa/inet.h>
#include <sys/socket.h>

//#define IP_LOCAL_MASHIN "192.168.192.128"
//#define SERVER "127.0.0.1"
#define SERVER "192.168.1.44"
#define BUFLEN 512	//Max length of buffer
#define DESTINATION_PORT 8888	//The port on which to send data
#define SOURCE_PORT 56657	//The port on which to send data


#define HEADER_IP_ETHERNET_TYPE 0x0800	// IPv4 Ethernet Type
#define HEADER_IP_VERSIA_IP 0x40	// VERSIA IPv4
#define HEADER_IP_LENGHT_IP_HEADER 0x05	// LENGHT_IP_HEADER 
#define HEADER_IP_TYPE_SERVICE 0x00	// Type of Service

#define HEADER_IP_IDENTIFICATION 0x0000	    // Identification
#define HEADER_IP_FLAGS_FR_OFFSET 0x0000    // Flags and Fragment Offset
#define HEADER_IP_TIME_TO_LIVE 0xFF	    // Time to Live
#define HEADER_IP_TYPE_PROTOCOL 17	    // Type Protocol Up Level (UDP = 0x11)




#define ETHERNET_OFFSET    14

/* MAC ADDRESS DESTINATION*/
/*
//00:23:81:20:02:b3
#define DS_MAC_ADDR0    0x00
#define DS_MAC_ADDR1    0x23
#define DS_MAC_ADDR2    0x81
#define DS_MAC_ADDR3    0x20
#define DS_MAC_ADDR4    0x02
#define DS_MAC_ADDR5    0xB3
*/
//00:23:81:20:02:b3
#define DS_MAC_ADDR0    0x00
#define DS_MAC_ADDR1    0x50
#define DS_MAC_ADDR2    0x56
#define DS_MAC_ADDR3    0xF1
#define DS_MAC_ADDR4    0x04
#define DS_MAC_ADDR5    0x3C




/* MAC ADDRESS SOURSE*/
//00:0c:29:fa:8d:0a
#define SR_MAC_ADDR0    0x00
#define SR_MAC_ADDR1    0x0C
#define SR_MAC_ADDR2    0x29
#define SR_MAC_ADDR3    0xFA
#define SR_MAC_ADDR4    0x8D
#define SR_MAC_ADDR5    0x0A

#define ETHERNET_TYPE    0x0800



