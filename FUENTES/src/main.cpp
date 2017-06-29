#include <iostream>
#include "global.h"
#include "DE.h"
#include "genetics.h"

using namespace std;

int main(int argc, char ** argv){

	srand(123456789);
	cout.precision(4);
	cout.setf( std::ios::fixed, std:: ios::floatfield );


	vector <Instance> data;

	char * db = argv[1];

	readFile(db, data);


	//Normalizamos los datos en {0,0.5,1}
	db_normalized(data);


	vector <vector <Instance> > set;
	particionate(data, set);

	vector <float> trivial_sol;
	for(unsigned i = 0; i < data[0].values.size(); i++){
		trivial_sol.push_back(1.0/data[0].values.size() * 1.0);
	}



	float train_tasa = 0;
	float test_tasa = 0;
	float tiempo = 0;
	float train_tasa_trivial = 0;
	float test_tasa_trivial = 0;

/*
	cout << "-------- DE-RAND" << endl;
	for(int i = 0; i < 5; i++){
		cout << "Partición " << i+1;
		vector <Instance> train_set;


		for(int j = 0; j <5; j++){
			//Obtenemos tamaño del vector
			if(j != i){
				train_set.insert(train_set.end(), set[j].begin(), set[j].end());
			}
		}


		exe_DE_current_best(train_set, set[i], test_tasa, tiempo, train_tasa);
		cout << endl;

		float aux1 = clasification_fitness(set[i],trivial_sol);
		float aux2 = clasification_fitness(train_set,trivial_sol);

		train_tasa_trivial += aux2;
		test_tasa_trivial += aux1;

		cout << "Solución trivial: \t" << aux1 << "\t" << "    -    "  << aux2 << endl;



		train_set.clear();
	}

	cout << "Media: \t\t" << test_tasa/5.0 << "\t" << tiempo/5.0 <<  "\t" << train_tasa/5.0 << endl;
	cout << "Media trivial: \t\t" << test_tasa_trivial/5.0 << "\t" << "-" <<  "\t" << train_tasa_trivial/5.0 << endl;



	test_tasa_trivial = 0;
	train_tasa_trivial = 0;

	train_tasa = 0;
	test_tasa = 0;
	tiempo = 0;


	cout << "-------- DE-current-to-best" << endl;
	for(int i = 0; i < 5; i++){
		cout << "Partición " << i+1;
		vector <Instance> train_set;


		for(int j = 0; j <5; j++){
			//Obtenemos tamaño del vector
			if(j != i){
				train_set.insert(train_set.end(), set[j].begin(), set[j].end());
			}
		}


		exe_DE_current_best(train_set, set[i], test_tasa, tiempo, train_tasa);
		cout << endl;

		float aux1 = clasification_fitness(set[i],trivial_sol);
		float aux2 = clasification_fitness(train_set,trivial_sol);

		train_tasa_trivial += aux2;
		test_tasa_trivial += aux1;

		cout << "Solución trivial: \t" << aux1 << "\t" << "    -    "  << aux2 << endl;



		train_set.clear();
	}

	cout << "Media: \t\t" << test_tasa/5.0 << "\t" << tiempo/5.0 <<  "\t" << train_tasa/5.0 << endl;
	cout << "Media trivial: \t\t" << test_tasa_trivial/5.0 << "\t" << "-" <<  "\t" << train_tasa_trivial/5.0 << endl;


	test_tasa_trivial = 0;
	train_tasa_trivial = 0;

	train_tasa = 0;
	test_tasa = 0;
	tiempo = 0;

	cout << "-------- DE-best" << endl;
	for(int i = 0; i < 5; i++){
		cout << "Partición " << i+1;
		vector <Instance> train_set;


		for(int j = 0; j <5; j++){
			//Obtenemos tamaño del vector
			if(j != i){
				train_set.insert(train_set.end(), set[j].begin(), set[j].end());
			}
		}


		exe_DE_best(train_set, set[i], test_tasa, tiempo, train_tasa);
		cout << endl;

		float aux1 = clasification_fitness(set[i],trivial_sol);
		float aux2 = clasification_fitness(train_set,trivial_sol);

		train_tasa_trivial += aux2;
		test_tasa_trivial += aux1;

		cout << "Solución trivial: \t" << aux1 << "\t" << "    -     "  << aux2 << endl;



		train_set.clear();
	}

	cout << "Media: \t\t" << test_tasa/5.0 << "\t" << tiempo/5.0 <<  "\t" << train_tasa/5.0 << endl;
	cout << "Media trivial: \t\t" << test_tasa_trivial/5.0 << "\t" << "-" <<  "\t" << train_tasa_trivial/5.0 << endl;



	train_tasa = 0;
	test_tasa = 0;
	tiempo = 0;
*/


	for(int i = 0; i < 5; i++){
		cout << "Partición " << i+1;
		vector <Instance> train_set;


		for(int j = 0; j <5; j++){
			//Obtenemos tamaño del vector
			if(j != i){
				train_set.insert(train_set.end(), set[j].begin(), set[j].end());
			}
		}


		exe_AGGCA(train_set, set[i], test_tasa, tiempo, train_tasa);
		cout << endl;

		float aux1 = clasification_fitness(set[i],trivial_sol);
		float aux2 = clasification_fitness(train_set,trivial_sol);

		train_tasa_trivial += aux2;
		test_tasa_trivial += aux1;

		cout << "Solución trivial: \t" << aux1 << "\t" << "    -     "  << aux2 << endl;



		train_set.clear();
	}

	cout << "Media: \t\t" << test_tasa/5.0 << "\t" << tiempo/5.0 <<  "\t" << train_tasa/5.0 << endl;
	cout << "Media trivial: \t" << test_tasa_trivial/5.0 << "\t" << "-" <<  "\t" << train_tasa_trivial/5.0 << endl;




}