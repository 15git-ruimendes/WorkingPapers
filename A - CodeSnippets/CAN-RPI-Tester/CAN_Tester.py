# Rui Mendes
# Jan 28 2023

# Please run the following command before starting testing program
# sudo ip link set can0 up type can bitrate 500000

import can
import os



# Testing Functions 

def GeneralCardTester():
    return

def TASCardTester():
    return

def SACCardTester():
    return

def DIGCCardTester():
    return

def IISCardTester():
    return

def MasterTester():
    return 

def COMTester():
    return

# Main Function

if __name__ == '__main__':

    #CAN Bus Bitrate Set
    bitRate = int(input("Insert the configured CAN Bus bitrate: "))

    # CAN Bus Creation
    try:
        CAN = can.Bus(interface='socketcan',channel='can0',bitrate = bitRate)
        print(f"CAN Client created: {CAN.channel_info}")
    except:
        print("CAN bus not connected. Please check CAN bus status and retry.")


