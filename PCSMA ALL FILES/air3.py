import serial
import time
 
port = serial.Serial("/dev/ttyAMA0", baudrate=9600, timeout=3.0)

t_end = time.time() + 20  # Run for 10 second
 
while time.time() < t_end:
 rcv = port.readline()
 print("received: " + repr(rcv))
 try:
        file = open("pollution.txt",'w')   # Trying to create a new file or open one
        file.write(repr(rcv)+"\n");
        

 except:
        print('Something went wrong! Can\'t tell what?')
        sys.exit(0) # quit Python
 
 
