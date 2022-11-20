import serial 
import csv

PORT = "/dev/ttyS0"
BAUD = 19200

BOOSTER_QC = 0

if __name__ == '__main__':
    print('------- Commmunication Quality Control -------')
    
    BOOSTER_QC = int(input('Please Insert Identifier Number of Booster: '))

    print("Booster No. " + str(BOOSTER_QC) + " Selected")

    print('----------------------------------------------\n')
    print('Communication Parameters:  ')
    print('      - BaudRate: '+ str(BAUD))
    print('      - Port Name: ' + PORT)

    print('')
    