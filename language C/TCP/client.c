#include "client_functions.c"


int main(int argc,char **argv) {
    json_object *arguments = args(argc, argv);
    if (!arguments) {
        return EXIT_FAILURE;
    }
    json_object *object_port = json_object_object_get(arguments, "port");
    json_object *object_host = json_object_object_get(arguments, "host");
    json_object *object_login = json_object_object_get(arguments, "login");
    const char *ip = json_object_get_string(object_host);
    int port = atoi(json_object_get_string(object_port));
    const char *login = json_object_get_string(object_login);

    pthread_t recvt;
    char msg[500] = {0};
    int len;
    int sock;
    char send_msg[502] = {0};
    struct sockaddr_in ServerIp;
    sock = socket( AF_INET, SOCK_STREAM,0);
    ServerIp.sin_port = htons(port);
    ServerIp.sin_family= AF_INET;
    ServerIp.sin_addr.s_addr = inet_addr(ip);

    if( (connect( sock ,(struct sockaddr *)&ServerIp,sizeof(ServerIp))) == -1 ) {
        printf("\n connection to socket failed \n");
        return EXIT_FAILURE;
    }

    printf("%s, welcome to the chat\n", login);

    //creating a client thread which is always waiting for a message
    pthread_create(&recvt,NULL,(void *)recvmg,&sock);

    //ready to read a message from console
    while(fgets(msg,500,stdin) > 0) {
        if (msg[0] == 10) {
            printf("message not sent, because it is empty!\n");
            continue;
        }
        if (strcmp(msg, "exit\n") == 0) {
            puts("Goodbye");
            return EXIT_SUCCESS;
        }
        sprintf(send_msg, "%s: %s", login, msg);

        write(sock,send_msg,strlen(send_msg));
        bzero(msg, strlen(msg));
        bzero(send_msg, strlen(send_msg));
    }

    //thread is closed
    pthread_join(recvt,NULL);
    close(sock);
    return 0;
}
