//
// Created by igor on 26.06.22.
//

#ifndef USER_AUTHORIZATION_PROTOTYPES_H
#define USER_AUTHORIZATION_PROTOTYPES_H

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


static Node *node_constructor(char *key, char *value);
NodeList *nodeList_constructor ();
static void add_node(NodeList *nodeList, Node *node);
static void delete_nodeList (NodeList *nodeList);
static NodeList *check_create_user_argv(int argc, char **argv);
static NodeList *check_get_user_argv(int argc, char **argv);

void add_user_to_file(NodeList *nodeList);
void read_user_from_file(NodeList *nodeList);

static char *hash_function(const char *str);
static int check_password (char *password);
static int check_file ();
static void create_file ();

#endif //USER_AUTHORIZATION_PROTOTYPES_H
