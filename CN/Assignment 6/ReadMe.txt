TESTING STOP AND WAIT

How to start the server


inside the src folder 
./netster -r 1 -f outSAW -p 8088

r= readmode
1= server
-f = file type
outSAW = output file after build
8088 = port to use


How to start the client
inside the src folder 
./netster -r 1 testfile.txt -p 8088 127.0.0.1


HOW TO TEST IN SERVER

from local login to silo.sice.indiana.edu

from silo ssh into netdev.open.sice.indiana.edu

ssh into server -> ssh server

clone code into client machine and build it using "make"



go into src and run -> test-netster ./netster 

test only in server



FOR CLIENT SET UP
from local login to silo.sice.indiana.edu

from silo ssh into netdev.open.sice.indiana.edu

ssh into client -> ssh client

clone code into client machine and build it using "make"




_____________________________________________________________

FOR TESTING GO BACK N

ALL THE SAME TESTING AS STOP AND WAIT 

Last final test as below 

To test the go back n

test-netster ./netster gobackn.o