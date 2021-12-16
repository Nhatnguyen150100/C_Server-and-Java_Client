CFLAGS = -c -Wall
CC = gcc
LIBS =  -lm 

all: account_mysql 

# server: serverSocket.c
# 	${CC} -pthread serverSocket.c $(mysql_config --cflags) -o serverSocket $(mysql_config --libs)

account_mysql.o: account_mysql.c
	${CC} ${CFLAGS} account_mysql.c

clean:
	rm -f *.o *~