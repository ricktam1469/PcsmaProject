import serial
import time
import os
 
port = serial.Serial("/dev/ttyAMA0", baudrate=9600, timeout=3.0)

t_end = time.time() + 20  # Run for 10 second
file = open("pollution.txt",'w')   # Trying to create a new file or open 
while time.time() < t_end:
 rcv = port.readline()
 print("Received Pollution Data(PPM): " + rcv)
 try:
        file.write(rcv);
        

 except:
        print('Something went wrong! Can\'t tell what?')
        sys.exit(0) # quit Python


#os.system("scp -i /home/pi/ricktam_aws.pem pollution.txt ec2-user@ec2-52-35-198-120.us-west-2.compute.amazonaws.com:/home/ec2-user/") 
 
