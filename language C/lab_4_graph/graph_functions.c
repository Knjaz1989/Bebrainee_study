#include "prototypes.h"

#define NO_SUCH_VERTEX 1
#define WRONG_PATH 2


Graph *createGraph() {
    Graph *graph = (Graph*)malloc(sizeof(Graph));
    graph->size = 0;
    graph->head = NULL;
    graph->tail = NULL;
    return graph;
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


AdjListNode *create_adjNode(AdjList *vertex) {
    AdjListNode *node = (AdjListNode *) malloc(sizeof(AdjListNode));
    node->vertex_name = vertex->vertexName;
    node->vertex = vertex;
    node->next = NULL;
    return node;
}


AdjListNode *search_adjNode(AdjList *vertex, char *name) {
    AdjListNode *tmp = vertex->head;
    while (tmp && strcmp(tmp->vertex_name, name) != 0) {
        tmp = tmp->next;
    }
    return tmp;
}


AdjList *searchVertex(Graph *graph, char *vertex_name) {
    AdjList *tmp = graph->head;
    while (tmp && strcmp(tmp->vertexName, vertex_name) != 0) {
        tmp = tmp->down;
    }
    return tmp;
}


AdjListNode *_search_queueNode(queueList *Queue, char *vertex_name) {
    AdjListNode *node = Queue->head;
    while (node && strcmp(vertex_name, node->vertex_name) != 0) {
        node = node->next;
    }
    return node;
}


queueList *createQueue() {
    queueList *queue = (queueList *) malloc(sizeof(queueList));
    queue->size = 0;
    queue->head = NULL;
    queue->tail = NULL;
    return queue;
}


void add_to_queue(queueList *Queue, AdjListNode *node) {
    Queue->size++;
    if (Queue->head == NULL) {
        Queue->head = node;
        Queue->tail = node;
        return;
    }
    AdjListNode *last = Queue->tail;
    last->next = node;
    Queue->tail = node;
}

void delete_queueNode(queueList *Queue, AdjListNode *node) {
    AdjListNode *next = node->next;
    Queue->head = next;
    Queue->size--;
    free(node);
}


static void _add(Graph *graph, AdjList *vertex) {
    if (graph->head == NULL) {
        graph->head = vertex;
        graph->tail = vertex;
        graph->size++;
        return;
    }
    AdjList *tmp = searchVertex(graph, vertex->vertexName);
    if (tmp) {
        printf("Such vertex exists\n");
        return;
    }
    AdjList *last = graph->tail;
    last->down = vertex;
    graph->tail = vertex;
    graph->size++;
}


void addVertex(Graph *graph, char *vertex_name) {
    AdjList *vertex = createVertex(vertex_name);
    _add(graph, vertex);
}


void _create(AdjList *vertex_from, AdjList *vertex_to) {
    AdjListNode *new_adjNode = create_adjNode(vertex_to);
    vertex_from->size++;
    if (vertex_from->head == NULL) {
        vertex_from->head = new_adjNode;
        vertex_from->tail = new_adjNode;
        return;
    }
    AdjListNode *prev = vertex_from->tail;
    prev->next = new_adjNode;
    vertex_from->tail = new_adjNode;
}


int createEdge(Graph *graph, char *scr, char *dest) {
    if (strcmp(scr, dest) == 0) {
        printf("You can't create edge from %s to %s\n",scr, dest);
        return WRONG_PATH;
    }
    AdjList *vertex_1 = searchVertex(graph, scr);
    AdjList *vertex_2 = searchVertex(graph, dest);
    if (vertex_1 == NULL || vertex_2 == NULL) {
        return NO_SUCH_VERTEX;
    }
    _create(vertex_1, vertex_2);
    _create(vertex_2, vertex_1);
    return 0;
}


void printGraph(Graph* graph) {
    AdjList *tmp = graph->head;
    while (tmp) {
        AdjListNode *node = tmp->head;
        printf("\n Adjacency list of vertex %s\n head ", tmp->vertexName);
        while (node) {
            printf("-> %s", node->vertex_name);
            node = node->next;
        }
        printf("\n");
        tmp = tmp->down;
    }
}


int _recursion(queueList* Stack, queueList *Visited, char *dest, int count) {
    queueList *newStack = createQueue();
    while (Stack->head) {
        AdjListNode *nod = Stack->head;
        if (_search_queueNode(Visited, nod->vertex_name) == NULL) {
            if (strcmp(nod->vertex_name, dest) == 0) {
                free(Stack);
                free(newStack);
                return count;
            }
            AdjListNode *tmp = nod->vertex->head;
            while (tmp) {
                add_to_queue(newStack, create_adjNode(tmp->vertex));
                tmp = tmp->next;
            }
            add_to_queue(Visited, create_adjNode(nod->vertex));
        }
        delete_queueNode(Stack, nod);
    }
    free(Stack);
    count++;
    if (newStack->size == 0) {
        return 0;
    }
    return _recursion(newStack, Visited, dest, count);
}


int graph_find(Graph *graph, char *scr, char *dest) {
    AdjList *vertex_from = searchVertex(graph, scr);
    AdjList *vertex_to = searchVertex(graph, dest);
    if (vertex_from == NULL || vertex_to == NULL) {
        return 0;
    }
    queueList *Visited = createQueue();
    queueList *Stack = createQueue();
    add_to_queue(Stack, create_adjNode(vertex_from));
    int count = _recursion(Stack, Visited, dest, 0);
    free(Visited);
    return count;
}


double graph_relative(Graph *graph, char *scr, char *dest) {
    int count = graph_find(graph, scr, dest);
    if (count) {
        double value = 10;
        for (int i = 1; i < count; i++) {
            value = value / 2;
        }
        return value;
    }
    return count;
}


void read_and_generate(Graph *graph, char *filename, int amount_edges_to) {
    char stroka[50];
    char stored[100][50];
    FILE *file = fopen(filename, "r");
    int i = 0;
    while(fgets(stroka, 50, file) != NULL) {
        if(index(stroka, '\n') != NULL ) {
            *index(stroka, '\n') = '\0';
        }
        strcpy(stored[i], stroka);
        addVertex(graph, stored[i]);
        i++;
    }
    fclose(file);
    int edge_number;
    for (int j = 0; j < graph->size; j++) {
        int ran_edges_value = rand() % amount_edges_to;
        while (ran_edges_value) {
            edge_number = rand() % graph->size;
            if (strcmp(stored[j], stored[edge_number]) == 0 ||
            search_adjNode(searchVertex(graph, stored[j]), stored[edge_number])) {

                continue;
            }
            createEdge(graph, stored[j], stored[edge_number]);
            ran_edges_value--;
        }
    }
}
