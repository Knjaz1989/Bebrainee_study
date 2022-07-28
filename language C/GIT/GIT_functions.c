#include "GIT_prototipes.h"


int read_args(int argc, char *argv[]) {
    if (argc == 2 && strcmp(argv[1], "log") == 0) {
        puts("log");
    } else if (argc == 2 && strcmp(argv[1], "status") == 0) {
        get_status();
    } else if (argc >= 3) {
        if (argc <= 4 && strcmp(argv[1], "commit") == 0) {
            json_object *tmp = get_argv (argc, argv);
            if (tmp) {
                json_object *obj_message = json_object_object_get(tmp, "message");
                const char *str_message = json_object_get_string(obj_message);
                create_commit(str_message);
            }
        } else if (argc == 3 && strcmp(argv[1], "add") == 0) {
            add_files(argv[2]);
        }
    } else {
        puts("Command line is empty or unknown command");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}


json_object *get_argv (int argc, char **argv) {
    json_object *dict = json_object_new_object();
    struct option long_options[] = {
            {"message", 1, NULL, 'm'},
            {0, 0, 0, 0}
    };
    opterr = 0; // not to get errors if arg doesn't exist
    int opt;
    int option_index; // buffer gor option index
    while ((opt = getopt_long(argc, argv, "", long_options, &option_index)) != -1) {
        if (opt != 63 && opt != 58 && (*optarg == '-' || *optarg == '\0')) {
            opt = ':';
        }
        char *value = optarg;
        switch (opt) {
            case 'm':
                json_object_object_add(dict, "message", json_object_new_string(value));
                break;
        }
    }
    char *collect[] = {"message"};
    for (int index = 0; index < 1; index++) {
        json_object *object = json_object_object_get(dict, collect[index]);
        if (object == NULL) {
            printf("You have missed '--%s' option or '--%s' has no argument\n", collect[index], collect[index]);
            return NULL;
        }
    }
    return dict;
}


static json_object *get_json_file(char *name_file) {
    json_object *object = json_object_from_file(name_file);
    if (object) {
        return object;
    }
    object = json_object_new_array();
    json_object_to_file(name_file, object);
    return object;
}


static int check_file_name_in_array(json_object *object, char *file_name) {
    unsigned int ign_length = json_object_array_length(object);
    for (int index = 0; index < ign_length; index++) {
        json_object *file = json_object_array_get_idx(object, index);
        const char *name = json_object_get_string(file);
        if (strcmp(name, file_name) == 0) {
            return 1;
        }
    }
    return 0;
}


static int check_file_in_json(char *source, char *file_name) {
    json_object *array = get_json_file(source);
    if (check_file_name_in_array(array, file_name)) {
        return 1;
    }
    return 0;
}


static json_object *get_files_from_directory(char *path) {
    json_object *files = json_object_new_array();
    DIR *directory = opendir(path);
    struct dirent *ent;
    if (strcmp(path, ".") == 0) {
        json_object *ignore_files = json_object_from_file(".ignore_system_files.json");
        while ((ent = readdir (directory)) != NULL) {
            if (!check_file_name_in_array(ignore_files, ent->d_name)) {
                json_object *value = json_object_new_string(ent->d_name);
                json_object_array_add(files, value);
            }
        }
    } else {
        while ((ent = readdir (directory)) != NULL) {
            json_object *value = json_object_new_string(ent->d_name);
            json_object_array_add(files, value);
        }
    }
    return files;
}

//------------------------------------- add -------------------------------------------------

static void add_files(char *name) {
    if (strcmp(name, "*") == 0) {
        return;
    }
    json_object *dir_files = get_files_from_directory(".");
    if (check_file_in_json(".add.json", name)) {
        puts("This file has already added");
        return;
    }
    if (!check_file_name_in_array(dir_files,name)) {
        puts("There is no such file in the working directory");
        return;
    }
    json_object *str_object = json_object_new_string(name);
    json_object *object_add = get_json_file(".add.json");
    json_object_array_add(object_add, str_object);
    json_object_to_file(".add.json", object_add);
    puts("File was added");
}

//------------------------------------- commit ----------------------------------------------

static void comapare_files(char *file_name) {

}


static void create_commit(char *message) {
    json_object *add = get_json_file(".add.json");
    unsigned int add_length = json_object_array_length(add);
    if (!add_length) {
        puts("Nothing to commit");
        return;
    }
    //get current time
    time_t current_time = time(NULL);
    char *time_str = ctime(&current_time);
    time_str[strlen(time_str) - 1] = '\0';

    json_object *commits_array = get_json_file("./.commits_array.json");
    json_object *dict = json_object_new_object();
    unsigned int com_length = json_object_array_length(commits_array);

    //add time
    json_object *time = json_object_new_string(time_str);
    json_object_object_add(dict, "data", time);

    //add commit name
    json_object *commit_name = json_object_new_string(message);
    json_object_object_add(dict, "commit_name", commit_name);

    //add files
    json_object *files = json_object_new_array();
//    char text[2048] = {0};
//    sprintf(text, "Data: %s", time_str);


//    if (!com_length) {
//        for (int i = 0; i < add_length; i++) {
//            const char *file_name = json_object_get_string(json_object_array_get_idx(add, i));
//        }
//    }
    json_object_object_add(dict, "files", files);
    json_object_array_add(commits_array, dict);
    json_object_to_file("./.commits_array.json", commits_array);
}

//------------------------------------- status ----------------------------------------------

static void show_untracked_files() {
    json_object *files = get_files_from_directory(".");
    unsigned int files_length = json_object_array_length(files);
    puts("\x1b[33mUntracked files:\x1b[0m");
    for (int i = 0; i < files_length; i++) {
        json_object *item = json_object_array_get_idx(files, i);
        const char *item_name = json_object_get_string(item);
        if (check_file_in_json(".add.json", item_name)) {
            continue;
        }
        printf ("\x1b[31m%s\x1b[0m\n", item_name);
    }
}


static void show_files_in_stage() {
    json_object *object = get_json_file(".add.json");
    unsigned int length = json_object_array_length(object);
    json_object *dir_files = get_files_from_directory(".");
    if (!length) {
        puts("\n\x1b[33mThere is no file in stage.\x1b[0m");
        return;
    }
    puts("\n\x1b[33mFiles in stage:\x1b[0m");
    for (int i = 0; i < length; i++) {
        json_object *file = json_object_array_get_idx(object, i);
        const char *name = json_object_get_string(file);
        if (check_file_name_in_array(dir_files, name)) {
            printf("\x1b[32m%s\x1b[0m\n", name);
            continue;
        }
        json_object_array_del_idx(object, i, 1);
        i--;
        length--;
    }
    json_object_to_file(".add.json", object);
}


static void get_status() {
    show_untracked_files();
    show_files_in_stage();
}
