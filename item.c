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
            (*graph)->node = (Node **) calloc(1, sizeof(Node *));
            n = 0;
        } else {
            (*graph)->node = (Node **) realloc((*graph)->node, (n + 1) * sizeof(Node *));
        }
        Node *new = (Node *) calloc(1, sizeof(Node));
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
                Ed *in = (Ed *) calloc(1, sizeof(Ed));
                edge->weight = weight;
                edge->from = f;
                edge->to = t;
                edge->next = NULL;
                edge->previous = NULL;
                in->weight = weight;
                in->from = f;
                in->to = t;
                in->next = NULL;
                in->previous = NULL;
                if (f->edges == NULL) {
                    f->edges = edge;
                } else {
                    f->edges->previous = edge;
                    edge->next = f->edges;
                    f->edges = edge;
                }
                if (t->in_edges == NULL) {
                    t->in_edges = in;
                } else {
                    t->in_edges->previous = in;
                    in->next = t->in_edges;
                    t->in_edges = in;
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
    Ed *e, *next;
    Node *ptr = find_name(*graph, name);
    if (ptr == NULL) {
        res = 4;
    } else {
        for (i = 0; i < (*graph)->size_node; i++) {
            if ((*graph)->node[i] == ptr) {
                k = i;
                e = (*graph)->node[i]->edges;
                while (e != NULL) {
                    next = e->next;
                    delete_edge(graph, e->from->name, e->to->name);
                    e = next;
                }
                e = (*graph)->node[i]->in_edges;
                while (e != NULL) {
                    next = e->next;
                    delete_edge(graph, e->from->name, e->to->name);
                    e = next;
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
        Ed*x;
        Ed *e=find_edge(f, t);
        if (e == NULL) {
            res = 6;
        } else {
            if (e->previous != NULL) {
                e->previous->next = e->next;
            }
            else{
                f->edges=e->next;
            }
            if (e->next != NULL) {
                e->next->previous = e->previous;
            }
            if (e->next == NULL && e->previous == NULL) {
                f->edges = NULL;
            }
            free(e);
            x = t->in_edges;
            while (x != NULL) {
                if (x->from == f) {
                    break;
                }
                x = x->next;
            }
            if (x->previous != NULL) {
                x->previous->next = x->next;
            }
            else{
                t->in_edges=x->next;
            }
            if (x->next != NULL) {
                x->next->previous = x->previous;
            }
            if (x->next == NULL && x->previous == NULL) {
                t->in_edges= NULL;
            }
            free(x);
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
        printf("Enter this node: \n");
        edg = graph->node[i]->in_edges;
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
    Ed *e, *next;
    for (int i=0;i<(*graph)->size_node;i++) {
        e = (*graph)->node[i]->edges;
        while (e != NULL) {
            next = e->next;
            free(e);
            e = next;
        }
        e = (*graph)->node[i]->in_edges;
        while (e != NULL) {
            next = e->next;
            free(e);
            e = next;
        }
        free((*graph)->node[i]);
    }
    free((*graph)->node);
    (*graph)->size_node = 0;
    (*graph)->size_ed = 0;
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

int width_find(Graph **graph, int from, int to) {
    int res = 7, size, d;
    Node *f = find_name(*graph, from);
    Node *t = find_name(*graph, to);
    if (f == NULL || t == NULL || f == t) {
    } else {
        for (int i = 0; i < (*graph)->size_node; i++) {
            (*graph)->node[i]->color = 0;
            (*graph)->node[i]->d = -1;
        }
        f->color++;
        Find *Q = (Find *) calloc(1, sizeof(Find));
        Q[0].node = f;
        size = 1;
        f->d = 0;
        d = 1;
        Q[0].d = 0;
        while (Q != NULL) {
            Ed *e = Q[0].node->edges;
            if (Q[0].node == t) {
                res = 0;
            }
            while (e != NULL) {
                e->to->color++;
                if (e->to->color < 2) {
                    Q = (Find *) realloc(Q, (size + 1) * sizeof(Find));
                    Q[size].node = e->to;
                    Q[size].d = Q[0].d + 1;
                    e->to->d = Q[0].d + 1;
                    size++;
                }
                e = e->next;
            }
            d++;
            for (int i = 0; i < size - 1; i++) {
                Q[i] = Q[i + 1];
            }
            size--;
            Q = (Find *) realloc(Q, (size) * sizeof(Find));
        }
    }

    return res;
}

void create_random(Graph **graph, int n_n, int n_e) {
    int name1, name2, w;
    for (int i = 0; i < n_n; i++) {
        add_node(graph, i + 1, i + 1, i + 1);
    }
    for (int i = 0; i < n_e; i++) {
        name1 = 1 + rand() % n_n;
        name2 = 1 + rand() % n_n;
        w = rand() % 100;
        while (add_edge(graph, name1, name2, w) != 0) {
            name1 = 1 + rand() % (n_n + 1);
            name2 = 1 + rand() % (n_n + 1);
        }
    }

}

Find *Sort(Find *mas, int size) {
    int i, i_min, min;
    i_min = -1;
    Find x;
    min = 2147483647;
    for (i = 0; i < size; i++) {
        if (mas[i].node->d != -1 && min > mas[i].node->d) {
            i_min = i;
            min = mas[i].node->d;
        }
    }
    if (i_min == -1) {
        mas = (Find *) realloc(mas, 0);
        return NULL;
    }
    x = mas[i_min];
    mas[i_min] = mas[size - 1];
    mas[size - 1] = x;
    return mas;
}

void vyvod(Node *x) {
    if (x->prev != NULL) {
        vyvod(x->prev);
    }
    printf("Name: %d, distant: %d\n", x->name, x->d);
}

int find_shortest_way(Graph **graph, int from, int to) {
    int res = 7, size, d, i;
    Ed *e;
    Node *f = find_name(*graph, from);
    Node *t = find_name(*graph, to);
    if (f == NULL || t == NULL || f == t) {
    } else {
        Find *Q = (Find *) calloc((*graph)->size_node, sizeof(Find));
        for (i = 0; i < (*graph)->size_node; i++) {
            Q[i].node = (*graph)->node[i];
            Q[i].node->prev = NULL;
            (*graph)->node[i]->d = -1;
            (*graph)->node[i]->color = 0;
        }
        f->d = 0;
        size = (*graph)->size_node;
        while (Q != NULL) {
            Q = Sort(Q, size);
            if (Q == NULL) {
                break;
            }
            e = Q[size - 1].node->edges;
            if (e != NULL) {
                e->from->color = 1;
            }
            while (e != NULL) {
                if (e->to == t) {
                    res = 0;
                }
                if (e->to->color != 1) {
                    if (e->to->d == -1) {
                        e->to->d = (e->from->d + e->weight);
                        e->to->prev = e->from;
                    } else {
                        if (e->to->d > (e->from->d + e->weight)) {
                            e->to->d = (e->from->d + e->weight);
                            e->to->prev = e->from;
                        }
                    }
                }
                e = e->next;
            }
            size--;
            Q = (Find *) realloc(Q, size * sizeof(Find));
        }

    }


    return res;
}

void dfs(Node **x, Node **p, int *size) {
    if ((*x)->color == 1) {
        return;
    } else {
        (*x)->color = 1;
        Ed *e = (*x)->edges;
        while (e != NULL) {
            dfs(&e->to, p, size);
            e = e->next;
        }
        p[*size]=(*x);
        (*size)++;
        return;
    }
}
void dfs_back(Node**x,int c){
    if((*x)->n_strong!=-1){
        return;
    }
    else{
        (*x)->n_strong=(c);
        Ed *e = (*x)->in_edges;
        while (e != NULL) {
            dfs_back(&e->from,c);
            e = e->next;
        }
    }
}
void show_with_strong_connectivity(Graph **graph){
    for(int i=0;i<(*graph)->size_node;i++){
        printf("Name: %d, Strong connectivity component: %d\n",(*graph)->node[i]->name,(*graph)->node[i]->n_strong);
    }
}
int find_strong_connectivity(Graph **graph) {
    int res, i, size = 0,c=0;
    Node **mas = (Node **) calloc((*graph)->size_node, sizeof(Node*));
    for (i = 0; i < (*graph)->size_node; i++) {
        (*graph)->node[i]->color = 0;
        (*graph)->node[i]->n_strong = -1;
    }
    for (i = 0; i < (*graph)->size_node; i++) {
        dfs(&((*graph)->node[i]), mas, &size);
    }
    for (i =(*graph)->size_node-1 ; i>=0; i--) {
        if(mas[i]->n_strong==-1){
            dfs_back(&mas[i], c);
            c++;
        }
    }
    show_with_strong_connectivity(graph);
    res=0;
    mas=(Node**) realloc(mas,0);
    return res;
}
void graphviz(Graph *graph, FILE* file){
    for(int i=0;i<graph->size_node;i++){
        Ed *e=graph->node[i]->edges;
        if(e!=NULL) {
            fprintf(file, "\n%d->", graph->node[i]->name);
        }
        while(e!=NULL){
            fprintf(file, "%d", e->to->name);
            if (e->next != NULL) {
                fprintf(file, ",", e->to->name);
            }
            e = e->next;
        }
    }
}