#include <stdio.h>
#include <malloc.h>
#include "dialog.h"
#include "get.h"
#include "item.h"

const char *errmsgs[] = {"OK", "Duplicate Node", "Duplicate Edge", "Memory problem", "There is no such node",
                         "To is the same as from","There is no such Edge"};

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

int Add_from_file(Graph **graph) {
    return 1;
}

int D_Delete_ALL(Graph **graph) {
    if((*graph)->node!=NULL){
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
    printf("Enter first node\n");
    Get_Int(&name1);
    printf("Enter second node\n");
    Get_Int(&name2);
    printf("Enter weight\n");
    Get_Int(&weight);
    int rc = add_edge(graph, name1, name2,weight);
    printf("%s", errmsgs[rc]);
    return 1;
}

int D_Add_Node(Graph **graph) {
    int x, y,name;
    printf("Enter x\n");
    Get_Int(&x);
    printf("Enter y\n");
    Get_Int(&y);
    printf("Enter name\n");
    Get_Int(&name);
    int rc = add_node(graph, x, y,name);
    printf("%s", errmsgs[rc]);
    return 1;
}

int D_Delete_Node(Graph **graph) {
    if((*graph)->node==NULL){
        printf("Graph is empty");
    }
    else {
        int name1, name2, weight;
        printf("Enter name\n");
        Get_Int(&name1);

        int rc = delete_node(graph, name1);
        printf("%s", errmsgs[rc]);
    }
    return 1;
}

int D_Delete_Edge(Graph **graph) {
    if((*graph)->node==NULL){
        printf("Graph is empty");
    }
    else {
        int name1, name2, weight;
        printf("Enter first node\n");
        Get_Int(&name1);
        printf("Enter second node\n");
        Get_Int(&name2);
        int rc = delete_edge(graph, name1, name2);
        printf("%s", errmsgs[rc]);
    }
    return 1;
}

int D_Show(Graph **graph) {
    if((*graph)->node==NULL){
        printf("Graph is empty");
    }
    else{
        show(*graph);
    }
    return 1;
}

int D_Find(Graph **graph) {
    return 1;
}

int D_Find_Shortest_Way(Graph **graph) {
    return 1;
}

int D_Strong_Connectivity(Graph **graph) {
    return 1;
}

int D_Save_File(Graph **graph) {
    return 1;
}