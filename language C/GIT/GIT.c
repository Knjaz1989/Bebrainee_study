#include "GIT_functions.c"


int main(int argc, char **argv) {
    //get options
    int check = read_args(argc, argv);
    if (check) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
