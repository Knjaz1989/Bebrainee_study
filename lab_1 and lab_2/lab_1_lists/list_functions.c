#include "prototypes.h"


DB *create_db() {
    DB *tmp = (DB *)malloc(sizeof(DB));
    tmp->size = 0;
    tmp->head = tmp->tail = NULL;
    return tmp;
}


Node *_create_node(char *value) {
    Node *tmp = (Node *)malloc(sizeof(Node));
    tmp->data = (char *)malloc(sizeof(char) * strlen(value) + 1);
    tmp->data = strcpy(tmp->data, value);
    tmp->next = NULL;
    tmp->prev = NULL;
    return tmp;
}


int get_db_size(DB *DB) {
    return DB->size;
}


char *get_current_node_value(DB *db, int index) {
    if (0 <= index & index < db->size) {
        int middle = db->size / 2;

        if (index <= middle) {
            Node *tmp = db->head;
            for(int i = 0; i < index; i++) {
                tmp = tmp->next;
            }
            return tmp->data;
        }
        else {
            Node *tmp = db->tail;
            for(int i = 0; i < db->size - index - 1; i++) {
                tmp = tmp->prev;
            }
            return tmp->data;
        }
    }
    return "There is no such element";
}


void show_index(DB* db, char *value) {
    Node *tmp = db->head;
    int count = 0;
    while (tmp) {
        if (strcmp(value, tmp->data) == 0) {
            printf("[%d]\n", count);
            return;
        }
        count++;
        tmp = tmp->next;
    }
    printf("There is no such value\n");
}


void delete_current_node(DB *db, int index) {
    if (0 <= index & index < db->size) {

        int middle = db->size / 2;
        Node *tmp;

        if (index <= middle) {
            tmp = db->head;
            for(int i = 0; i < index; i++) {
                tmp = tmp->next;
            }
        }
        else {
            tmp = db->tail;
            for(int i = 0; i < db->size - index - 1; i++) {
                tmp = tmp->prev;
            }
        }

        Node *prev_node = tmp->prev;
        Node *next_node = tmp->next;

        if (prev_node) {
            prev_node->next = next_node;
        } else {
            db->head = next_node;
        }

        if (next_node) {
            next_node->prev = prev_node;
        } else {
            db->tail = prev_node;
        }

        db->size--;
        free(tmp);
        return;
    }
}


void add_node(DB *db, int index, char *value) {
    Node *new_node = _create_node(value);
    if (db->head == NULL & db->tail == NULL) {
        db->head = db->tail = new_node;
        db->size++;
        return;
    }
    if (index <= 0) {
        Node *tmp = db->head;
        new_node->next = tmp;
        tmp->prev = new_node;
        db->head = new_node;
        db->size++;
        return;
    }
    if (index >= db->size - 1) {
        Node *tmp = db->tail;
        tmp->next = new_node;
        new_node->prev = tmp;
        db->tail = new_node;
        db->size++;
        return;
    }
    int middle = db->size / 2;
    Node *current_node;
    if (index <= middle) {
        current_node = db->head;
        for(int i = 0; i < index; i++) {
            current_node = current_node->next;
        }
    } else {
        current_node = db->tail;
        for(int i = 0; i < db->size - index - 1; i++) {
            current_node = current_node->prev;
        }
    }
    Node *prev_node = current_node->prev;
    prev_node->next = new_node;
    new_node->prev = prev_node;
    new_node->next = current_node;
    current_node->prev = new_node;
    db->size++;
}


void print_db(DB *db) {
    if (db->head == NULL) {
        printf("The database is empty");
        return;
    }
    Node *tmp = db->head;
    while (tmp) {
        printf("[%s]\n", (*tmp).data);
        tmp = tmp->next;
    }
}


void add_a_certain_amount_into_db(DB *db, int amount) {
    int size = db->size;
    for (int i = size; i < amount + size; i++) {
        char *value = random_value();
        add_node(db, i, value);
    }
}

char *random_value() {
    int string_length = rand() % 40;
    char *value = malloc(string_length + 1);
    char *letters_string = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    for (int i = 0; i < string_length; i++) {
        int number = rand() % strlen(letters_string);
        value[i] = letters_string[number];
    }
    return value;
}
