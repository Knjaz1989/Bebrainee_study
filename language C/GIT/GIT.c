#include "GIT_functions.c"


int main(int argc, char **argv) {
    //get options
    int check = read_args(argc, argv);
    if (check) {
        return EXIT_FAILURE;
    }
//    json_object *object_file = json_object_object_get(arguments, "file");
//    json_object *object_out = json_object_object_get(arguments, "out");
//    json_object *object_method = json_object_object_get(arguments, "method");
//    const char * name_file = json_object_get_string(object_file);
//    const char * name_out_file = json_object_get_string(object_out);
//    const char * method = json_object_get_string(object_method);

    return EXIT_SUCCESS;
}
