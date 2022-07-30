#ifndef GIT_GIT_PROTOTYPES_H
#define GIT_GIT_PROTOTYPES_H

#include <stdio.h>
#include "stdlib.h"
#include "string.h"
#include "json-c/json.h"
#include "getopt.h"
//for working with files in system
#include "dirent.h"
#include "time.h"


//---------------------------- json --------------------------------

static json_object *get_json_file(char *name_file, char *type);
static json_object *get_files_from_directory(char *path);

//---------------------- working with args -------------------------

int read_args(int argc, char *argv[]);
json_object *get_argv (int argc, char **argv);

//---------------------------- checking ----------------------------

static int check_file_name_in_array(json_object *object, char *file_name);
static int check_file_in_dict(char *source, char *file_name);
static int check_file_in_json(char *source, char *file_name);
static char *check_compare(FILE *curr_file, FILE *track_file);

//----------------------------- compare ----------------------------

static char *get_compare(FILE *curr_file, FILE *track_file);
char *compare_files(char *file_name, char *type);

//------------------------------ status ----------------------------

static void get_status();
static void show_untracked_files();
static void show_files_in_stage();
static void show_tracking_files();

//-------------------------------- add -----------------------------

static void add_files(char *name);

//------------------------------- commit ---------------------------

static void create_commit(char *message);
static void *add_pm_string(char *data, char *curr_string, int line, char *type);
static void file_copy(char *file_name);
static void remove_from_tracking(json_object *obj, char *file_name);
static void add_to_tracking(json_object *obj, char *file_name);
static void state_changing(json_object *obj, char *file_name);

//--------------------------------- log ----------------------------

static void show_log();


#endif //GIT_GIT_PROTOTYPES_H
