#include <iostream>
#include <vector>
#include <fstream>
#include <random>

#include "global.h"


using namespace std;


Instance::Instance(vector<float> values_, float label_)
{
	values = values_;
	label = label_;
}

Instance &Instance::operator=(Instance inst){
	this->values = inst.values;
	this->label = inst.label;

	return *this;
}

bool &Instance::operator==(Instance & inst){
	bool returned = false;
	if(this->values==inst.values && this->label==inst.label){
		returned = true;
	}

	return returned;
}




void readFile(char * name_file, vector <Instance> & d)
{
	//Abrimos fichero
	ifstream fi(name_file);
	string line;

	while(!fi.eof())
	{
		getline(fi, line);

		//Variables auxiliares
		string aux;
		vector <float> vector_aux;
		float label; //Etiquetas


		for(unsigned i = 0 ; i < line.size() ; i++)
		{
			if(line[i] == ' ')
			{
				vector_aux.push_back(atof(aux.c_str()));

				aux.clear();
			}else
			{
				aux.push_back(line[i]);
			}
		}

		//Guardamos el último valor por si es la etiqueta
		label = atof(aux.c_str());
		
		//Creamos 
		Instance c(vector_aux, label);
		d.push_back(c);
	}

	fi.close();
}


//Utilizamos como función fitness el cálculo del tanto por ciento de acierto.
float fitness(vector <Instance> & data, vector<float> & sol){

	int num_success = 0;
	
	for(unsigned i = 0; i < data.size(); i++){
		float estimated_value=0;

		for(unsigned j = 0; j < data[i].values.size(); j++){
			estimated_value += data[i].values[j] * sol[j];
		}

		//Truncamos el valor estimado [0,0.33] [0.33, 0.66] [0.66,1]
		if(estimated_value <= 0.33){
			estimated_value = 0;
		}else if(estimated_value <= 0.66){
			estimated_value = 0.5;
		}else{
			estimated_value = 1;
		}

		if(estimated_value == data[i].label){
			num_success++;
		}
	}

	return num_success/(data.size() * 1.0);

}

vector <float> truncate(vector <float> & sol){

	vector <float> new_sol;	

	for(unsigned i = 0; i < sol.size(); i++){
		if(sol[i] < 0.1){
			new_sol.push_back(0.0);
		}else{
			new_sol.push_back(sol[i]);
		}
	}

	return new_sol;
}





void normalized(vector<float> & sol){
	//Obtenemos el máximo del vector de pesos 
	float max_weigth = 0;

	for(unsigned i=0; i < sol.size(); i++){
		if(sol[i] > max_weigth){
			max_weigth = sol[i];
		}
	}

	//Normalizamos el vector de pesos w_i en [0,1]
	for(unsigned i = 0; i < sol.size(); i++){
		if(sol[i] < 0){
			sol[i] = 0;
		}else{
			sol[i] = (float)(sol[i] / max_weigth);
		}
	}
}

/* Deprecated.
//Función que normaliza los datos a [0,1]
void db_normalized(vector <Instance> & data){
	
	for(unsigned i = 0; i < data.size(); i++){
		for(unsigned j = 0; j < data[i].values.size(); j++){
			data[i].values[j] = (data[i].values[j] + 1)/2.0;
		}

		data[i].label = (data[i].label + 1)/2.0;
	}
}*/


void db_normalized(vector <Instance> & data){
	
	//Vector de máximos y mínimos para cada característica
	//Tamaño de los vectores (Instance.at(i).values.size())
	float max, min;	
	//Vamos recorriendo característica a característica en cada una de las instancias buscando
	//el máximo y el mínimo
	for(unsigned i = 0; i < data.at(0).values.size(); i++){
		min = 10000000;
		max = -100000;


		for(unsigned j = 0; j < data.size(); j++){
			cout << i << endl;

			//Si hemos encontrado uno más grande
			if(data.at(j).values.at(i) > max){
				max = data.at(j).values.at(i);
			}

			//Si hemos encontrado uno más pequeño
			if(data.at(j).values.at(i) < min){
				min = data.at(j).values.at(i);
			}

		}




		//Una vez encontrado minimo y máximo para la característica "i", la normalizamos

		for(unsigned j = 0; j < data.size(); j++){
			if(max != min){
				data.at(j).values.at(i) = (data.at(j).values.at(i)-min)/(max-min);
			}else{
				data.at(j).values.at(i) = 0.0;
			}




		}	
	}

	//Cambiamos la etiqueta al {0, 0.5, 1}
	for(unsigned i = 0; i < data.size(); i++){
		data[i].label = (data[i].label + 1)/2.0;
	}


}


void particionate(vector <Instance> & data, vector < vector <Instance> > & sets)
{
	vector <int> pos_lab1;
	vector <int> pos_lab2;

	//Identificamos como primera etiqueta la etiqueta del primer elemento
	float label1 = data.at(0).label;

	//Guardamos las posiciones de los vectores según su etiqueta
	for(unsigned i = 0; i < data.size(); i++){
		if(label1 == data.at(i).label){
			pos_lab1.push_back(i);
		}else{
			pos_lab2.push_back(i);
		}
	}

	sets.resize(5);

	//Barajamos ambos vectores
	shuffle(pos_lab1);
	shuffle(pos_lab2);

	//Asignamos la mitad de cada tipo a la muestra y la otra mitad al test
	for(unsigned i = 0; i < pos_lab1.size();i++){
		if(i%5 == 0){
			sets[0].push_back(data.at(pos_lab1.at(i)));
		}else if(i%5 == 1){
			sets[1].push_back(data.at(pos_lab1.at(i)));
		}else if(i%5 == 2){
			sets[2].push_back(data.at(pos_lab1.at(i)));
		}else if(i%5 == 3){
			sets[3].push_back(data.at(pos_lab1.at(i)));
		}else{
			sets[4].push_back(data.at(pos_lab1.at(i)));
		}
	}

	for(unsigned i = 0; i < pos_lab2.size();i++){
		if(i%5 == 0){
			sets[4].push_back(data.at(pos_lab2.at(i)));
		}else if(i%5 == 1){
			sets[3].push_back(data.at(pos_lab2.at(i)));
		}else if(i%5 == 2){
			sets[2].push_back(data.at(pos_lab2.at(i)));
		}else if(i%5 == 3){
			sets[1].push_back(data.at(pos_lab2.at(i)));
		}else{
			sets[0].push_back(data.at(pos_lab2.at(i)));
		}
	}

}


//Lo generamos con un aleatorio entre 0,1
default_random_engine generator_random_1;
uniform_real_distribution<float> distribution_random_1(0,1);
void randomSolution(vector <float> & solution){
	float rand_numb;

	for(unsigned i = 0; i < solution.size(); i++){
		rand_numb = distribution_random_1(generator_random_1);
		//Creo que esto no hace falta nunca.
		if(rand_numb < 0.0){
			rand_numb = 0.0;
		}else if(rand_numb > 1.0){
			rand_numb = 1.0;
		}
		solution.at(i) = rand_numb;
	}

	//normalized(solution);
}

/* PRIMERA VERSION: haciendolo en orden, favoreciendo a los primeros
vector <float> generateRandomSolution(int size){
	vector <float> solution;
	float acumulado = 0;

	for(int i = 0; i < size; i++){
		float rand_numb = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/(1-acumulado)));

		acumulado += rand_numb;
		solution.push_back(rand_numb);

	}

	return solution;
}*/

vector <float> generateRandomSolution(int size){
	vector <float> solution;
	float acumulado = 0;

	for(int i = 0; i < size; i++){
		float rand_numb = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/(1-acumulado)));
		acumulado += rand_numb;
		solution.push_back(rand_numb);

	}

	shuffle(solution.begin(), solution.end(),std::default_random_engine(1));

	return solution;
}



void shuffle(vector <int> &v){
  int changes;
  int c1,c2;

  // Número de intercambios de cartas (de 1 a 2000)
  changes = 20 + (rand() % 1000);

  // Intercambiar cartas

  for(int i=0; i < changes; i++) {

      c1 = rand() % v.size();
      c2 = rand() % v.size();

      swap(v,c1,c2);
  }

}

default_random_engine generator;
normal_distribution<double> distribution(0.0,0.3);
void generateNeighbour(vector <float> & solution, vector <float> & new_solution, int &  pos){

    float rand_numb = distribution(generator);
    float aux;

	//Vaciamos el vector pues puede contener ya una solucion anterior
	new_solution.clear();

    for(unsigned i = 0; i < solution.size(); i++){
    	if(i != pos){
    		new_solution.push_back(solution.at(i));
    	}else{
    		//Normalizamos
    		aux = solution.at(i) + rand_numb;
    		if(aux < 0.0){
    			aux = 0.0;
    		}else if (aux > 1.0){
    			aux = 1.0;
    		}
    		new_solution.push_back(aux);
    	}
    }
}




void swap(vector<int>  & v, int pos1, int pos2){
	int aux = v.at(pos1);
	v.at(pos1) = v.at(pos2);
	v.at(pos2) = aux;
}

int findMin(vector <float> & v){
	float min = INF;
	int pos = 0;
	
	for(unsigned i = 0; i < v.size(); i++){
		if(v.at(i) < min){
			min = v[i];
			pos = i;
		}
	}

	return pos;
}


int findMax(vector <float> & v){
	float max = -9999999;
	int pos;
	
	for(unsigned i = 0; i < v.size(); i++){
		if(v.at(i) > max){
			max = v[i];
			pos = i;
		}
	}

	return pos;
}


