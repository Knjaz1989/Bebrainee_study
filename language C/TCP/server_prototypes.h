#ifndef TCP_NEW_SERVER_PROTOTYPES_H
#define TCP_NEW_SERVER_PROTOTYPES_H

#include <stdio.h>
#include "stdlib.h"
#include "sys/socket.h"
#include "arpa/inet.h"
#include "unistd.h"
#include "string.h"
#include "pthread.h"
#include "json-c/json.h"
#include "getopt.h"

#define MAX_CLIENTS 50


typedef struct {
    int *clients;
    int client_sock;
} t_args;


json_object *get_argv (int argc, char **argv);
json_object *args(int argc,char *argv[]);
void add_client(int *clients, int client_socket);
void remove_client(int *clients, int client_sock);
void sendtoall(int *clients, char *msg, int curr);
void *recvmg(void *args);


#endif //TCP_NEW_SERVER_PROTOTYPES_H
