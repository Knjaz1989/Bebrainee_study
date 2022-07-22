#include "functions.c"





int main(int argc, char **argv) {
//    mkdir("Accounts", 0777);
    NodeList *list;
    if (strcmp(argv[1], "create-user") == 0 && (list = get_argv(argc, argv, "create"))) {
        add_user_to_file(list);
    }
    else if (strcmp(argv[1], "read-user") == 0 && (list = get_argv(argc, argv, "read-user"))) {
        read_user_from_file(list);
    }
    else if (strcmp(argv[1], "send-message") == 0 && (list = get_argv(argc, argv, "send"))) {
        send_message (list);
    }
    else if (strcmp(argv[1], "read-mailbox") == 0 && (list = get_argv(argc, argv, "read-mailbox"))) {
        read_mailbox (list);
    }
    else if (strcmp(argv[1], "read-mail") == 0 && (list = get_argv(argc, argv, "read-mail"))) {
        read_mail (list);
    }
    return 0;
}
