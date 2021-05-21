#include <stddef.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include "item.h"


int add_node(Graph **graph, int x, int y, int name) {
    int res;
    if (find_x_y_name((*graph), x, y, name) != NULL) {
        res = 1;
    } else {
        int n = (*graph)->size_node;
        if ((*graph)->node == NULL) {
            (*graph)->node = (Node **) calloc(sizeof(Node **), 1);
            n = 0;
        } else {
            (*graph)->node = (Node **) realloc((*graph)->node, (n + 1) * sizeof(Node *));
        }
        Node *new = (Node *) calloc(sizeof(Node), 1);
        new->x = x;
        new->y = y;
        new->name = name;
        new->edges = NULL;
        (*graph)->node[n] = new;
        (*graph)->size_node = n + 1;
        res = 0;

    }
    return res;
}

int add_edge(Graph **graph, int from, int to, int weight) {
    int res;
    Node *f = find_name(*graph, from);
    Node *t = find_name(*graph, to);
    if (f == NULL || t == NULL) {
        res = 4;
    } else {
        if (find_edge(f, t) != NULL) {
            res = 2;
        } else {
            if (t == f) {
                res = 5;
            } else {
                Ed *edge = (Ed *) calloc(1, sizeof(Ed));
                edge->weight = weight;
                edge->from = f;
                edge->to = t;
                edge->next=NULL;
                edge->previous=NULL;
                if(f->edges==NULL){
                    f->edges=edge;
                }
                else {
                    f->edges->previous = edge;
                    edge->next = f->edges;
                    f->edges = edge;
                }
                (*graph)->size_ed++;
                res = 0;
            }

        }
    }
    return res;
}

Ed *find_edge(Node *from, Node *to) {
    Ed *res = NULL;
    Ed *ptr = from->edges;
    while (ptr != NULL) {
        if (ptr->to == to) {
            res = ptr;
            break;
        }
        ptr = ptr->next;
    }

    return res;
}

int delete_node(Graph **graph, int name) {
    int res, i,k;
    Ed * e;
    Node *ptr = find_name(*graph, name);
    if (ptr == NULL) {
        res = 4;
    } else {
        for(i=0; i<(*graph)->size_node;i++){
            if((*graph)->node[i]==ptr){
                k=i;
                e = (*graph)->node[i]->edges;
                while (e != NULL) {
                    delete_edge(graph, e->from->name, e->to->name);
                    e = e->next;
                }
            }
            else {
                e = (*graph)->node[i]->edges;
                while (e != NULL) {
                    if(e->to==ptr) {
                        delete_edge(graph, e->from->name, e->to->name);
                    }
                    e = e->next;
                }
            }
        }

        (*graph)->node[k]=(*graph)->node[(*graph)->size_node-1];
        (*graph)->node=(Node**) realloc((*graph)->node,(*graph)->size_node-1);
        (*graph)->size_node--;
        free(ptr);
        res = 0;
    }
    return res;
}

int delete_edge(Graph **graph, int name_from, int name_to) {
    int res;
    Node *f = find_name(*graph, name_from);
    Node *t = find_name(*graph, name_to);
    if (f == NULL || t == NULL || f == t) {
        res = 6;
    } else {
        Ed* e=find_edge(f, t);
        if ( e== NULL) {
            res = 6;
        } else {
            if(e->previous!=NULL) {
                e->previous->next = e->next;
            }
            if(e->next!=NULL){
            e->next->previous=e->previous;
                }
            if(e->next==NULL&&e->previous==NULL){
                f->edges=NULL;
            }
            free (e);
            res = 0;
            (*graph)->size_ed--;
        }
    }
    return res;
}

Node *find_x_y_name(Graph *graph, int x, int y, int name) {
    Node *res = NULL;
    for (int i = 0; i < graph->size_node; i++) {
        if ((graph->node[i]->x == x && graph->node[i]->y == y) || (graph->node[i]->name == name)) {
            res = graph->node[i];
            break;
        }
    }
    return res;
}

Node *find_name(Graph *graph, int name) {
    Node *res = NULL;
    for (int i = 0; i < graph->size_node; i++) {
        if (graph->node[i]->name == name) {
            res = graph->node[i];
            break;
        }
    }
    return res;
}

Node *find_x_y(Graph *graph, int x, int y) {
    Node *res = NULL;
    for (int i = 0; i < graph->size_node; i++) {
        if (graph->node[i]->x == x && graph->node[i]->y == y) {
            res = graph->node[i];
            break;
        }
    }
    return res;
}

void show(Graph *graph) {
    Ed *edg;
    for (int i = 0; i < graph->size_node; i++) {
        printf("Node name: %d, x: %d, y: %d\n", graph->node[i]->name, graph->node[i]->x, graph->node[i]->y);
        printf("Connect with: \n");
        edg = graph->node[i]->edges;
        while (edg != NULL) {
            printf("(name from: %d, name to %d, weight: %d )\n", edg->from->name, edg->to->name, edg->weight);
            edg = edg->next;
        }
        printf("\n");
    }

}

int work_with_file(Node *graph, FILE *file) {
    int res;
    int num1;
    int num2;
    char *key;
    char *string;
    int n;
    while (feof(file) == 0) {
        n = fscanf(file, " %m[^\n]", &key);
        if (n == -1) {
            break;
        }
        fgetc(file);
        n = fscanf(file, " %d", &num1);
        if (n == 0) {
            res = 1;
            break;
        }
        fgetc(file);
        n = fscanf(file, " %d", &num2);
        if (n == 0) {
            res = 1;
            break;
        }
        fgetc(file);
        fscanf(file, " %m[^\n]", &string);
        fgetc(file);

        if (res != 0) {
            break;
        }
    }

    return res;
}
void delete_all(Graph **graph){
    for(int i=(*graph)->size_node-1;i>=0;i--){
        delete_node(graph,(*graph)->node[i]->name);
    }
}