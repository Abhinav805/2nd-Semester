Take the latest code 

Go to src folder 

make a file 
touch testfile.txt

nano testfile.txt

This is Assignment 4 (Write something inside this file)


First start the server
Now to send the file 
./netster -f output.txt

Now the sever is running

Now open new terminal

go to client 

./netster -f testfile.txt 127.0.0.1

Now the file is send from client to server 

To check the difference between the file 

diff testfile.txt output.txt

No changes 



__________________________________________________

This is UDP testing 
Go to server

./netster -u -f output_udp.txt

Now go to client 

./netster -u -f testfile.txt 127.0.1
