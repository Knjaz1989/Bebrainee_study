#include "functions.c"





int main(int argc, char **argv) {
//    mkdir("Accounts", 0777);
    NodeList *list;
    if (strcmp(argv[1], "create-user") == 0 && (list = check_create_user_argv(argc, argv))) {
        add_user_to_file(list);
    }
    else if (strcmp(argv[1], "read-user") == 0 && (list = check_get_user_argv(argc, argv))) {
        read_user_from_file(list);
    }
    else if (strcmp(argv[1], "send-message") == 0 && (list = check_send_message_argv(argc, argv))) {
        send_message (list);
    }
    return 0;
}
