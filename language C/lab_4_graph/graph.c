#include "graph_functions.c"


int main() {
    Graph *gr = createGraph();
    printGraph(gr);
    read_and_generate(gr, "categories.txt", 3);
    char begin[] = "Автомобили";
    char end[] = "Журналистика";
    printf("Path from %s to %s is %d edges\n", begin , end, graph_find(gr, begin, end));
    printf("Worth is %.2f\n", graph_relative(gr, begin, end));
    return 0;
}
