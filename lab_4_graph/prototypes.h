//
// Created by igor on 14.05.2022.
//

#ifndef GRAPH_PROTOTYPES_H
#define GRAPH_PROTOTYPES_H


#include <stdio.h>
#include "stdlib.h"
#include "string.h"



typedef struct queueList {
    int size;
    struct AdjListNode *head;
    struct AdjListNode *tail;
} queueList;

typedef struct AdjListNode {
    char *vertex_name;
    struct AdjList *vertex;
    struct AdjListNode *next;
} AdjListNode;

typedef struct AdjList {
    int size;
    char *vertexName;
    struct AdjListNode *head;
    struct AdjListNode *tail;
    struct AdjList *down;

} AdjList;

typedef struct Graph {
    int size;
    AdjList *head;
    AdjList *tail;
} Graph;


static void _add(Graph *graph, AdjList *vertex);


Graph *createGraph();
queueList *createQueue();
void add_to_queue(queueList *Queue, AdjListNode *node);
void delete_queueNode(queueList *Queue, AdjListNode *node);
AdjListNode *_search_queueNode(queueList *Queue, char *vertex_name);
AdjList *createVertex(char *vertex_name);
AdjList *searchVertex(Graph *graph, char *vertex_name);
void addVertex(Graph *graph, char *vertex_name);
int createEdge(Graph *graph, char *scr, char *dest);
void printGraph(Graph* graph);
int graph_find(Graph *graph, char *scr, char *dest);
double graph_relative(Graph *graph, char *scr, char *dest);
void read_and_generate(Graph *graph, char *filename, int amount_edges_to);

int _recursion(queueList* Stack, queueList *Visited, char *dest, int count);

#endif //GRAPH_PROTOTYPES_H
