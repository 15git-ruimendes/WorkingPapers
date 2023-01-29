import socket

if __name__ == '__main__':
    msgToSend = "1"
    bytesToSend = str.encode(msgToSend)
    serverADR = ("192.168.1.230",7)
    bufferSize = 1024

    UDPClientSocket = socket.socket(family = socket.AF_INET,type = socket.SOCK_DGRAM)
    UDPClientSocket.sendto(bytesToSend,serverADR)
    msgRx = UDPClientSocket.recvfrom(bufferSize)

    print(msgRx)
