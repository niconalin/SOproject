#include <arpa/inet.h>  // htons() and inet_addr()
#include <errno.h>
#include <netinet/in.h> // struct sockaddr_in
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "common.h"
#include "database.h"
#include "linked_list.h"
#include "user_list.h"

//client-side registration routine
int client_registration(int socket_desc)
{
    if (DEBUG)
        fprintf(stderr, "REGISTRATION STARTED\n");
    int ret, recv_bytes, option = REGISTRATION, response = NOT_READY, registration_result = FAILED;

    //request the registration
    while ((ret = send(socket_desc, &option, sizeof(int *), 0)) < 0)
    {
        if (errno == EINTR)
            continue;
        handle_error("Cannot write to socket");
    }
    if (DEBUG)
        fprintf(stderr, "REGISTRATION REQUESTED\n");
    //response from server
    while ((recv_bytes = recv(socket_desc, &response, sizeof(int *), 0)) < 0)
    {
        if (errno == EINTR)
            continue;
        handle_error("Cannot read from socket");
    }
    if (response == NOT_READY)
        handle_error_en(-1, "Cannot registrate");
    if (DEBUG)
        fprintf(stderr, "STATUS RECEIVED\n");
    char username[MAX_LENGTH_USERNAME];
    char password[MAX_LENGTH_PASSWORD];

    //TODO ask for credentials and check if valid

    //read username from input
    printf("\n#### REGISTRATION ####\n\nUsername: ");
    if (fgets(username, sizeof(username), stdin) != (char *)username)
    {
        handle_error("Error while reading username from stdin");
    }
    //read password from input
    printf("\nPassword: ");
    if (fgets(password, sizeof(password), stdin) != (char *)password)
    {
        handle_error("Error while reading password from stdin");
    }

    //remove \n from strings
    int us_len = strlen(username);
    username[--us_len] = '\0';
    int pw_len = strlen(password);
    password[--pw_len] = '\0';

    //send username to server
    while ((ret = send(socket_desc, username, us_len, 0)) < 0) {
        if (errno == EINTR) continue;
        handle_error("Cannot write on socket");
    }
    if (DEBUG) fprintf(stderr, "USERNAME SENT\n");

    //send password to server
    while ((ret = send(socket_desc, password, pw_len, 0)) < 0) {
        if (errno == EINTR) continue;
        handle_error("Cannot write on socket");
    }
    if (DEBUG) fprintf(stderr, "PASSWORD SENT\n");

    //wait for registration result from server
    while ((recv_bytes = recv(socket_desc, &registration_result, sizeof(int*), 0)) < 0) {
        if (errno == EINTR) continue;
        handle_error("Cannot read from socket");
    }
    if (DEBUG) fprintf(stderr, "REGISTRATION RESULT RECEIVED: %d\n", registration_result);
    return registration_result;
}

//server-side registration routine
int server_registration(int socket_desc, ListHead *userHead) {
    if (DEBUG) fprintf(stderr, "REGISTRATION STARTED\n");
    int ret, recv_bytes, status = NOT_READY, registration_result = FAILED;
    char buf[BUFFER_LENGTH];
    char username[MAX_LENGTH_USERNAME];
    char password[MAX_LENGTH_PASSWORD];

    //tell the client that server is ready for handle registration request
    status = READY;
    while ((ret = send(socket_desc, &status, sizeof(int*), 0)) < 0) {
        if (errno == EINTR) continue;
        handle_error("Cannot write on socket");
    }
    if (DEBUG) fprintf(stderr, "STATUS SENT\n");

    //receive username from client
    while ((recv_bytes = recv(socket_desc, buf, sizeof(buf), 0)) < 0)
    {
        if (errno == EINTR)
            continue;
        handle_error("Cannot read from socket");
    }
    if (recv_bytes == 0) {
        return registration_result;
    }
    strcpy(username, buf);
    memset(buf, 0, sizeof(buf));
    if (DEBUG) fprintf(stderr, "USERNAME RECEIVED\n");

    //receive password from client
    while ((recv_bytes = recv(socket_desc, buf, sizeof(buf), 0)) < 0)
    {
        if (errno == EINTR)
            continue;
        handle_error("Cannot read from socket");
    }
    if (recv_bytes == 0) {
        return registration_result;
    }
    strcpy(password, buf);
    memset(buf, 0, sizeof(buf));
    if (DEBUG) fprintf(stderr, "PASSWORD RECEIVED\n");

    //check if username already exsists
    UserListItem* record = UserList_find(userHead, username);
    if (DEBUG) fprintf(stderr, "USERNAME CHECKED\n");
    if (record) registration_result = ALREADY_REGISTERED;
    else {
        record = UserList_create_user(userHead, username, password);
        database_add_user((ListItem*)record);
        registration_result = REGISTRATION_SUCCESS;
    }

    //send response back to client
    while ((ret = send(socket_desc, &registration_result, sizeof(int*), 0)) < 0) {
        if (errno == EINTR) continue;
        handle_error("Cannot write on socket");
    }
    if (DEBUG) fprintf(stderr, "RESPONSE SENT BACK\n");

    memset(username, 0, MAX_LENGTH_USERNAME);
    memset(password, 0, MAX_LENGTH_PASSWORD);
    return registration_result;
}