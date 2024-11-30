#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <string.h>
#include "packet.h"
#pragma comment(lib, "Ws2_32.lib")

#define PACKETID 54321
#define SOURCEIP 0x2D21209C
#define DESTIP 0x5993A3CE

//Flags
#define SYN 1
#define FIN 2
#define synACK 3
#define synPSH 4
#define SYNfin 5
#define ACKpsh 6

//Fragment
#define statelessTEST 0
#define statefulTEST 1

//Ports
#define SOURCEPORT 80
#define DESTPORT 80

uint16_t checksum(const uint16_t* buffer, const int len) {
    uint32_t sum = 0;
    uint16_t result;

    for (int i = 0; i < len / sizeof(uint16_t); i++) sum += buffer[i];

    if (len % 2 == 1) sum += *(unsigned char*)buffer[len / sizeof(uint16_t)];

    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);

    result = ~sum;
    return result;
}

void ipFormat(pIPhdr IPpacket, const unsigned short* format) {
    // 0: ttl, 1: fragment offset

    switch (format[0])
    {
    case 0:
        IPpacket->ttl = htons(1);
        break;

    case 1:
        IPpacket->ttl = htons(5);
        break;

    case 2:
        IPpacket->ttl = htons(32);
        break;

    case 3:
        IPpacket->ttl = htons(64);
        break;

    case 4:
        IPpacket->ttl = htons(128);
        break;

    case 5:
        IPpacket->ttl = htons(255);
        break;
    }

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

    IPpacket->srcIP = SOURCEIP;
    IPpacket->destIP = DESTIP;
    IPpacket->version = 4;
    IPpacket->ihl = 5;
    IPpacket->id = ntohs(PACKETID);
    IPpacket->protocol = 6;
    IPpacket->total_len = htons(sizeof(IPhdr) / sizeof(uint8_t));
    IPpacket->tos = 0;

    IPpacket->checksum = 0;
    IPpacket->checksum = checksum((const uint16_t*)IPpacket, sizeof(IPhdr));
}

void tcpFormat(pTCPhdr TCP, const unsigned short* format) {
    // 2: flags, 3: sequence number. 4: acknowlegment number

    switch (format[2])
    {
    case SYN:
        TCP->flags = 0x02;
        break;

    case FIN:
        TCP->flags = 0x01;
        break;

    case synACK:
        TCP->flags = 0x02 | 0x10;
        break;

    case synPSH:
        TCP->flags = 0x02 | 0x08;
        break;

    case SYNfin:
        TCP->flags = 0x02 | 0x01;
        break;

    case ACKpsh:
        TCP->flags = 0x10 | 0x08;
        break;
    }

    TCP->destPORT = htons(DESTPORT);
    TCP->srcPORT = htons(SOURCEPORT);

    TCP->hl = htons(5);
    TCP->reserved = htons(0);
    TCP->window = htons(65535);
    TCP->checksum = htons(checksum((const uint16_t*)TCP, sizeof(TCPhdr)));
    TCP->pUrgent = 0;
}

int main(const unsigned short* format) {
    WSADATA wsa;
    void* packet = malloc(sizeof(IPhdr) + sizeof(TCPhdr));

    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        perror("WSAStartup failed");
        exit(EXIT_FAILURE);
    }

    pIPhdr IPpacket = (pIPhdr)packet;
    pTCPhdr TCPpacket = (pTCPhdr)((char*)IPpacket + sizeof(IPhdr));

    ipFormat(IPpacket, format);
    tcpFormat(TCPpacket, format);

    int sock = socket(AF_INET, SOCK_RAW, IPPROTO_TCP);
    if (sock < 0) {
        ("Socket creation failed");
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    PCSTR SOURCEip = SOURCEIP;

    SOCKADDR_IN dest;
    memset(&dest, 0, sizeof(SOCKADDR_IN));  
    dest.sin_family = AF_INET;
    inet_pton(AF_INET, SOURCEip, &dest.sin_addr);
    dest.sin_port = ntohs(80);
   
    if (sendto(sock, (char*)packet, sizeof(IPhdr) + sizeof(TCPhdr), 0, &dest, sizeof(dest)) < 0) {
        perror("sendto failed");
        closesocket(sock);
        WSACleanup();
        exit(EXIT_FAILURE);
    }

    closesocket(sock);
    WSACleanup();

    return 0;
}
