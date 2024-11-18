#ifndef PACKET_H
#define PACKET_H

#include <stdint.h>


#pragma pack(1)

typedef struct _IPHDR_ {
    uint8_t version : 4, ihl : 4, tos;
    uint16_t total_len;
    uint16_t id;
    uint16_t flags : 3, fragment_off : 13;
    uint8_t ttl, protocol;
    uint16_t checksum;
    uint32_t srcIP, destIP;
}IPhdr, *pIPhdr;

typedef struct _TCPHDR_ {
    uint16_t srcPORT, destPORT;
    uint32_t seq, ack_seq;
    uint16_t hl : 4, reserved : 6, flags : 6;
    uint16_t window, checksum, pUrgent;
} TCPhdr, *pTCPhdr;

#pragma pack()

#endif // !1