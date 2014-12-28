all	: client.exe server.exe
client.exe : client.o ttt.o
	gcc -o client.exe client.o ttt.o
client.o : client.c ttt.h
	gcc -c client.c

server.exe : server.o ttt.o
	gcc -o server.exe server.o ttt.o
server.o : server.c ttt.h
	gcc -c server.c

ttt.o :  ttt.h
	gcc -c ttt.c

clean	:
	rm -rf ttt.o client.exe client.o server.exe server.o 
