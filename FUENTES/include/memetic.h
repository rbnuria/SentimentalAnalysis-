#ifndef MEMETICS_H
#define MEMETICS_H

#include <iostream>
#include "global.h"
#include "genetics.h"

using namespace std;

/*
	Búsqueda local modificada según los requerimientos del problema.
	Pasamos como parámetro el valor de las evaluaciones hasta el momento para  q lo modifique
	Imponemos de única condición para terminar cuando lleve 20*n vecinos generados sin
	mejora.

	return -> fitness de la sol encontrada

*/
float LS_memetic(vector <Instance> & train, vector <float> & sol, int & ev_fit);

/*
	Función Compare usada para ordenar el vector < vector <float>> según su fitness.
*/

bool comp(pair <vector<float>, float> & p1, pair <vector<float>, float> & p2);


/*
	Genera una población inicial de 10 cromosomas de forma aleatoria.
*/
vector <vector <float> > generate_generation_memetic(vector <Instance> & problem);

/*

	Algoritmo memético AM(10, 0.1), se crea una población de 10 cromosomas cada 10 generaciones
	se aplica BL sobre todos los cromosomas de la población. 
*/

vector <float> AM_10_1(vector <Instance> & train, vector <vector <float> > & generation,  vector < vector <float> >  & second_generation);


vector <float> AM_10_01(vector <Instance> & train, vector <vector <float> > & generation,  vector < vector <float> >  & second_generation);


vector <float> AM_10_01mej(vector <Instance> & train, vector <vector <float> > & generation,  vector < vector <float> >  & second_generation);

/*
	Ejecutar el algoritmo AM_10_1
*/
void exe_AM_10_1(vector <Instance> & train, vector <Instance> & test, float & tasa, float & time1, float & t_train);


/*
	Ejecutar el algoritmo AM_10_01
*/
void exe_AM_10_01(vector <Instance> & train, vector <Instance> & test, float & tasa, float & time1, float & t_train);


/*
	Ejecutar el algoritmo AM_10_01
*/
void exe_AM_10_01mej(vector <Instance> & train, vector <Instance> & test, float & tasa, float & time1, float & t_train);


#endif
