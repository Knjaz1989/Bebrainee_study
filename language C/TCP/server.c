#include "server_functions.c"


int main(int argc, char **argv) {
    json_object *arguments = args(argc, argv);
    if (!arguments) {
        return EXIT_FAILURE;
    }
    json_object *object_port = json_object_object_get(arguments, "port");
    json_object *object_host = json_object_object_get(arguments, "host");
    const char *ip = json_object_get_string(object_host);
    int port = atoi(json_object_get_string(object_port));

    struct sockaddr_in ServerIp;
    pthread_t recvt;
    int sock = 0, Client_sock = 0;
    int clients[MAX_CLIENTS] = {0};

    ServerIp.sin_family = AF_INET;
    ServerIp.sin_port = htons(port);
    ServerIp.sin_addr.s_addr = inet_addr(ip);
    sock = socket(AF_INET, SOCK_STREAM, 0);

    if (bind(sock, (struct sockaddr *) &ServerIp, sizeof(ServerIp)) == -1) {
        printf("cannot bind, error!! \n");
        return EXIT_FAILURE;
    }

    if( listen( sock ,20 ) == -1 ) {
        printf("listening failed \n");
        return EXIT_FAILURE;
    }

    printf("Server Started\n");

    while(1){
        if( (Client_sock = accept(sock, (struct sockaddr *)NULL,NULL)) < 0 ) {
            printf("accept failed \n");
            return EXIT_FAILURE;
        }
        add_client(clients, Client_sock);
        t_args *args;
        args->client_sock = Client_sock;
        args->clients = clients;

        // creating a thread for each client
        pthread_create(&recvt,NULL,(void *)recvmg,args);
    }
    return 0;
}
