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
            (*graph)->node = (Node **) calloc( 1,sizeof (Node*));
            n = 0;
        } else {
            (*graph)->node = (Node **) realloc((*graph)->node, (n + 1) * sizeof(Node *));
        }
        Node *new = (Node *) calloc(1,sizeof(Node));
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
                edge->next = NULL;
                edge->previous = NULL;
                if (f->edges == NULL) {
                    f->edges = edge;
                } else {
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
    int res, i, k;
    Ed *e;
    Node *ptr = find_name(*graph, name);
    if (ptr == NULL) {
        res = 4;
    } else {
        for (i = 0; i < (*graph)->size_node; i++) {
            if ((*graph)->node[i] == ptr) {
                k = i;
                e = (*graph)->node[i]->edges;
                while (e != NULL) {
                    delete_edge(graph, e->from->name, e->to->name);
                    e = e->next;
                }
            } else {
                e = (*graph)->node[i]->edges;
                while (e != NULL) {
                    if (e->to == ptr) {
                        delete_edge(graph, e->from->name, e->to->name);
                    }
                    e = e->next;
                }
            }
        }

        (*graph)->node[k] = (*graph)->node[(*graph)->size_node - 1];
        (*graph)->node = (Node **) realloc((*graph)->node, ((*graph)->size_node - 1) * sizeof(Node *));
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
        Ed *e = find_edge(f, t);
        if (e == NULL) {
            res = 6;
        } else {
            if (e->previous != NULL) {
                e->previous->next = e->next;
            }
            if (e->next != NULL) {
                e->next->previous = e->previous;
            }
            if (e->next == NULL && e->previous == NULL) {
                f->edges = NULL;
            }
            free(e);
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

int work_with_file(Graph **graph, FILE *file) {
    int res, i, k, n_n, n_e, name, x, y, f, t, weight;
    fread(&n_n, sizeof(int), 1, file);
    fread(&n_e, sizeof(int), 1, file);
    for (i = 0; i < n_n; i++) {
        fread(&name, sizeof(int), 1, file);
        fread(&x, sizeof(int), 1, file);
        fread(&y, sizeof(int), 1, file);
        res = add_node(graph, x, y, name);
        if (res != 0) {
            delete_all(graph);
            return 1;
        }
    }
    for (i = 0; i < n_e; i++) {
        fread(&f, sizeof(int), 1, file);
        fread(&t, sizeof(int), 1, file);
        fread(&weight, sizeof(int), 1, file);
        res = add_edge(graph, f, t, weight);
        if (res != 0) {
            delete_all(graph);
            return 1;
        }
    }
    res = 0;
    return res;
}

void delete_all(Graph **graph) {
    for (int i = (*graph)->size_node - 1; i >= 0; i--) {
        delete_node(graph, (*graph)->node[i]->name);
    }
}

void save_to_file(Graph **graph, FILE *file) {
    Ed *ed;
    fwrite(&(*graph)->size_node, sizeof(int), 1, file);
    fwrite(&(*graph)->size_ed, sizeof(int), 1, file);
    for (int i = 0; i < (*graph)->size_node; i++) {
        fwrite(&(*graph)->node[i]->name, sizeof(int), 1, file);
        fwrite(&(*graph)->node[i]->x, sizeof(int), 1, file);
        fwrite(&(*graph)->node[i]->y, sizeof(int), 1, file);
    }
    for (int i = 0; i < (*graph)->size_node; i++) {
        ed = (*graph)->node[i]->edges;
        while (ed != NULL) {
            fwrite(&ed->from->name, sizeof(int), 1, file);
            fwrite(&ed->to->name, sizeof(int), 1, file);
            fwrite(&ed->weight, sizeof(int), 1, file);
            ed = ed->next;
        }
    }
}
 int width_find(Graph** graph, int from,int to){
    int res=7,size;
    Node *f= find_name(*graph,from);
    Node *t= find_name(*graph,to);
    if(f==NULL || t==NULL||f==t){
    }
    else{
        for(int i=0;i<(*graph)->size_node;i++){
            (*graph)->node[i]->color=0;
            (*graph)->node[i]->d=-1;
        }
        f->color++;
        Find *Q=(Find*) calloc(1,sizeof (Find));
        Q[0].node=f;
        size=1;
        f->d=0;
        Q[0].d=0;
        while (Q!=NULL){
            Ed* e=Q[0].node->edges;
            if(Q[0].node==t){
                res=0;
            }
            while(e!=NULL) {
                e->to->color++;
                if(e->to->color<2) {
                    Q=(Find*) realloc(Q,(size+1)* sizeof(Find));
                    Q[size].node=e->to;
                    size++;
                    Q[size].d=Q[size-1].d+1;
                    e->to->d=Q[size-1].d+1;
                }
                e=e->next;
            }
            for(int i=0;i<size-1;i++){
                Q[i]=Q[i+1];
            }
            Q=(Find*) realloc(Q,(size-1)*sizeof (Find));
            size--;
        }
        for(int i=0;i<(*graph)->size_node;i++){
            printf("Name: %d, distant: %d \n",(*graph)->node[i]->name,(*graph)->node[i]->d);
        }
    }

    return res;
}