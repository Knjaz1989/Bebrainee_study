#ifndef GIT_GIT_PROTOTIPES_H
#define GIT_GIT_PROTOTIPES_H

#include <stdio.h>
#include "string.h"
#include "json-c/json.h"
#include "getopt.h"
//for working with files in system
#include "dirent.h"
#include "time.h"


static json_object *get_json_file(char *name_file);

int read_args(int argc, char *argv[]);
json_object *get_argv (int argc, char **argv);


static void get_status();
static void add_files(char *name);
static void create_commit(char *message);

static int check_file_name_in_array(json_object *object, char *file_name);
static void show_untracked_files();
static void show_files_in_stage();
static void show_traking_files();

#endif //GIT_GIT_PROTOTIPES_H
