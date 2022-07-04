//
// Created by igor on 26.06.22.
//

#ifndef USER_AUTHORIZATION_PROTOTYPES_H
#define USER_AUTHORIZATION_PROTOTYPES_H

#define ACC_FILE "accounts/accounts.json"

#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include <sys/stat.h> // Для работы с папками
#include "json-c/json.h"
#include "getopt.h"


typedef struct Node {
    char *key;
    char *value;
    struct Node *next;
} Node;

typedef struct NodeList {
    struct Node *head;
    struct Node *last;
    int length;
} NodeList;


static Node *node_constructor (char *key, char *value);
NodeList *nodeList_constructor ();
static void add_node (NodeList *nodeList, Node *node);
static void delete_nodeList (NodeList *nodeList);
NodeList *get_argv(int argc, char **argv, char *type);

void add_user_to_file (NodeList *nodeList);
void read_user_from_file (NodeList *nodeList);
void send_message (NodeList *nodeList);
void read_mailbox (NodeList *nodeList);
void read_mail (NodeList *nodeList);

static char *hash_function (const char *str);
static int check_right_password (char *password);
static int check_file ();
static void create_file ();
static int check_user (NodeList *nodeList);
static char *create_name_file (char *acc_name, char *prefix);
static json_object *check_login (char *user_login);
static void delete_ind(char **argvs, int index);
static int check_argv (NodeList *nodeList, char *type);

#endif //USER_AUTHORIZATION_PROTOTYPES_H
