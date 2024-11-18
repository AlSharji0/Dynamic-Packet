#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <string.h>
#include "packet.h"
#pragma comment(lib, "Ws2_32.lib")

#define PACKETID 54321
#define SOURCEIP 0xFFFF
#define DESTIP 0xFFFF

//Flags
#define SYN 1
#define FIN 2
#define RSTack 
#define RSTsyn 4
#define SYNfin 5

//Fragment
#define statelessTEST 0
#define statefulTEST 1

uint16_t checksum(const uint16_t* buffer, const int len) {
    uint32_t sum;
    uint16_t result;

    for (int i = 0; i < len / sizeof(uint16_t); i++) sum += buffer[i];

    if (len % 2 == 1) sum += *(unsigned char*)buffer[len / sizeof(uint16_t)];

    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);

    result = ~sum;
    return result;
}

pIPhdr ipFormat(pIPhdr IPpacket, const unsigned int* format) {
    // 0: ttl, 1: fragment offset

    IPpacket->ttl = format[0];
    switch (format[1])
    {
    case statelessTEST:
        IPpacket->flags = 1;
        IPpacket->fragment_off = htons(1480);
        break;

    case statefulTEST:
        IPpacket->flags = 0;
        IPpacket->fragment_off = htons(0);
        break;
    }

    IPpacket->fragment_off = htons(form

    IPpacket->srcIP = htonl(SOURCEIP);
    IPpacket->destIP = htonl(DESTIP);
    IPpacket->version = 4;
    IPpacket->ihl = 5;
    IPpacket->id = ntohs(PACKETID);
    IPpacket->protocol = 6;
    IPpacket->total_len = htons(sizeof(IPhdr) / sizeof(uint8_t));
    IPpacket->tos = 0;

    IPpacket->checksum = 0;
    IPpacket->checksum = checksum((const uint16_t*)IPpacket, sizeof(IPhdr));
}

pTCPhdr tcpFormat(pTCPhdr TCP, const unsigned int* format) {
    // 2: flags, 3: dest ports, 4: sequence number

    switch (format[2])
    {
    case SYN:
        
    case FIN:

    case RSTack:

    case RSTsyn:

    case SYNfin:

    }

}

int main()
{
    
}
