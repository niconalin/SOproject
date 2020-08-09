#pragma once

//client-side registration routine
int client_registration(int socket_desc);

//server-side registartion routine
int server_registration(int socket_desc, ListHead *userHead);
