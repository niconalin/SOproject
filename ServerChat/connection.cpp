#include <arpa/inet.h>  // htons() and inet_addr()
#include <errno.h>
#include <netinet/in.h> // struct sockaddr_in
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include "common.h"
#include "connection.h"

typedef struct handler_args_s
{
    int socketDesc;
    struct sockaddr_in *client_addr;
} handler_args_t;

Connection::Connection(){
    running = false;
}

int Connection::get_socketDesc(){
    return socketDesc;
}

void connection_handler(int socketDesc, struct sockaddr_in *client_addr)
{
    int ret;

    if (DEBUG) fprintf(stderr, "%d\n", client_addr->sin_port);

    // close socket
    ret = close(socketDesc);
    if (ret)
        handle_error("Cannot close socket for incoming connection");
}

// Wrapper function that take as input handler_args_t struct and then call
// connection_handler.
void *thread_connection_handler(void *arg)
{
    handler_args_t *args = (handler_args_t *)arg;
    int socketDesc = args->socketDesc;
    struct sockaddr_in *client_addr = args->client_addr;
    connection_handler(socketDesc, client_addr);
    free(args->client_addr);
    free(args);
    pthread_exit(NULL);
}

void Connection::start_server()
{
    int ret;

    socketDesc = 0;

    struct sockaddr_in server_addr;

    // initialize socket for listening
    socketDesc = socket(AF_INET, SOCK_STREAM, 0);
    if (socketDesc < 0)
        handle_error("Could not create socket");

    server_addr.sin_addr.s_addr = INADDR_ANY; // we want to accept connections from any interface
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT); // don't forget about network byte order!

    // We enable SO_REUSEADDR to quickly restart our server after a crash
    int reuseaddr_opt = 1;
    ret = setsockopt(socketDesc, SOL_SOCKET, SO_REUSEADDR, &reuseaddr_opt, sizeof(reuseaddr_opt));
    if (ret)
        handle_error("Cannot set SO_REUSEADDR option");

    // We enable SO_REUSEPORT to quickly restart our server after a crash
    int reuseport_opt = 1;
    ret = setsockopt(socketDesc, SOL_SOCKET, SO_REUSEPORT, &reuseport_opt, sizeof(reuseport_opt));
    if (ret)
        handle_error("Cannot set SO_REUSEPORT option");

    // bind address to socket
    ret = bind(socketDesc, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_in));
    if (ret)
        handle_error("Cannot bind address to socket");

    // start listening
    ret = listen(socketDesc, MAX_CONN_QUEUE);
    if (ret)
        handle_error("Cannot listen on socket");

    // loop to manage incoming connections spawning handler threads
    int sockaddr_len = sizeof(struct sockaddr_in);
    running = true;
    while (running)
    {
        // we dynamically allocate a fresh client_addr object at each
        // loop iteration and we initialize it to zero
        struct sockaddr_in *client_addr = (struct sockaddr_in *)calloc(1, sizeof(struct sockaddr_in));

        // accept incoming connection
        int client_desc = accept(socketDesc, (struct sockaddr *)client_addr, (socklen_t *)&sockaddr_len);

        if (client_desc == -1 && errno == EINTR)
            continue; // check for interruption by signals
        if (client_desc < 0)
            handle_error("Cannot open socket for incoming connection");

        if (DEBUG)
            fprintf(stderr, "Incoming connection accepted...\n");

        pthread_t thread;

        // prepare arguments for the new thread
        handler_args_t *thread_args = (handler_args_t*)malloc(sizeof(handler_args_t));
        thread_args->socketDesc = client_desc;
        thread_args->client_addr = client_addr;
        ret = pthread_create(&thread, NULL, thread_connection_handler, (void *)thread_args);
        if (ret)
            handle_error_en(ret, "Could not create a new thread");

        if (DEBUG)
            fprintf(stderr, "New thread created to handle the request!\n");

        ret = pthread_detach(thread); // I won't phtread_join() on this thread
        if (ret)
            handle_error_en(ret, "Could not detach the thread");
    }

    ret = close(socketDesc);
    if (ret)
        handle_error("Could not close socket");
}

void Connection::stop_server(){
    running = false;
}
