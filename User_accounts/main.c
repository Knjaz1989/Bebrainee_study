#include "functions.c"





int main(int argc, char **argv) {
//    mkdir("Accounts", 0777);
    if (strcmp(argv[1], "create-user") == 0 && argc == 7) {
        char *shortopts = "login:password:name:surname:age:";
        int opt = getopt_long_only(argc, argv, shortopts, );





        add_user_to_file(argv);
    }
    else if (strcmp(argv[1], "read-user") == 0) {
        read_user_from_file("star", "111");
    }
    return 0;
}
