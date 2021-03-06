#include <iostream>
#include <vector>
#include <fstream>
#include <random>

#include "global.h"


using namespace std;


Instance::Instance(vector<float> values_, string label_)
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




void readFile(char * name_file, vector <Instance> & d, bool front)
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
		string label; //Etiquetas
		string label_f; 
		string label_b;
		bool first = true;

		for(unsigned i = 0 ; i < line.size() ; i++)
		{
			if(line[i] == ',')
			{
				vector_aux.push_back(atof(aux.c_str()));
				
				if(first){ //Guardamos el primer valor por si es la etiqueta
					label_f = aux;
					first = false;
				}

				aux.clear();
			}else
			{
				aux.push_back(line[i]);
			}
		}

		//Guardamos el último valor por si es la etiqueta
		label_b = aux;
		vector_aux.push_back(atof(aux.c_str()));

		//Guardamos la etiqueta
		if(front){ 
			label = label_f;
			//Quitamos la etiqueta del vector
			vector_aux.erase(vector_aux.begin());
		}else{
			label = label_b;
			//Quitamos la etiqueta del vector
			vector_aux.pop_back();
		}	
		
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
		if(0 <= estimated_value <= 0.33){
			estimated_value = 0;
		}else if(estimated value < 0.66){
			estimated_value = 0.5;
		}else{
			estimated_value = 1;
		}

		if(estimated_value == data[i].label){
			num_success++;
		}
	}

	return num_success/data.size();

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

//Función que normaliza los datos de {-1,0,1} a {0,0.5,1}
void db_normalized(vector <Instance> & data){
	
	for(unsigned i = 0; i < data.size(); i++){
		for(unsigned j = 0; j < data[i].values.size(); j++){
			
			data[i].values[j] = (data[i].values[j] + 1)/2.0;
		}
	}
}


void particionate(vector <Instance> & data, vector < vector <Instance> > & sets)
{
	vector <int> pos_lab1;
	vector <int> pos_lab2;

	//Identificamos como primera etiqueta la etiqueta del primer elemento
	string label1 = data.at(0).label;

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

//Lo generamos con un aleatorio entre 0,1
default_random_engine generator_random;
uniform_real_distribution<float> distribution_random(0,1);

vector <float> generateRandomSolution(int size){
	vector <float> solution;

	for(int i = 0; i < size; i++){
		float rand_numb = distribution_random(generator_random);
		solution.push_back(rand_numb);
	}


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

void exe_1NN(vector <Instance> & train, vector <Instance> & test, float & tasa, float & time1, float  & red1, float & agr1){
	//Variables auxilaires
	unsigned t0, t1;
	float elapsed_time;


	vector <float> sol;
	//Asignamos al vector sol el tamaño que hace falta y lo inicializamos todo a 0
	for(unsigned i = 0; i < train.at(0).values.size(); i++){
		sol.push_back(1);
	}


	t0 = clock();
	float aux2 = fitness_1NN(train, test);
	t1 = clock();

	
	float aux4 = calculate_tasa_red(sol);
	//float aux2 = calculate_tasa_clas(test, sol);

	float agr = aux2*0.5 + aux4*0.5;

	elapsed_time = (float)((t1-t0)*1.0 / CLOCKS_PER_SEC);

	tasa+=aux2;
	time1+=elapsed_time;
	red1+= aux4;
	agr1+=agr;

	cout << "&" << aux2 << "&" << aux4 << "&" << agr << "&" << elapsed_time;
}

