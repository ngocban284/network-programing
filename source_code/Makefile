CFLAGS = -c -Wall 
CC = gcc -g
LIBS =  -lm 

all: server client

server: file.o fileRes.o  user.o folder.o space.o showData.o menu.o server.o 
	${CC} server.o file.o fileRes.o  user.o folder.o space.o showData.o menu.o -o server -lmysqlclient -lpthread
client: file.o fileRes.o user.o folder.o space.o showData.o menu.o client.o
	${CC} client.o file.o fileRes.o folder.o user.o space.o showData.o menu.o -o client -lmysqlclient -lpthread
server.o: server.c
	${CC} ${CFLAGS} -lmysqlclient -lpthread server.c 
client.o: client.c
	${CC} ${CFLAGS} -lmysqlclient -lpthread client.c
file.o: file.c
	${CC} ${CFLAGS} -lmysqlclient file.c
fileRes.o: fileRes.c
	${CC} ${CFLAGS} -lmysqlclient fileRes.c
user.o: user.c
	${CC} ${CFLAGS} -lmysqlclient user.c
folder.o: folder.c
	${CC} ${CFLAGS} folder.c
space.o: space.c
	${CC} ${CFLAGS} space.c
showData.o: showData.c
	${CC} ${CFLAGS} showData.c
menu.o: menu.c
	${CC} ${CFLAGS} menu.c
clean:
	rm -f *.o *~ client server


