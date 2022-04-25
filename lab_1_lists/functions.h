//
// Created by igor on 25.04.2022.
//

#ifndef UNTITLED2_FUNCTIONS_H
#define UNTITLED2_FUNCTIONS_H


DB *create_db() {
    DB *tmp = (DB *)malloc(sizeof(DB));
    tmp->size = 0;
    tmp->head = tmp->tail = NULL;
    return tmp;
}


Node *create_node(char *value) {
    Node *tmp = (Node *)malloc(sizeof(Node));
    tmp->data = (char *)malloc(sizeof(char) * strlen(value) + 1);
    tmp->data = strcpy(tmp->data, value);
    tmp->next = NULL;
    tmp->prev = NULL;
    return tmp;
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
                tmp = tmp->next;
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
                tmp = tmp->next;
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
        printf("Element with index %d was deleted successfully\n", index);
        return;
    }
    printf("There is no such element\n");
}


void add_node_to_end(DB *db, char *value) {
    Node *new_node = create_node(value);
    db->size++;

    if (db->head == NULL & db->tail == NULL) {
        db->head = db->tail = new_node;
        return;
    }
    Node *tmp = db->tail;
    tmp->next = new_node;
    new_node->prev = tmp;
    db->tail = new_node;
}


void add_node_to_begin(DB *db, char *value) {
    Node *new_node = create_node(value);
    db->size++;

    if (db->head == NULL & db->tail == NULL) {
        db->head = db->tail = new_node;
        return;
    }
    Node *tmp = db->head;
    tmp->prev = new_node;
    new_node->next = tmp;
    db->head = new_node;
}


void put_node_into_a_current_place(DB *db, int index, char *value) {
    if (db->size == 0) {
        printf("Sorry, the database is empy");
        return;
    }
    if (0 <= index & index < db->size) {
        int middle = db->size / 2;
        Node *current_node;
        Node *new_node = create_node(value);

        if (index <= middle) {
            current_node = db->head;
            for(int i = 0; i < index; i++) {
                current_node = current_node->next;
            }
        }
        else {
            current_node = db->tail;
            for(int i = 0; i < db->size - index - 1; i++) {
                current_node = current_node->next;
            }
        }

        Node *prev_node = current_node->prev;

        if (prev_node) {
            prev_node->next = new_node;
            new_node->prev = prev_node;
        } else {
            db->head = new_node;
            current_node->prev = new_node;
        }
        new_node->next = current_node;
        current_node->prev = new_node;
        db->size++;
        printf("Element was added successfully\n");
        return;

    }
    printf("There is no such index\n");
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

    for (int i = 0; i < amount; i++) {
        Node *new_node = create_node("temporary value");
        if (db->head == NULL & db->tail == NULL) {
            db->head = db->tail = new_node;
            continue;
        }
        Node *last_node = db->tail;
        last_node->next = new_node;
        new_node->prev = last_node;
        db->tail = new_node;
        db->size++;
    }
}


#endif //UNTITLED2_FUNCTIONS_H
