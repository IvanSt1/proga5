//
// Created by ivan on 20.05.2021.
//

#ifndef PROGA5_ITEM_H
#define PROGA5_ITEM_H
typedef struct Node Node;
typedef struct Ed {
    Node*from;
    Node*to;
    int weight;
    struct Ed * next;
    struct Ed *previous;
} Ed;
typedef struct Node {
    int name;
    int x;
    int y;
    Ed* edges;

} Node;
typedef struct Graph {
    int size_node;
    int size_ed;
    Node** node;
} Graph;
void show(Graph* graph);
Ed* find_edge( Node *from,Node *to);
int add_node(Graph** graph, int x, int y, int name);
int add_edge(Graph **graph, int name1, int name2, int weight);
int delete_node(Graph **graph, int name);
int delete_edge(Graph** graph, int name_from, int name_to);
Node *find_x_y_name(Graph *graph, int x, int y, int name) ;
Node *find_name(Graph *graph, int name);
Node *find_x_y(Graph *graph, int x, int y);
void delete_all(Graph **Graph);
#endif //PROGA5_ITEM_H
