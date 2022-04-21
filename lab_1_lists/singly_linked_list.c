#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct Node {
    char *data;
    struct Node *next;
} Node;

Node *db = NULL;
int count = 0;

Node *create_node(char *value) {
    Node *tmp = (Node *)malloc(sizeof(Node));
    tmp->data = (char *)malloc(sizeof(char) * strlen(value) + 1);
    tmp->data = strcpy(tmp->data, value);
    tmp->next = NULL;
    return tmp;
}


Node *get_current_node(int value) {
    Node *tmp = db;
    for(int i = 0; i < value + 1; i++) {
        if (i == value) {
            return tmp;
        }
        tmp = tmp->next;
    }
}


void add_node(char *value, int num) {
    Node *tmp_node = db;
    Node *new_node = create_node(value);
    if (!db) {
        db = new_node;
        count++;
        return;
    }
    if (num == 1) {

        while (tmp_node->next != NULL) {
            tmp_node = tmp_node->next;
        }
        tmp_node->next = new_node;
        count++;
        return;
    }
    if (num == -1) {
        new_node->next = tmp_node;
        db = new_node;
        count++;
        return;
    }
}


void show_node(int value) {
    if (count -1 < value) {
        printf("There is not such element\n");
        return;
    }
    Node *node = get_current_node(value);
    printf("[%s]\n", node->data);
}


void show_index(char *value) {
    Node *tmp = db;
    int count = 0;
    while (tmp) {
        if (strcmp(value, tmp->data) == 0) {
            printf("[%d]\n", count);
            return;
        }
        count++;
        tmp = tmp->next;
    }
}


void delete_node(int value) {
    if (count - 1 < value) {
        printf("There is not such element\n");
        return;
    }
    Node *prev_node = get_current_node(value - 1);
    Node *tmp = get_current_node(value);
    prev_node->next = tmp->next;
    free(tmp);
}


void print_db() {
    Node *tmp = db;
    while (tmp) {
        printf("[%s]\n", (*tmp).data);
        tmp = tmp->next;
    }
}


int main() {
    add_node("Hello", 1);
    add_node("World", -1);
    add_node("Pip", 1);
    add_node("Gio", 1);
    add_node("Hello", 1);
    show_node(5);

    delete_node(5);

    show_index("Pip");

    print_db();
    return 0;
}
