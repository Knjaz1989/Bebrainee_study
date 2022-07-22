#include "server_prototypes.h"


json_object *get_argv (int argc, char **argv) {
    json_object *dict = json_object_new_object();
    struct option long_options[] = {
            {"port", 1, NULL, 'p'},
            {"host", 1, NULL, 'h'},
            {0, 0, 0, 0}
    };
    opterr = 0; // not to get errors if arg doesn't exist
    int opt;
    int option_index; // buffer gor option index
    while ((opt = getopt_long(argc, argv, "", long_options, &option_index)) != -1) {
        if (opt == 63 || opt == 58) {

        } else if (*optarg == '-' || *optarg == '\0') {
            opt = ':';
        }
        switch (opt) {
            case '?':
                puts("Uknown option or argument has no value");
                return NULL;
            case 'p':
                json_object_object_add(dict, "port", json_object_new_string(optarg));
                break;
            case 'h':
                json_object_object_add(dict, "host", json_object_new_string(optarg));
                break;
        }
    }
    json_object_to_file("file.json", dict);
    char *collect[] = {"port", "host"};
    for (int index = 0; index < 2; index++) {
        json_object *object = json_object_object_get(dict, collect[index]);
        if (object == NULL) {
            printf("You have missed '--%s' argument\n", collect[index]);
            return NULL;
        }
    }
    return dict;
}


json_object *args(int argc,char *argv[]) {
    if (argc < 3 || strcmp(argv[1], "start") != 0) {
        puts("Usage: start --host=<...> --port=<...>");
        return NULL;
    }
    json_object *object;
    if ((object = get_argv(argc, argv)) == NULL) {
        return NULL;
    }
    return object;
}


void add_client(int *clients, int client_socket) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i] == 0) {
            clients[i] = client_socket;
            break;
        }
    }
}


void remove_client(int *clients, int client_sock) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i] == client_sock) {
            clients[i] = 0;
            break;
        }
    }
}


void sendtoall(int *clients, char *msg, int curr){
    for(int i = 0; i < MAX_CLIENTS; i++) {
        if(clients[i] != curr && clients[i] != 0) {
            send(clients[i],msg,strlen(msg),0);
        }
    }
}


void *recvmg(void *args) {
    t_args *_args = (t_args *)args;
    int sock = _args->client_sock;
    int *clients = _args->clients;
    char msg[500] = {0};
    while(recv(sock,msg,500,0)) {
        if (strstr(msg, ": exit\n")) {
            break;
        }
        sendtoall(clients, msg,sock);
        bzero(msg, sizeof(msg));
    }

    remove_client(clients, sock);
    close(sock);
}
