#include "client_prototypes.h"


json_object *get_argv (int argc, char **argv) {
    json_object *dict = json_object_new_object();
    struct option long_options[] = {
            {"port", 1, NULL, 'p'},
            {"host", 1, NULL, 'h'},
            {"login", 1, NULL, 'l'},
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
            case 'l':
                json_object_object_add(dict, "login", json_object_new_string(optarg));
                break;
        }
    }
    char *collect[] = {"port", "host", "login"};
    for (int index = 0; index < 3; index++) {
        json_object *object = json_object_object_get(dict, collect[index]);
        if (object == NULL) {
            printf("You have missed '--%s' argument\n", collect[index]);
            return NULL;
        }
    }
    return dict;
}


json_object *args(int argc,char *argv[]) {
    if (argc < 1) {
        puts("Usage: --host=<...> --port=<...> --login=<...>");
        return NULL;
    }
    json_object *object;
    if ((object = get_argv(argc, argv)) == NULL) {
        return NULL;
    }
    return object;
}


void *recvmg(void *my_sock) {
    char msg[500] = {0};
    int sock = *((int *)my_sock);
    // client thread always ready to receive message
    while(recv(sock,msg,500,0)) {
        fputs(msg,stdout);
        bzero(msg, sizeof(msg));
    }
}
