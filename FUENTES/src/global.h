#ifndef GLOBAL_H
#define GLOBAL_H

#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <string>
#include "random.h"


#define INF 1/0.0


using namespace std;




/*
	Clase instance: almacenamos los valores y la etiqueta de cada una de las instancias del problema.

*/
class Instance
{
public:
	vector<float> values;
	string label; 

	/*
		uctor de parámetros valores y etiqueta
	*/
	Instance(vector<float> values_, string label_);
	
	/*
		uctor por defecto
	*/
	Instance(){}

	/*
		Sobrecarga del operador de asignación
	*/
	Instance & operator=(Instance inst);

	bool &operator==(Instance & inst);




};

/*
	Método que lee un archivo de tipo "arff" sin los comentarios.
	Asigna estos valores al parámetro d.
	El booleano nos indica en qué posición se encuentra en el fichero la etiqueta.
*/
void readFile(char * name_file, vector <Instance> & d, bool front);


/*
	Método que calcula la distancia euclídea entre dos instancias componente a componente.
*/
float euclidean_distance(Instance & first, Instance & second);
/*
	Método que calcula la distancia euclídea entre dos vectores de float componente a componente
	usando pesos.
*/
float euclidean_distance(vector <float> & vec1, vector <float> & vec2, vector <float> & weigth);


/*
	Método que calcula la distancia euclídea entre dos vectores de float componente a componente
	usando pesos.
*/
float euclidean_distance(vector <float> & vec1, vector <float> & vec2);

/*
	Método que calcula la distancia de Manhattan entre dos instancias componente a componente.
*/
float manhattan_distance(Instance & first, Instance & second);


float manhattan_distance(float &  first,float & second);


/*
	Método que devuelve la posición en la que se encuentra el mínimo de un vector de float.
	Será utilizado para calcular el mínimo de un vector de distancias por regla general.
*/
int findMin(vector <float> & v);

/*
	Método que devuelve la posición en la que se encuentra el mínimo de un vector de float.
	Será utilizado para calcular el mínimo de un vector de distancias por regla general.
*/
int findMax(vector <float> & v);


/*
	Método 1_NN, particularización del k_nn, que nos sirve para clasificar 
	la validez de una solución propuesta.
	Utilizamos la técnica leave-one-out que no se cuenta a sí mismo porque si no
	la tasa de acierto sería del 100% siempre.
*/
string k1_NN(vector <Instance> & data, vector <float> & problem, vector <float> & weigth);

/*
	Método 1_NN, particularización del k_nn, que nos sirve para clasificar 
	la validez de una solución propuesta.
	Utilizamos la técnica leave-one-out que no se cuenta a sí mismo porque si no
	la tasa de acierto sería del 100% siempre.

	Se ayuda de una matriz de distancias auxiliar para ganar eficiencia.
*/

string k1_NN_matrix(vector<Instance> & data, int & pos , vector < vector <float> >  &matrix);

/*
	Función objetivo (coste) a minimizar. Mide el coste de una solución dada.
	Será el porcentaje de acierto de la función 1-NN con la estrategia leave-one-out.
	Pasamos como argumento la muestra sobre la que trabajamos.
*/

float fitness(vector <Instance> & data, vector<float> & sol);

/*
	Función usada para normalizar el vector de pesos.
*/
void normalized(vector<float> & sol);



/*
	Función usada para normalizar los valores de las características para que no 
	tengan más peso unas que otras
*/

void db_normalized(vector<Instance> & data);


/*
	Función que particiona el conjunto de instancias del problema utilizando la técnica
	5x2, hacemos 5 parejas de <muestra,test>
*/
void particionate(vector <Instance> & data,vector < vector <Instance> > & sets);



/*
	Generar solución aleatoria
*/

void randomSolution(vector <float> & solution);

/*
	Generar una solución aleatoria de un cierto tamaño
*/

vector <float> generateRandomSolution(int size);

/*
	Método que baraja un vector de instancias.
*/
void shuffle(vector <int> &v);


/*
	Método que intercambia dos elementos de un vector
*/
void swap(vector <int> &v, int pos1, int pos2);


void exe_1NN(vector <Instance> & train, vector <Instance> & test, float & tasa, float & time1, float  & red1, float & agr1);

float fitness_1NN(vector <Instance> & train, vector<Instance> & test);


string k1_NN(vector <Instance> & data,vector <float> & problem);

/*Vector auxiliar que calcula la tasa_clas*/
float calculate_tasa_clas(vector <Instance> &  data, vector<float> & sol);

float calculate_tasa_test(vector <Instance> & train, vector <Instance> & test, vector <float> & sol);


/*Vector auxiliar que calcula la tasa_red*/
float calculate_tasa_red(vector <float> & sol);


void generateNeighbour(vector <float> & solution, vector <float> & new_solution, int &  pos);


/*
	Random pos, devuelve el valor de una posicion del vector elegida de forma aleatoria
	Devuelve el valor almacenado en la componente y lo elimina
*/

int randomPos(vector <int> & vec);

//Truncamos vector de pesos los que son < 0.1å
vector <float> truncate(vector <float> & sol);



#endif







