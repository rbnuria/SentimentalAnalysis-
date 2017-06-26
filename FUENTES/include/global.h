#ifndef GLOBAL_H
#define GLOBAL_H

#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <string>


#define INF 1/0.0


using namespace std;




/*
	Clase instance: almacenamos los valores y la etiqueta de cada una de las instancias del problema.

*/
class Instance
{
public:
	vector<float> values;
	float label; 

	/*
		uctor de parámetros valores y etiqueta
	*/
	Instance(vector<float> values_, float label_);
	
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
void readFile(char * name_file, vector <Instance> & d);




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





void generateNeighbour(vector <float> & solution, vector <float> & new_solution, int &  pos);


/*
	Random pos, devuelve el valor de una posicion del vector elegida de forma aleatoria
	Devuelve el valor almacenado en la componente y lo elimina
*/

int randomPos(vector <int> & vec);

//Truncamos vector de pesos los que son < 0.1å
vector <float> truncate(vector <float> & sol);



#endif







