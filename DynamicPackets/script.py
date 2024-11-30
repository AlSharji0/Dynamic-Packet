from scapy.all import sniff, TCP, IP
import socket

TARGET_IP = "35.180.139.74"
TARGET_PORT = 80
sequences = list(range(5))

def packet_sequence_sniff(packet):
    if packet[IP].src == TARGET_IP and packet[TCP].sport == TARGET_PORT:
        sequences[3] = packet[TCP].seq
        sequences[4] = packet[TCP].ack
        pass

try:
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client_socket.connect((TARGET_IP, TARGET_PORT))
    print("Connection established...")

    sniff(filter=f"tcp and host {TARGET_IP} and port {TARGET_PORT}", prn=packet_sequence_sniff, count=1)
    print(f"tcp and host {TARGET_IP} and port {TARGET_PORT}")
    print(sequences)

except Exception as e:
    print(f"Failed to initiate connection: {e}")
