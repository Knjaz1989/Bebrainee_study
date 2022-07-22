#ifndef TCP_NEW_CLIENT_PROTOTYPES_H
#define TCP_NEW_CLIENT_PROTOTYPES_H

#include <stdio.h>
#include "sys/socket.h"
#include "arpa/inet.h"
#include "unistd.h"
#include "string.h"
#include "pthread.h"
#include "json-c/json.h"
#include "getopt.h"


json_object *get_argv (int argc, char **argv);
json_object *args(int argc,char *argv[]);
void *recvmg(void *my_sock);


#endif //TCP_NEW_CLIENT_PROTOTYPES_H
