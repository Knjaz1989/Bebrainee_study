#include "GIT_prototypes.h"


int read_args(int argc, char *argv[]) {
    if (argc == 2 && strcmp(argv[1], "log") == 0) {
        show_log();
    } else if (argc == 2 && strcmp(argv[1], "status") == 0) {
        get_status();
    } else if (argc >= 3) {
        if (argc <= 4 && strcmp(argv[1], "commit") == 0) {
            json_object *tmp = get_argv (argc, argv);
            if (tmp) {
                json_object *obj_message = json_object_object_get(tmp, "message");
                const char *str_message = json_object_get_string(obj_message);
                create_commit((char *)str_message);
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


static json_object *get_json_file(char *name_file, char *type) {
    json_object *object = json_object_from_file(name_file);
    if (object) {
        return object;
    }
    if (strcmp(type, "dict") == 0) {
        object = json_object_new_object();
    } else if (strcmp(type, "array") == 0) {
        object = json_object_new_array();
    } else {
        return NULL;
    }
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


static int check_file_in_dict(char *source, char *file_name) {
    json_object *dict = get_json_file(source, "dict");
    json_object_object_get(dict, file_name);
}


static int check_file_in_json(char *source, char *file_name) {
    json_object *array = get_json_file(source, "array");
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


static char *check_compare(FILE *curr_file, FILE *track_file) {
    char *status = 0;
    char curr_string[300] = {0};
    char tr_string[300] = {0};
    while (1) {
        fgets(curr_string, 300, curr_file);
        fgets(tr_string, 300, track_file);
        if (strcmp(curr_string, tr_string) != 0) {
            status = "1";
            break;
        }
        if (strcmp(curr_string, "\0") == 0 && strcmp(tr_string, "\0") == 0) {
            break;
        }
        bzero(curr_string, 300);
        bzero(tr_string, 300);
    }
    fclose(curr_file);
    fclose(track_file);
    return status;
}


static char *get_compare(FILE *curr_file, FILE *track_file) {
    char *data = malloc(sizeof(char) * 1024);
    bzero(data, 1);
    char *curr_string = malloc(sizeof(char) * 300);
    char *tr_string = malloc(sizeof(char) * 300);

    int line = 1;
    while (1) {
        if (fgets(curr_string, 300, curr_file) == NULL) {
            curr_string = 0;
        }
        if (fgets(tr_string, 300, track_file) == NULL) {
            tr_string = 0;
        }
        if (!curr_string && !tr_string) {
            break;
        }
        if (curr_string && !tr_string) {
            add_pm_string(data, curr_string, line, "+");
        } else if (!curr_string) {
            add_pm_string(data, tr_string, line, "-");
        } else {
            add_pm_string(data, tr_string, line, "-");
            add_pm_string(data, curr_string, line, "+");
        }
        line++;
    }
    free(curr_string);
    free(tr_string);
    fclose(curr_file);
    fclose(track_file);
    return data;
}


char *compare_files(char *file_name, char *type) {
    char link[] = ".tracking_files/";
    char f_name[sizeof(link) + sizeof(file_name)];
    sprintf(f_name, "%s%s", link, file_name);
    FILE *curr_file = fopen(file_name, "r");
    FILE *track_file = fopen(f_name, "r");
    if (strcmp(type, "check") == 0) {
        return check_compare(curr_file, track_file);
    } else if (strcmp(type, "get") == 0) {
        return get_compare(curr_file, track_file);
    }
    fclose(curr_file);
    fclose(track_file);
    return NULL;
}

//------------------------------------- add -------------------------------------------------

static void add_files(char *name) {
    json_object *dir_files = get_files_from_directory(".");
    if (check_file_in_dict(".add.json", name)) {
        puts("This file has already added");
        return;
    }
    int check_dir = check_file_name_in_array(dir_files,name);
    int check_tracking = check_file_in_json(".tracking.json", name);
    if (!check_dir && !check_tracking) {
        puts("There is no such file in the working directory and in tracking files");
        return;
    }
    json_object *dict = get_json_file(".add.json", "dict");
    json_object *state;
    if(check_dir && check_tracking) {
        if (compare_files(name, "check")) {
            state = json_object_new_string("changed");
        } else {
            puts("This file has no changes");
            return;
        }
    } else if (check_dir && !check_tracking) {
        state = json_object_new_string("new");
    } else if (!check_dir && check_tracking) {
        state = json_object_new_string("removed");
    }
    json_object_object_add(dict, name, state);
    json_object_to_file(".add.json", dict);
    puts("File was added");
}

//------------------------------------- commit ----------------------------------------------

static void file_copy(char *file_name) {
    char rem[100];
    sprintf(rem, "cp ./%s ./.tracking_files/", file_name);
    strcat(rem, file_name);
    system(rem);
}


static void *add_pm_string(char *data, char *curr_string, int line, char *type) {
    size_t len = strlen(curr_string);
    if (curr_string[len - 1] != '\n') {
        curr_string[len] = '\n';
        curr_string[len + 1] = '\0';
    }
    char tmp[200];
    if (strcmp(type, "+") == 0) {
        sprintf(tmp, "\x1b[32m+ %d: %s\x1b[0m", line, curr_string);
    } else if (strcmp(type, "-") == 0) {
        sprintf(tmp, "\x1b[31m- %d: %s\x1b[0m", line, curr_string);
    }
    strcat(data, tmp);
    bzero(curr_string, 300);
}


static void remove_from_tracking(json_object *obj, char *file_name) {
    char rem[100] = "rm ./.tracking_files/";
    strcat(rem, file_name);
    system(rem);
    json_object *tr_array = get_json_file(".tracking", "array");
    unsigned int tr_array_length = json_object_array_length(tr_array);
    for (int i = 0; i < tr_array_length; ++i) {
        json_object *array_item = json_object_array_get_idx(tr_array, i);
        const char *tr_array_string = json_object_get_string(array_item);
        if (strcmp(tr_array_string, file_name) == 0) {
            json_object_array_del_idx(tr_array, i, 1);
            break;
        }
    }
    json_object *info = json_object_new_string("File was deleted\n");
    json_object_object_add(obj, "changes", info);
}


static void add_to_tracking(json_object *obj, char *file_name) {
    //copy file
    file_copy(file_name);

    //add to tracking
    json_object *array = get_json_file(".tracking.json", "array");
    json_object_array_add(array, json_object_new_string(file_name));
    json_object_to_file(".tracking.json", array);

    //add info
    json_object *info = json_object_new_string("File was created\n");
    json_object_object_add(obj, "changes", info);
}


static void state_changing(json_object *obj, char *file_name) {
    char *data = compare_files(file_name, "get");
    file_copy(file_name);
    json_object_object_add(obj, "changes", json_object_new_string(data));
    free(data);
}


static void create_commit(char *message) {
    json_object *add = get_json_file(".add.json", "dict");
    unsigned int add_length = json_object_object_length(add);
    json_object *commits_array = get_json_file(".commits_array.json", "array");
    json_object *dict = json_object_new_object();

    //check add
    if (!add_length) {
        puts("Nothing to commit");
        return;
    }

    //get current time
    time_t current_time = time(NULL);
    char *time_str = ctime(&current_time);
    time_str[strlen(time_str) - 1] = '\0';

    //add time
    json_object *time = json_object_new_string(time_str);
    json_object_object_add(dict, "data", time);

    //add commit name
    json_object *commit_name = json_object_new_string(message);
    json_object_object_add(dict, "commit_name", commit_name);

    //iterating files in add
    json_object *array = json_object_new_array();
    struct json_object_iterator iter = json_object_iter_begin(add);
    for (int i = 0; i < add_length; ++i) {
        json_object *tmp_dict = json_object_new_object();
        const char *key = json_object_iter_peek_name(&iter);
        const char *value = json_object_get_string(json_object_iter_peek_value(&iter));

        json_object_object_add(tmp_dict, "file_name", json_object_new_string(key));

        if (strcmp(value, "new") == 0) {
            add_to_tracking(tmp_dict, (char *)key);
        } else if (strcmp(value, "removed") == 0) {
            remove_from_tracking(tmp_dict, (char *)key);
        } else if (strcmp(value, "changed") == 0) {
            state_changing(tmp_dict, (char *)key);
        }
        json_object_array_add(array, tmp_dict);

        json_object_iter_next(&iter);
        json_object_object_del(add, key);
    }
    json_object_to_file(".add.json", add);
    json_object_object_add(dict, "files", array);
    json_object_array_add(commits_array, dict);
    json_object_to_file(".commits_array.json", commits_array);
}

//------------------------------------- status ----------------------------------------------

static void show_untracked_files() {
    json_object *files = get_files_from_directory(".");
    unsigned int files_length = json_object_array_length(files);
    if (!files_length) {
        puts("\n\x1b[33mThere is no files in working directory.\x1b[0m");
        return;
    }
    puts("\x1b[33mFiles in working directory:\x1b[0m");
    for (int i = 0; i < files_length; i++) {
        json_object *item = json_object_array_get_idx(files, i);
        const char *item_name = json_object_get_string(item);
        if (check_file_in_dict(".add.json", (char *)item_name)) {
            continue;
        }
        printf ("\x1b[31m%s\x1b[0m\n", item_name);
    }
}


static void show_tracking_files() {
    json_object *object_array = get_json_file(".tracking.json", "array");
    unsigned int length = json_object_array_length(object_array);
    if (!length) {
        puts("\n\x1b[33mThere is no tracking files.\x1b[0m");
        return;
    }
    puts("\n\x1b[33mTracking files:\x1b[0m");
    for (int i = 0; i < length; ++i) {
        json_object *item = json_object_array_get_idx(object_array, i);
        const char *item_name = json_object_get_string(item);
        if (check_file_in_dict(".add.json", (char *)item_name)) {
            continue;
        }
        printf ("\x1b[31m%s\x1b[0m\n", item_name);
    }
}


static void show_files_in_stage() {
    json_object *object = get_json_file(".add.json", "dict");
    unsigned int length = json_object_object_length(object);
    if (!length) {
        puts("\n\x1b[33mThere is no file in stage.\x1b[0m");
        return;
    }
    puts("\n\x1b[33mFiles in stage:\x1b[0m");
    struct json_object_iterator iter = json_object_iter_begin(object);
    for (int i = 0; i < length; i++) {
        const char *key = json_object_iter_peek_name(&iter);
        const char *value = json_object_get_string(json_object_iter_peek_value(&iter));
        printf("\x1b[32m%s: %s\x1b[0m\n", key, value);
        json_object_iter_next(&iter);
    }
}


static void get_status() {
    show_untracked_files();
    show_tracking_files();
    show_files_in_stage();
}

//------------------------------------- log ----------------------------------------------

static void show_log() {
    json_object *log = get_json_file(".commits_array.json", "array");
    unsigned int log_length = json_object_array_length(log);
    if (!log_length) {
        puts("\x1b[33mThere is no logs\x1b[0m");
        return;
    }
    for (int index = (int )log_length -1; index >= 0; --index) {
        json_object *dict = json_object_array_get_idx(log, index);
        struct json_object_iterator iter = json_object_iter_begin(dict);
        for (int i = 0; i < 3; ++i) {
            const char *key = json_object_iter_peek_name(&iter);
            if (i != 2) {
                const char *value = json_object_get_string(json_object_iter_peek_value(&iter));
                printf("\x1b[33m%s\x1b[0m: %s\n", key, value);
            } else {
                json_object *inner_array = json_object_iter_peek_value(&iter);
                unsigned int inner_array_length = json_object_array_length(inner_array);
                for (int j = 0; j < inner_array_length; ++j) {
                    json_object *inner_dict = json_object_array_get_idx(inner_array, j);
                    struct json_object_iterator inner_iter = json_object_iter_begin(inner_dict);
                    for (int k = 0; k < 2; ++k) {
                        const char *inner_key = json_object_iter_peek_name(&inner_iter);
                        const char *inner_value = json_object_get_string(json_object_iter_peek_value(&inner_iter));
                        if (k == 0) {
                            printf("\x1b[33m%s\x1b[0m: %s\n", inner_key, inner_value);
                        } else {
                            printf("\x1b[33m%s\x1b[0m:\n%s\n", inner_key, inner_value);
                        }
                        json_object_iter_next(&inner_iter);
                    }
                }
            }
            json_object_iter_next(&iter);
        }
    }
}
