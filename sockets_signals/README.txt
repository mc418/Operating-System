Mengchen Yang
my1489

gcc -o server server.c
./server 3000             

(Pass the port number (2000 to 65535) in as an argument when run the server)

gcc -o client client.c
./client 127.0.0.1 3000  		

(Pass the server ip address and port number in as two arguments when run the client)