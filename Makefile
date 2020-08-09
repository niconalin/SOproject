CC = gcc -Wall -g
LIBRARIES = -lpthread
COMMON = chat.h chat.c chat_list.h chat_list.c common.h database.c database.h linked_list.c linked_list.h login.c login.h registration.c registration.h user_list.c user_list.h
COMMON_C = chat.c chat_list.c database.c linked_list.c login.c registration.c user_list.c

all: client server

client: client.c $(COMMON)
	$(CC) -o client client.c $(COMMON_C)

server: server.c common.h $(COMMON)
	$(CC) -o server server.c $(COMMON_C) $(LIBRARIES)

.PHONY: clean

clean:
	rm -f client server
