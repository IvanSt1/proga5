#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "dialog.h"
#include "get.h"
#include "item.h"

const char *msgs[] = {"1. Quite","2. Add Node", "3. Add Edge", "4. Delete Node", "5. Delete Edge", "6. Show","7.Find","8. Find Shortest way","9 ","10. Create Random Graph"};
const int NMgsgs = sizeof(msgs) / sizeof(msgs[0]);

int (*f[])(Graph**) ={NULL, D_Add_Node, D_Add_Edge, D_Delete_Node, D_Delete_Edge, D_Show, D_Find, D_Find_Shortest_Way,
                     D_Strong_Connectivity};

int main() {
    int rc;
    Graph* graph=(Graph*) calloc(sizeof(Graph),1);
    graph->node= NULL;
    graph->size_ed=0;
    graph->size_node=0;
    printf("Do you want to work with file? (y/n)");
    char *answer;
    answer= Get_Str();
    if (answer==NULL){
        printf("Something goes wrong restart\n");
        return 0;
    }
    int k1 = strcmp("y", answer);
    int k2 = strcmp("n", answer);
    while ((k1 != 0) && (k2 != 0)) {
        printf("Try one more time\n");
        answer=Get_Str();
        if (answer==NULL){
            printf("something goes wrong restart\n");
            return 0;
        }
        k2 = strcmp("n", answer);
        k1 = strcmp("y", answer);
    }
    if (k1 == 0) {
        Add_from_file(&graph);
    }
    rc = dialog(msgs, NMgsgs);
    while (rc) {
        if (!f[rc](&graph)) {
            break;
        }
        rc = dialog(msgs, NMgsgs);
    }

    printf("Do you want to save to file? (y/n)");
    answer = Get_Str();
    if (answer==NULL){
        printf("Something goes wrong restart\n");
        return 0;
    }
    k1 = strcmp("y", answer);
    k2 = strcmp("n", answer);
    while ((k1 != 0) && (k2 != 0)) {
        printf("Try one more time\n");
        answer=Get_Str();
        if (answer==NULL){
            printf("something goes wrong restart\n");
            return 0;
        }
        k2 = strcmp("n", answer);
        k1 = strcmp("y", answer);
    }
    if (k1 == 0) {
        D_Save_File(&graph);
    }
    free(answer);
    D_Delete_ALL(&graph);
    free(graph);
    return 0;
}