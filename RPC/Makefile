all: client server

msg.h: msg.x
	rpcgen msg.x
	
msg_svc.c msg_clnt.c receive.c: msg.h

client: receive.o msg_clnt.o
	cc receive.c msg_clnt.c -o client

server: msg_proc.o msg_svc.o
	cc msg_proc.c msg_svc.c -o server

.PHONY: clean

clean:
	-rm *.o
	-rm client
	-rm server
	-rm msg.h
	-rm msg_clnt.c
	-rm msg_svc.c
