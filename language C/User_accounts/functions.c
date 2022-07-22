//
// Created by igor on 26.06.22.
//

#include "prototypes.h"


static Node *node_constructor (char *key, char *value) {
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


static void add_node (NodeList *nodeList, Node *node) {
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
    if (strcmp(node->key, "destination") == 0 && nodeList->length - 1 == 3) {
        tmp = nodeList->last;
        nodeList->head->next->next = node;
        node->next = tmp;
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


static void delete_ind(char **argvs, int index) {
    int j = index;
    while (argvs[j] != NULL) {
        argvs[j] = argvs[j + 1];
        j++;
    }
}


static int check_argv (NodeList *nodeList, char *type) {
    char **argvs = (char **)malloc(5 * sizeof(char[15]));
    if (strcmp(type, "create") == 0) {
        argvs[0] = "login";
        argvs[1] = "password";
        argvs[2] = "name";
        argvs[3] = "lastname";
        argvs[4] = "age";
    }
    if (strcmp(type, "read-user") == 0 || strcmp(type, "read-mailbox") == 0) {
        argvs[0] = "login";
        argvs[1] = "password";
    }
    if (strcmp(type, "send") == 0) {
        argvs[0] = "login";
        argvs[1] = "password";
        argvs[2] = "destination";
        argvs[3] = "message";
    }
    if (strcmp(type, "read-mail") == 0) {
        argvs[0] = "login";
        argvs[1] = "password";
        argvs[2] = "message-id";
    }
    Node *tmp = nodeList->head;

    while (tmp) {
        int i = 0;
        while (argvs[i]) {
            if (strcmp(argvs[i], tmp->key) == 0) {
                delete_ind(argvs, i);
                break;
            }
            i++;
        }
        tmp = tmp->next;
    }
    if (strlen(argvs)) {
        free(argvs);
        puts("Wrong arguments");
        return 1;
    }
    free(argvs);
    return 0;
}


NodeList *get_argv (int argc, char **argv, char *type) {
    NodeList *node_list = nodeList_constructor();
    struct option long_options[] = {
            {"login", 1, NULL, 'l'},
            {"password", 1, NULL, 'p'},
            {"name", 1, NULL, 'n'},
            {"lastname", 1, NULL, 's'},
            {"age", 1, NULL, 'a'},
            {"destination", 1, NULL, 'd'},
            {"message", 1, NULL, 'm'},
            {"message-id", 1, NULL, 'i'},
            {0, 0, 0, 0}
    };
    opterr = 0; // not to get errors if arg doesn't exist
    int opt;
    int option_index; // buffer gor option index
    while ((opt = getopt_long(argc, argv, "", long_options, &option_index)) != EOF) {
        switch (opt) {
            case 'l': add_node(node_list, node_constructor("login", optarg)); break;
            case 'p': if (check_right_password(optarg)) { delete_nodeList(node_list); return NULL; }
                add_node(node_list, node_constructor("password", optarg)); break;
            case 'n': add_node(node_list, node_constructor("name", optarg)); break;
            case 's': add_node(node_list, node_constructor("lastname", optarg)); break;
            case 'a': add_node(node_list, node_constructor("age", optarg)); break;
            case 'd': add_node(node_list, node_constructor("destination", optarg)); break;
            case 'm': add_node(node_list, node_constructor("message", optarg)); break;
            case 'i': add_node(node_list, node_constructor("message-id", optarg)); break;
            case '?': printf("%s is invalid", argv[optind - 1]); delete_nodeList(node_list); return NULL;
            case ':': printf("%s has no value", long_options[option_index + 1].name); delete_nodeList(node_list);
                return NULL;
        }
    }
    if (check_argv(node_list, type)) {
        delete_nodeList(node_list);
        return NULL;
    }
    return node_list;
}


static void create_file (char *file_name, char *type) {
    if (strcmp(type, "dict") == 0) {
        json_object *dict = json_object_new_object();
        json_object_to_file(file_name, dict);
    }
    else if (strcmp(type, "list") == 0) {
        json_object *list = json_object_new_array();
        json_object_to_file(file_name, list);
    }
}


static int check_file (char *file_name) {
    if (fopen(file_name, "r")) { // Допустимо ли такое открытие файла?
        return 0;
    }
    return 1;
}


char *hash_function (const char *str) {
    char i[4] = {0};
    char *string = malloc(sizeof(char) * 100);
    memset(string, 0, sizeof(&string));
    for (int j = 0; j < strlen(str); j++) {
        sprintf(i, "%d", str[j]);
        strcat(string, i);
    }
    return string;
}


static int check_right_password (char *password) {
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


static json_object *check_login (char *user_login) {
    json_object *accounts = json_object_from_file(ACC_FILE);
    char *login = user_login;
    json_object *object_login = json_object_object_get(accounts, login);
    if (!object_login) {
        printf("There is no such login '%s' in base. Please, try another one", user_login);
        return NULL;
    }
    return object_login;
}


static int check_user (NodeList *nodeList) {
     // Читаем файл
    json_object *object_login = check_login(nodeList->head->value);
    if (object_login == NULL) {
        return 1;
    }
    json_object *password = json_object_object_get(object_login, "password");
    const char *password_str = json_object_get_string(password);
    char *hash_password = hash_function(nodeList->head->next->value);
    nodeList->head->next->value = hash_password;
    if (strcmp(password_str, hash_password) != 0) {
        puts("Password is wrong");
        free(hash_password);
        return 1;
    }
    free(hash_password);
    return 0;
}


void add_user_to_file (NodeList *nodeList) {
    /* login_node is always first*/
    if (check_file(ACC_FILE)) {
        create_file(ACC_FILE, "dict");
    }
    json_object *dict = json_object_from_file(ACC_FILE); // read dict from file
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
    json_object_to_file(ACC_FILE, dict);
    printf("User with login '%s' was created", login);
    free(nodeList);
    free(hash_password);
}


void read_user_from_file (NodeList *nodeList) {
    if (check_file(ACC_FILE)) {
        create_file(ACC_FILE, "dict");
    }
    if (check_user(nodeList)) {
        return;
    }
    json_object *accounts = json_object_from_file(ACC_FILE);
    char *login = nodeList->head->value;
    json_object *object_login = json_object_object_get(accounts, login);
    json_object *name = json_object_object_get(object_login, "name");
    json_object *surname = json_object_object_get(object_login, "lastname");
    json_object *age = json_object_object_get(object_login, "age");
    const char *name_str = json_object_get_string(name);
    const char *surname_str = json_object_get_string(surname);
    int age_int = json_object_get_int(age);
    printf("name=%s, lastname=%s, age=%d\n", name_str, surname_str, age_int);
    free(nodeList);
}


static char *create_name_file (char *acc_name, char *prefix) {
    char *filename = malloc(sizeof(char) * 70);
    strcat(filename, "accounts/messages/");
    strcat(filename, acc_name);
    strcat(filename, prefix);
    return filename;
}


void send_message (NodeList *nodeList) {
    char *filename = create_name_file(nodeList->head->next->next->value, "_getting.json");
    if (check_user(nodeList)) {
        return;
    }
    char *login = nodeList->head->next->next->value;
    if (check_login(login) == NULL) {
        return;
    }
    if (check_file(filename)) {
        create_file(filename, "list");
    }
    json_object *all_lists = json_object_from_file(filename);
    json_object *new_list = json_object_new_array();
    json_object *from = json_object_new_string(nodeList->head->value);
    json_object_array_add(new_list, from);
    json_object *message = json_object_new_string(nodeList->last->value);
    json_object_array_add(new_list, message);
    json_object_array_add(all_lists, new_list);
    json_object_to_file(filename, all_lists);
    free(filename);
}


void read_mailbox (NodeList *nodeList) {
    char *filename = create_name_file(nodeList->head->value, "_getting.json");
    if (check_user(nodeList)) {
        return;
    }
    json_object *list = json_object_from_file(filename);
    unsigned int count = json_object_array_length(list);
    printf("You have %d unread messages\n", count);
    for (int i = 0; i < count; i++) {
        json_object *object = json_object_array_get_idx(list, i);
        json_object *from = json_object_array_get_idx(object, 0);
        const char *from_str = json_object_get_string(from);
        printf("id=%d, from=%s\n", i + 1, from_str);
    }
    free(filename);
}


void read_mail (NodeList *nodeList) {
    char *filename = create_name_file(nodeList->head->value, "_getting.json");
    if (check_user(nodeList)) {
        return;
    }
    json_object *list = json_object_from_file(filename);
    unsigned int length = json_object_array_length(list);
    if (!length) {
        puts("You have no messages");
        return;
    }
    long int index = strtol(nodeList->last->value, &nodeList->last->value, 10);
    if (index < 1 || index > length) {
        puts("message-id is wrong");
        return;
    }
    json_object *object_list = json_object_array_get_idx(list, index - 1);
    json_object *from = json_object_array_get_idx(object_list, 0);
    const char *from_str = json_object_get_string(from);
    json_object *message = json_object_array_get_idx(object_list, 1);
    const char *message_str = json_object_get_string(message);
    printf("from=%s\nmessage=%s\n", from_str, message_str);
    json_object_array_del_idx(list, index - 1, 1);
    json_object_to_file(filename, list);
    free(filename);
}
