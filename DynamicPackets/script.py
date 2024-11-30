from scapy.all import sniff, TCP, IP
import socket

TARGET_IP = "45.33.32.156"
TARGET_PORT = 80
sequences = []

def packet_sequence_sniff(packet):
    if packet[IP].src == TARGET_IP and packet[TCP].sport == TARGET_PORT:
        sequences.append(([packet[TCP].seq, packet[TCP].ack]))
        pass

try:
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client_socket.connect((TARGET_IP, TARGET_PORT))
    print("Connection established...")

    sniff(filter=f"tcp and host {TARGET_IP} and port {TARGET_PORT}", prn=packet_sequence_sniff, count=1)
    print("Sniffing :) ")
    print(sequences)

except Exception as e:
    print(f"Failed to initiate connection: {e}")