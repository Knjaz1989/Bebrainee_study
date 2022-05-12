#include <stdio.h>
#include "stdlib.h"
#include "string.h"


typedef struct AdjListNode {
    char *dest;
    struct AdjListNode *next;
} AdjListNode;

typedef struct AdjList {
    int size;
    char *vertexName;
    AdjListNode *head;
    AdjListNode *tail;
    struct AdjList *down;
} AdjList;

typedef struct Graph {
    int size;
    AdjList *head;
    AdjList *tail;
} Graph;


Graph *createGraph(int size) {
    Graph *g = (Graph*)malloc(sizeof(Graph));
    g->size = size;
    g->head = NULL;
    g->tail = NULL;
    return g;
}

AdjList *createVertex(char *vertex_name) {
    AdjList *vertex = (AdjList*) malloc(sizeof(AdjList));
    vertex->vertexName = vertex_name;
    vertex->size = 0;
    vertex->head = NULL;
    vertex->tail = NULL;
    vertex->down = NULL;
    return vertex;
}

AdjListNode *createNode(char *name) {
    AdjListNode *node = (AdjListNode *) malloc(sizeof(AdjListNode));
    node->dest = name;
    node->next = NULL;
    return node;
}

AdjList *searchVertex(Graph *graph, char *vertex_name) {
    AdjList *tmp = graph->head;
    while (tmp) {
        if (strcmp(tmp->vertexName, vertex_name) == 0) {
            return tmp;
        }
        tmp = tmp->down;
    }
    return tmp;
}

AdjListNode *searchNode(AdjList *vertex, char *name) {
    AdjListNode *tmp = vertex->head;
    while (tmp) {
        if (strcmp(tmp->dest, name) == 0) {
            return tmp;
        }
        tmp = tmp->next;
    }
    return tmp;
}

void addVertex(Graph *graph, char *vertex_name) {
    AdjList *vertex = createVertex(vertex_name);
    if (graph->head == NULL) {
        graph->head = vertex;
        graph->tail = vertex;
        return;
    }
    AdjList *tmp = searchVertex(graph, vertex_name);
    if (tmp) {
        printf("Such vertex exists\n");
        return;
    }
    AdjList *last = graph->tail;
    last->down = vertex;
    graph->tail = vertex;
}

void createEdge(Graph *graph, char *scr, char *dest) {
    if (strcmp(scr, dest) == 0) {
        printf("You can't create edge from %s to %s\n",scr, dest);
        return ;
    }
    AdjList *vertex_from = searchVertex(graph, scr);
    if (vertex_from == NULL) {
        printf("There is no such %s\n", scr);
        return ;
    }
    AdjList *vertex_to = searchVertex(graph, dest);
    if (vertex_to == NULL) {
        printf("There is no such %s\n", dest);
        return ;
    }
    AdjListNode *tmp = createNode(dest);
    if (vertex_from->head == NULL) {
        vertex_from->head = tmp;
        vertex_from->tail = tmp;
        return;
    }
    AdjListNode *prev = vertex_from->tail;
    prev = tmp;
    vertex_from->tail = tmp;
}


void printGraph(Graph* graph)
{
    AdjList *tmp = graph->head;
    while (tmp) {
        AdjListNode *node = tmp->head;
        printf("\n Adjacency list of vertex %s\n head ", tmp->vertexName);
        while (node) {
            printf("-> %s", node->dest);
            node = node->next;
        }
        printf("\n");
        tmp = tmp->down;
    }
}

int main() {
    int graph_size = 1000;
    Graph *gr = createGraph(graph_size);
    addVertex(gr, "Роскошь");
    addVertex(gr, "Роскошь");
    createEdge(gr, "Роскошь", "Роскошь");
    addVertex(gr, "Автомобили");
    createEdge(gr, "Роскошь", "Автомобили");
    printGraph(gr);
    return 0;
}
