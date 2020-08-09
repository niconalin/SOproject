#include <arpa/inet.h>  // htons() and inet_addr()
#include <errno.h>
#include <netinet/in.h> // struct sockaddr_in
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "chat_list.h"
#include "common.h"
#include "database.h"
#include "linked_list.h"
#include "user_list.h"

//client-side login routine
int client_login(int socket_desc, char *username[MAX_LENGTH_USERNAME])
{
    if (DEBUG)
        fprintf(stderr, "LOGIN STARTED\n");
    int ret, recv_bytes, option = LOGIN, response = NOT_READY, login_result;

    //request the login
    while ((ret = send(socket_desc, &option, sizeof(int *), 0)) < 0)
    {
        if (errno == EINTR)
            continue;
        handle_error("Cannot write to socket");
    }
    if (DEBUG)
        fprintf(stderr, "LOGIN REQUESTED\n");

    //response from server
    while ((recv_bytes = recv(socket_desc, &response, sizeof(int *), 0)) < 0)
    {
        if (errno == EINTR)
            continue;
        handle_error("Cannot read from socket");
    }
    if (response == NOT_READY)
        handle_error_en(-1, "Cannot log in");
    if (DEBUG)
        fprintf(stderr, "STATUS RECEIVED\n");
    char username_inserted[MAX_LENGTH_USERNAME];
    char password_inserted[MAX_LENGTH_PASSWORD];

    //read username from input
    printf("\n### LOGIN ###\n\nUsername: ");
    if (fgets(username_inserted, sizeof(username_inserted), stdin) != (char *)username_inserted)
    {
        handle_error("Error while reading username from stdin");
    }
    //read password from input
    printf("\nPassword: ");
    if (fgets(password_inserted, sizeof(password_inserted), stdin) != (char *)password_inserted)
    {
        handle_error("Error while reading password from stdin");
    }

    //remove \n from strings
    int us_len = strlen(username_inserted);
    username_inserted[--us_len] = '\0';
    int pw_len = strlen(password_inserted);
    password_inserted[--pw_len] = '\0';

    // send username to server
    while ((ret = send(socket_desc, username_inserted, us_len, 0)) < 0)
    {
        if (errno == EINTR)
            continue;
        handle_error("Cannot write to socket");
    }
    if (DEBUG)
        fprintf(stderr, "USERNAME SENT\n");
    // send password to server
    while ((ret = send(socket_desc, password_inserted, pw_len, 0)) < 0)
    {
        if (errno == EINTR)
            continue;
        handle_error("Cannot write to socket");
    }
    if (DEBUG)
        fprintf(stderr, "PASSWORD SENT\n");

    //wait login result from server
    while ((recv_bytes = recv(socket_desc, &login_result, sizeof(int *), 0)) < 0)
    {
        if (errno == EINTR)
            continue;
        handle_error("Cannot read from socket");
    }
    if (DEBUG)
        fprintf(stderr, "LOGIN RESULT RECEIVED: %d\n", login_result);

    *username = username_inserted;
    return login_result;
}

//server-side login routine
int server_login(int socket_desc, ListHead *userHead, ListHead** chatHead)
{
    if (DEBUG)
        fprintf(stderr, "LOGIN STARTED\n");
    char buf[BUFFER_LENGTH];
    size_t buf_len = sizeof(buf);
    char username[MAX_LENGTH_USERNAME];
    char password[MAX_LENGTH_PASSWORD];
    int ret, recv_bytes, login_result, status = READY;

    //inform the client that we are ready for login
    while ((ret = send(socket_desc, &status, sizeof(int *), 0)) < 0)
    {
        if (errno == EINTR)
            continue;
        handle_error("Cannot write to socket");
    }
    if (DEBUG)
        fprintf(stderr, "STATUS SENT\n");

    //read username from client
    while ((recv_bytes = recv(socket_desc, buf, buf_len, 0)) < 0)
    {
        if (errno == EINTR)
            continue;
        handle_error("Cannot read from socket");
    }
    if (recv_bytes == 0)
    {
        login_result = FAILED;
        return login_result;
    }
    strcpy(username, buf);
    memset(buf, 0, buf_len);
    if (DEBUG)
        fprintf(stderr, "USERNAME RECEIVED\n");

    //read password from client
    while ((recv_bytes = recv(socket_desc, buf, buf_len, 0)) < 0)
    {
        if (errno == EINTR)
            continue;
        handle_error("Cannot read from socket");
    }
    if (recv_bytes == 0)
    {
        login_result = FAILED;
        return login_result;
    }
    strcpy(password, buf);
    memset(buf, 0, buf_len);
    if (DEBUG)
        fprintf(stderr, "PASSWORD RECEIVED\n");

    //check if username exists
    UserListItem *record = UserList_find(userHead, username);
    if (DEBUG)
        fprintf(stderr, "USERNAME CHECKED\n");
    if (!record)
    {
        //set login result
        login_result = NOT_REGISTERED;
    }
    else
    {
        //check if password is correct
        if (strcmp(record->password, password))
        {
            if (DEBUG)
                fprintf(stderr, "PASSWORD CHECKED\n");
            login_result = INCORRECT_PASSWORD;
        }
        else
        {
            if (DEBUG)
                fprintf(stderr, "PASSWORD CHECKED\n");
            *chatHead = database_get_chats(username);
            if (!*chatHead) handle_error("Cannot retrieve chats from database");
            login_result = LOGGED_IN;
        }
    }

    //send response back to client
    while ((ret = send(socket_desc, &login_result, sizeof(int *), 0)) < 0)
    {
        if (errno == EINTR)
            continue;
        handle_error("Cannot write to socket");
    }
    if (DEBUG)
        fprintf(stderr, "RESPONSE SENT BACK\n");
    return login_result;
}