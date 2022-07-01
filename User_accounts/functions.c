//
// Created by igor on 26.06.22.
//

#include "prototypes.h"


static Node *node_constructor(char *key, char *value) {
    Node *node = (Node *) malloc(sizeof(Node));
    node->key = key;
    node->value = value;
    node->next = NULL;
    return node;
}


NodeList *nodeList_constructor () {
    NodeList *nodeList = (NodeList *) malloc(sizeof(NodeList));
    nodeList->head = NULL;
    nodeList->last = NULL;
    nodeList->length = 0;
    return nodeList;
}


static void add_node(NodeList *nodeList, Node *node) {
    Node *tmp;
    nodeList->length++;
    if (nodeList->head == NULL) {
        nodeList->head = node;
        nodeList->last = node;
        return;
    }
    if (
            strcmp(node->key, "login") == 0 ||
            (strcmp(node->key, "password") == 0 && strcmp(nodeList->head->key, "login") != 0)
    ) {
        tmp = nodeList->head;
        node->next = tmp;
        nodeList->head = node;
        return;
    }
    if (strcmp(node->key, "password") == 0 && strcmp(nodeList->head->key, "login") == 0) {
        tmp = nodeList->head;
        node->next = tmp->next;
        tmp->next = node;
        if (nodeList->length == 2) {
            nodeList->last = node;
        }
        return;
    }
    tmp = nodeList->last;
    tmp->next = node;
    nodeList->last = node;
}


static void delete_nodeList (NodeList *nodeList) {
    Node *tmp = nodeList->head;
    while (tmp) {
        nodeList->head = tmp->next;
        free(tmp);
        tmp = nodeList->head;
    }
    free(nodeList);
}


static NodeList *check_create_user_argv(int argc, char **argv) {
    NodeList *node_list = nodeList_constructor();
    struct option long_options[] = {
            {"login", 1, NULL, 'l'},
            {"password", 1, NULL, 'p'},
            {"name", 1, NULL, 'n'},
            {"lastname", 1, NULL, 's'},
            {"age", 1, NULL, 'a'},
            {0, 0, 0, 0}
    };
    opterr = 0; // not to get errors if arg doesn't exist
    int opt;
    int option_index; // buffer gor option index
    while ((opt = getopt_long(argc, argv, "", long_options, &option_index)) != EOF) {
        switch (opt) {
            case 'l': add_node(node_list, node_constructor("login", optarg)); break;
            case 'p': if (check_password(optarg)) { delete_nodeList(node_list); return NULL; }
                add_node(node_list, node_constructor("password", optarg)); break;
            case 'n': add_node(node_list, node_constructor("name", optarg)); break;
            case 's': add_node(node_list, node_constructor("lastname", optarg)); break;
            case 'a': add_node(node_list, node_constructor("age", optarg)); break;
            case '?': printf("%s is invalid", argv[optind - 1]); delete_nodeList(node_list); return NULL;
            case ':': printf("%s has no value", long_options[option_index + 1].name); delete_nodeList(node_list);
                return NULL;
        }
    }
    int count = 5 - node_list->length;
    if (count) {
        printf("You have missed %d arguments", count);
        delete_nodeList(node_list);
        return NULL;
    }
    return node_list;
}


static NodeList *check_get_user_argv(int argc, char **argv) {
    NodeList *node_list = nodeList_constructor();
    struct option long_options[] = {
            {"login", 1, NULL, 'l'},
            {"password", 1, NULL, 'p'},
            {0, 0, 0, 0}
    };
    opterr = 0; // not to get errors if arg doesn't exist
    int opt;
    int option_index; // buffer gor option index
    while ((opt = getopt_long(argc, argv, "", long_options, &option_index)) != EOF) {
        switch (opt) {
            case 'l': add_node(node_list, node_constructor("login", optarg)); break;
            case 'p': add_node(node_list, node_constructor("password", optarg)); break;
            case '?': printf("%s is invalid", argv[optind - 1]); delete_nodeList(node_list); return NULL;
            case ':': printf("%s has no value", long_options[option_index + 1].name); delete_nodeList(node_list);
                return NULL;
        }
    }
    int count = 2 - node_list->length;
    if (count) {
        printf("You have missed %d arguments", count);
        delete_nodeList(node_list);
        return NULL;
    }
    return node_list;
}


static void create_file () {
    json_object *list = json_object_new_object();
    json_object_to_file("accounts.json", list);
}


static int check_file () {
    if (fopen("accounts.json", "r")) { // Допустимо ли такое открытие файла?
        return 0;
    }
    return 1;
}


char *hash_function(const char *str) {
    int i = 0;
    char *string = malloc(sizeof(char) * 40);
    for (int j=0; str[j]; j++)
        i += str[j];
    i = i + 147025836;
    sprintf(string, "%d", i); // convert from int to string
    return string;
}


static int check_password (char *password) {
    unsigned int length = strlen(password);
    if ( length < 8) {
        puts("Password is too short. It can have at least 8 characters.");
        return 1;
    }
    int upper = 0;
    int lower = 0;
    int digits = 0;
    for (int i = 0; i < length; i++) {
        if (password[i] >=48 && password[i] <=57) {
            digits++;
        }
        if (password[i] >=65 && password[i] <=90) {
            upper++;
        }
        if (password[i] >=97 && password[i] <=122) {
            lower++;
        }
    }
    if (!digits) {
        puts("The string must contain one number.");
        return 1;
    }
    if (!upper) {
        puts("The string must contain one uppercase letter");
        return 1;
    }
    if (!lower) {
        puts("The string must contain one lowercase letter");
        return 1;
    }
    return 0;
}


void add_user_to_file(NodeList *nodeList) {
    /* login_node is always first*/
    if (check_file()) {
        create_file();
    }
    json_object *dict = json_object_from_file("accounts.json"); // read dict from file
    char *login = nodeList->head->value;
    json_object *object_login = json_object_object_get(dict, login);
    if (object_login) {
        puts("There is such login in base. Please, try another one");
        return;
    }
    char *hash_password = hash_function(nodeList->head->next->value);
    nodeList->head->next->value = hash_password;
    json_object *new_dict = json_object_new_object(); //create new dict
    Node *node = nodeList->head->next; // start from node number two
    while (node) {
        json_object_object_add(new_dict, node->key, json_object_new_string(node->value));
        node = node->next;
    }
    json_object_object_add(dict, login, new_dict);
    json_object_to_file("accounts.json", dict);
    free(nodeList);
    free(hash_password);
}


void read_user_from_file(NodeList *nodeList) {
    if (check_file()) {
        puts("There is no such user");
        return;
    }
    json_object *accounts = json_object_from_file("accounts.json"); // Читаем файл
    char *login = nodeList->head->key;
    json_object *object_login = json_object_object_get(accounts, login);
    if (!object_login) {
        puts("There is no such login in base. Please, try another one");
        return;
    }
    json_object *password = json_object_object_get(object_login, "password");
    const char *password_str = json_object_get_string(password);
    char *hash_password = hash_function(nodeList->head->next->value);
    nodeList->head->next->value = hash_password;
    if (strcmp(password_str, hash_password) != 0) {
        puts("Password is wrong");
        return;
    }
    json_object *name = json_object_object_get(object_login, "name");
    json_object *surname = json_object_object_get(object_login, "lastname");
    json_object *age = json_object_object_get(object_login, "age");
    const char *name_str = json_object_get_string(name);
    const char *surname_str = json_object_get_string(surname);
    int age_int = json_object_get_int(age);
    printf("name=%s, lastname=%s, age=%d\n", name_str, surname_str, age_int);
    free(nodeList);
    free(hash_password);
}
