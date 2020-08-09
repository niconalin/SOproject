#pragma once

// macros for handling errors
#define handle_error_en(en, msg) \
    do                           \
    {                            \
        errno = en;              \
        perror(msg);             \
        exit(EXIT_FAILURE);      \
    } while (0)
#define handle_error(msg)   \
    do                      \
    {                       \
        perror(msg);        \
        exit(EXIT_FAILURE); \
    } while (0)

/* Configuration parameters */
#define DEBUG 1          // display debug messages
#define MAX_CONN_QUEUE 3 // max number of connections the server can queue
#define SERVER_ADDRESS "127.0.0.1"
#define SERVER_PORT 2020
#define BUFFER_LENGTH 512
#define MAX_LENGTH_USERNAME 64
#define MAX_LENGTH_PASSWORD 64
#define MAX_LENGTH_MESSAGE 512
#define USER_SEM "UserListSemaphore"

enum
{
    READY,
    NOT_READY
} STATUS;

enum
{
    LOGIN,
    REGISTRATION,
    CHAT
} OPTION;

enum
{
    ALREADY_REGISTERED,
    NOT_REGISTERED,
    INCORRECT_PASSWORD,
    LOGGED_IN,
    REGISTRATION_SUCCESS,
    FAILED
} OPERATION_RESULT;
