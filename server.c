#include <arpa/inet.h>  // htons() and inet_addr()
#include <errno.h>
#include <netinet/in.h> // struct sockaddr_in
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "chat.h"
#include "common.h"
#include "database.h"
#include "login.h"
#include "registration.h"

typedef struct handler_args_s
{
    int socket_desc;
    struct sockaddr_in *client_addr;
    ListHead *userHead;
} handler_args_t;

void connection_handler(int socket_desc, struct sockaddr_in *client_addr, ListHead *userHead)
{
    int ret, recv_bytes, option;
    ListHead* chatHead = 0;

    while (1)
    {
        while ((recv_bytes = recv(socket_desc, &option, sizeof(int *), 0)) < 0)
        {
            if (errno == EINTR)
                continue;
            handle_error("Cannot read from socket");
        }
        if (recv_bytes == 0)
        {
            break;
        }
        switch (option)
        {
        case LOGIN:
            ret = server_login(socket_desc, userHead, &chatHead);
            switch (ret)
            {
            case FAILED:
                break;
            case NOT_REGISTERED:
                break;
            case INCORRECT_PASSWORD:
                break;
            case LOGGED_IN:
                server_chat(socket_desc, chatHead);
                break;
            default:
                break;
            }
            break;
        case REGISTRATION:
            ret = server_registration(socket_desc, userHead);
            break;
        default:
            break;
        }
    }

    // close socket
    ret = close(socket_desc);
    if (ret)
        handle_error("Cannot close socket for incoming connection");
}

// Wrapper function that take as input handler_args_t struct and then call
// connection_handler.
void *thread_connection_handler(void *arg)
{
    handler_args_t *args = (handler_args_t *)arg;
    int socket_desc = args->socket_desc;
    struct sockaddr_in *client_addr = args->client_addr;
    ListHead *userHead = args->userHead;
    connection_handler(socket_desc, client_addr, userHead);
    // do not forget to free client_addr! by design it belongs to the
    // thread spawned for handling the incoming connection
    free(args->client_addr);
    free(args);
    pthread_exit(NULL);
}

void mthreadServer(int server_desc, ListHead *userHead)
{
    int ret = 0;

    // loop to manage incoming connections spawning handler threads
    int sockaddr_len = sizeof(struct sockaddr_in);
    while (1)
    {
        // we dynamically allocate a fresh client_addr object at each
        // loop iteration and we initialize it to zero
        struct sockaddr_in *client_addr = calloc(1, sizeof(struct sockaddr_in));

        // accept incoming connection
        int client_desc = accept(server_desc, (struct sockaddr *)client_addr, (socklen_t *)&sockaddr_len);

        if (client_desc == -1 && errno == EINTR)
            continue; // check for interruption by signals
        if (client_desc < 0)
            handle_error("Cannot open socket for incoming connection");

        if (DEBUG)
            fprintf(stderr, "Incoming connection accepted...\n");

        pthread_t thread;

        // prepare arguments for the new thread
        handler_args_t *thread_args = malloc(sizeof(handler_args_t));
        thread_args->socket_desc = client_desc;
        thread_args->client_addr = client_addr;
        thread_args->userHead = userHead;
        ret = pthread_create(&thread, NULL, thread_connection_handler, (void *)thread_args);
        if (ret)
            handle_error_en(ret, "Could not create a new thread");

        if (DEBUG)
            fprintf(stderr, "New thread created to handle the request!\n");

        ret = pthread_detach(thread); // I won't phtread_join() on this thread
        if (ret)
            handle_error_en(ret, "Could not detach the thread");
    }
}

//signal handler for CTRL+C
void signal_handler(int sign)
{
    //UserList_free(get_UserListHead());
    //fprintf(stderr, "\nWould you like to exit? Nope!\n");
    //fflush(stdout);
    fprintf(stderr, "\nSIGINT received, exiting...\n");
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
    int ret, socket_desc;

    //catch CTRL+C
    signal(SIGINT, signal_handler);

    //populate database from txt
    ListHead *userHead = database_get_users();
    if (DEBUG)
        UserList_print(userHead);

    //initialize semaphore for writing on database
    sem_unlink(USER_SEM);
    sem_t *user_sem = sem_open(USER_SEM, O_CREAT | O_EXCL, 0600, 1);
    if (user_sem == SEM_FAILED)
        handle_error("Cannot create User Semaphore");

    // some fields are required to be filled with 0
    struct sockaddr_in server_addr ={ 0 };

    // initialize socket for listening
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc < 0)
        handle_error("Could not create socket");

    server_addr.sin_addr.s_addr = INADDR_ANY; // we want to accept connections from any interface
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT); // don't forget about network byte order!

    // We enable SO_REUSEADDR to quickly restart our server after a crash
    int reuseaddr_opt = 1;
    ret = setsockopt(socket_desc, SOL_SOCKET, SO_REUSEADDR, &reuseaddr_opt, sizeof(reuseaddr_opt));
    if (ret)
        handle_error("Cannot set SO_REUSEADDR option");

    // bind address to socket
    ret = bind(socket_desc, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_in));
    if (ret)
        handle_error("Cannot bind address to socket");

    // start listening
    ret = listen(socket_desc, MAX_CONN_QUEUE);
    if (ret)
        handle_error("Cannot listen on socket");

    mthreadServer(socket_desc, userHead);

    exit(EXIT_SUCCESS); // this will never be executed
}
