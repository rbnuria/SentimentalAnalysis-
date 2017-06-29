#ifndef DE_H
#define DE_H

#include <iostream>
#include <vector>
#include "global.h"


//Es un método que enfatiza la mutación -> Realiza cruce despues de la mutación.
void DE_rand(vector <Instance> & train, vector <float> & sol, float CR, float F);

void DE_current_best(vector <Instance> & train, vector <float> & sol, float CR, float F);

vector <vector <float>> initialize(int size1, int size2);

//Coge 3 posiciones aleatorias sin coger pos. Todos excluyentes entre sí.
vector <int> select3Parents(int pos);


vector <int> select2Parents(int pos);

void exe_DE_rand(vector <Instance> & train, vector <Instance> & test, float & tasa, float & time1, float & t_train);

void exe_DE_current_best(vector <Instance> & train, vector <Instance> & test, float & tasa, float & time1, float & t_train);

void exe_DE_best(vector <Instance> & train, vector <Instance> & test, float & tasa, float & time1, float & t_train);

void normalize_solution(vector <float> & sol);


#endif