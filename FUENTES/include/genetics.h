#ifndef GENETICS_H
#define GENETICS_H

#include <iostream>
#include <vector>
#include "global.h"



using namespace std;


/*
	Esquema de evolución generacional con elitismo.
	Se selecciona una población del mismo tamaño que la actual.
	Se realizan tantos torneos binarios como tamaño de la población y nos quedamos siempre
	con el mejor.
	Para mantener el elitismo, si el mejor de  la población no está en la de los padres, sustituye
	al peor de esta.
*/

vector <float> AGG_BLX(vector <Instance> & train, vector <Instance> & test, float & tasa, float & time1, float & t_train, float & red1, float & agr1);

/*
	Esquema de evolución generacional con elitimos.
	La diferencia es el operador de cruce seleccionado.
*/
vector <float> AGG_CA(vector <Instance> & train, vector <vector <float> > & generation, vector < vector <float> >  & second_generation);

/*
	Esquema de evolución estacionario.
	Se selecciona una población del mismo tamaño que la actual.
	Se realizan tantos torneos binarios como tamaño de la población y nos quedamos siempre
	con el mejor.
	Para mantener el elitismo, si el mejor de  la población no está en la de los padres, sustituye
	al peor de esta.

*/

vector <float> AGE_BLX(vector <Instance> & train, vector <vector <float> > & generation);

/*
	Esquema de evolución generacional con elitimos.
	La diferencia es el operador de cruce seleccionado.
*/
vector <float> AGE_CA(vector <Instance> & train, vector <vector <float> > & generation);



/*
	Cruce BLX-a con a = 0.3
	Dados dos cromosomas, genera dos descendientes.
	Los almacena en los mismos vectores q le pasamos como parametro.
*/
void BLX(vector <float> & first, vector <float> & second);



/*
	Operador de cruce aritmético. Devuelve el descendiente
*/
vector <float> CA(vector <float> & first, vector <float> & second);


/*

	Operador de mutación.
*/
void mutate(vector <float> & crom, int pos_gen);

/*
	Gener una genración de 30 cromosomas
*/
vector <vector<float>> generate_generation(vector <Instance> & problem);

/*
	Ejecuta el método AGG-BLX
*/

void exe_AGGBLX(vector <Instance> & train, vector <Instance> & test, float & tasa, float & time1, float & t_train, float & red1, float & agr1);

/*
	Ejecuta el método AGE-BLX
*/

void exe_AGEBLX(vector <Instance> & train, vector <Instance> & test, float & tasa, float & time1, float & t_train);



/*
	Ejecuta el método AGG-CA
*/

void exe_AGGCA(vector <Instance> & train, vector <Instance> & test, float & tasa, float & time1, float & t_train);


/*
	Ejecuta el método AGE-CA
*/

void exe_AGECA(vector <Instance> & train, vector <Instance> & test, float & tasa, float & time1, float & t_train);

#endif