//
// Created by ivan on 20.05.2021.
//

#ifndef PROGA5_DIALOG_H
#define PROGA5_DIALOG_H

#include "item.h"

int dialog(const char *msgs[], int n);

void Add_from_file(Graph **);

int D_Delete_ALL(Graph **);

int D_Add_Edge(Graph **);

int D_Add_Node(Graph **);


int D_Delete_Node(Graph **);


int D_Delete_Edge(Graph **);


int D_Show(Graph **);


int D_Find(Graph **);


int D_Find_Shortest_Way(Graph **);


int D_Strong_Connectivity(Graph **);


void D_Save_File(Graph **);

#endif //PROGA5_DIALOG_H
