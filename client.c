#include <arpa/inet.h>  // htons() and inet_addr()
#include <errno.h>
#include <netinet/in.h> // struct sockaddr_in
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include "chat.h"
#include "common.h"
#include "login.h"
#include "registration.h"

void welcome(int socket_desc)
{
    int op;
    char tmp[8];
    while (1)
    {

        //welcome message
        printf("\n#### PRIVATE CHAT ####\nPress:\n1) to Login\n2) to Register (if not yet subscribed)\n3) to Quit.\n\nChoice: ");
        if (fgets(tmp, sizeof(tmp), stdin) != (char *)tmp)
            handle_error("Cannot read from input");
        op = atoi(tmp);
        switch (op)
        {
        /*LOGIN*/ case 1:
        {
            char *username[MAX_LENGTH_USERNAME];
            if (client_login(socket_desc, username) == LOGGED_IN)
            {
                client_chat(socket_desc, *username);
                return;
            }
            break;
        }
        /*REGISTER*/ case 2:
        {
            if (client_registration(socket_desc) == REGISTRATION_SUCCESS)
                if (DEBUG)
                    fprintf(stderr, "REGISTERED SUCCESSFULLY!\n");
            break;
        }
        /*QUIT*/ case 3:
        {
            return;
            break;
        }
        default:
            printf("Invalid option!\n");
            break;
        }
    }
}


//main
int main(int argc, char *argv[]) {
    int ret;

    // variables for handling a socket
    int socket_desc;
    struct sockaddr_in server_addr ={ 0 }; // some fields are required to be fill       with 0

    // create a socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc < 0)
        handle_error("Could not create socket");

    // set up parameters for the connection
    server_addr.sin_addr.s_addr = inet_addr(SERVER_ADDRESS);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT); // don't forget about network byte order!

    // initiate a connection on the socket
    ret = connect(socket_desc, (struct sockaddr *)&server_addr, sizeof(struct sockaddr_in));
    if (ret)
        handle_error("Could not create connection");
    if (DEBUG)
        fprintf(stderr, "Connection established!\n");

    // login, register or quit
    welcome(socket_desc);

    // close the socket
    ret = close(socket_desc);
    if (ret)
        handle_error("Cannot close socket");

    if (DEBUG)
        fprintf(stderr, "Exiting...\n");

    exit(EXIT_SUCCESS);
}
