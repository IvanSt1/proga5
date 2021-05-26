#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <time.h>
#include "dialog.h"
#include "get.h"
#include "item.h"

const char *errmsgs[] = {"OK", "Duplicate Node", "Duplicate Edge", "Memory problem", "There isn't such node",
                         "To is the same as from", "There isn't such Edge", "There isn't this way"};

int dialog(const char *msgs[], int n) {
    char *errmsg = "";
    int rc;
    int i, p;
    do {
        puts(errmsg);
        errmsg = "You are wrong";
        for (i = 0; i < n; ++i)
            puts(msgs[i]);
        puts("Make your choise: -->");
        p = Get_Int(&rc);
        if (p == 0)
            rc = 0;
    } while (rc < 0 || rc > n);
    rc--;
    return rc;
}

void Add_from_file(Graph **graph) {
    printf("Enter file Name\n");
    char *name = Get_Str();
    if (name == NULL) {
        return;
    }
    FILE *file = fopen(name, "rb");
    free(name);
    while (file == NULL) {
        printf("There is no such file, try one more time\n");
        printf("Enter file Name\n");
        name = Get_Str();
        if (name == NULL) {
            return;
        }
        file = fopen(name, "r");
        free(name);
    }
    int res = work_with_file(graph, file);
    if (res != 0) {
        printf("Format of information is wrong in file\n");
        D_Delete_ALL(graph);
    } else {
        printf("Successful reading from file\n");
    }
    fclose(file);
}

int D_Delete_ALL(Graph **graph) {
    if ((*graph)->node != NULL) {
        delete_all(graph);
    }
    return 1;
}

int D_Add_Edge(Graph **graph) {
    if (((*graph)->size_node) * ((*graph)->size_node - 1) == ((*graph)->size_ed)) {
        printf("Reached max number of Edges. Make one more Node");
        return 1;
    }
    int name1, name2, weight;
    printf("Enter node from\n");
    Get_Int(&name1);
    printf("Enter node to\n");
    Get_Int(&name2);
    printf("Enter weight\n");
    Get_Int(&weight);
    int rc = add_edge(graph, name1, name2, weight);
    printf("%s", errmsgs[rc]);
    return 1;
}

int D_Add_Node(Graph **graph) {
    int x, y, name;
    printf("Enter x\n");
    Get_Int(&x);
    printf("Enter y\n");
    Get_Int(&y);
    printf("Enter name\n");
    Get_Int(&name);
    int rc = add_node(graph, x, y, name);
    printf("%s", errmsgs[rc]);
    return 1;
}

int D_Delete_Node(Graph **graph) {
    if ((*graph)->node == NULL) {
        printf("Graph is empty");
    } else {
        int name1, name2, weight;
        printf("Enter name\n");
        Get_Int(&name1);

        int rc = delete_node(graph, name1);
        printf("%s\n", errmsgs[rc]);
    }
    return 1;
}

int D_Delete_Edge(Graph **graph) {
    if ((*graph)->node == NULL) {
        printf("Graph is empty");
    } else {
        int name1, name2, weight;
        printf("Enter node from\n");
        Get_Int(&name1);
        printf("Enter node to\n");
        Get_Int(&name2);
        int rc = delete_edge(graph, name1, name2);
        printf("%s", errmsgs[rc]);
    }
    return 1;
}

int D_Show(Graph **graph) {
    if ((*graph)->node == NULL) {
        printf("Graph is empty");
    } else {
        show(*graph);
    }
    return 1;
}

int D_Find(Graph **graph) {
    if ((*graph)->node == NULL) {
        printf("Graph is empty");
    } else {
        int name1, name2, weight;
        printf("Enter node from\n");
        Get_Int(&name1);
        printf("Enter node to\n");
        Get_Int(&name2);
        int rc = width_find(graph, name1, name2);
        printf("%s", errmsgs[rc]);
        if(rc==0) {
            for (int i = 0; i < (*graph)->size_node; i++) {
                printf("Name: %d, distant: %d \n", (*graph)->node[i]->name, (*graph)->node[i]->d);
            }
        }
    }
    return 1;
}

int D_Find_Shortest_Way(Graph **graph) {
    if ((*graph)->node == NULL) {
        printf("Graph is empty");
    } else {
        int name1, name2, weight;
        printf("Enter node from\n");
        Get_Int(&name1);
        printf("Enter node to\n");
        Get_Int(&name2);
        int rc = find_shortest_way(graph, name1, name2);
        if (rc == 0) {
            vyvod(find_name((*graph),name2));
        }
        printf("%s", errmsgs[rc]);
    }
    return 1;
}

int D_Strong_Connectivity(Graph **graph) {
    if ((*graph)->node == NULL) {
        printf("Graph is empty");
    } else {
        int rc = find_strong_connectivity(graph);
        printf("%s", errmsgs[rc]);
    }
    return 1;
}

void D_Save_File(Graph **graph) {
    printf("Enter file Name\n");
    char *name = Get_Str();
    if (name == NULL) {
        return;
    }
    FILE *file = fopen(name, "wb");
    if (file == NULL) {
        printf("Problem with open file");
    }
    save_to_file(graph, file);
    free(name);
    fclose(file);
    printf("OK");
}

int D_Random_Graph(Graph **graph) {
    D_Delete_ALL(graph);
    (*graph)->node=NULL;

    int n_n, n_e;
    printf("Enter number of nodes\n");
    Get_Int(&n_n);
    printf("Enter number of edges\n");
    Get_Int(&n_e);
    while ((n_n) * (n_n - 1) < (n_e)) {
        printf("Too much edges\n");
        printf("Enter number of edges\n");
        Get_Int(&n_e);
    }
    create_random(graph,n_n,n_e);
    return 1;
}
int D_Timing(Graph** x){
    Graph* graph=(Graph*) calloc(sizeof(Graph),1);
    int n = 1,  k, cnt = 1000, i, m, re;
    clock_t first, last;
    srand(time(NULL));
    while (n <= 10) {
        create_random(&graph,n * 1000,2*n*1000);
        m = 0;
        first = clock();
        for (i = 0; i < 1000; ++i) {
            if (width_find(&graph,rand()%5000,rand()%5000)==0) {
                m++;
            }
        }
        last = clock();
        printf("%d items were found\n", m);
        printf("test #%d, number of nodes = %d, time = %ld\n",  n, n * cnt, last - first);
        m=0;
        for (i = 0; i < 1000; ++i) {

            if (find_shortest_way(&graph,rand()%5000,rand()%5000)==0) {
                m++;
            }
        }
        last = clock();
        printf("%d ways were found\n", m);
        printf("test #%d, number of nodes = %d, time = %ld\n",  n, n * cnt, last - first);
        n++;
    }

    return 1;
}
int D_Graphviz(Graph** graph){
    if ((*graph)->node == NULL) {
        printf("Graph is empty");
    } else {
        char *s="output.txt";
        FILE *file=fopen(s,"w");
              graphviz((*graph),file);
        fclose(file);
    }

    return 1;
}