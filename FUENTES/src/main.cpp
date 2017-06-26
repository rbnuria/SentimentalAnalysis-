#include <iostream>
#include "global.h"
#include "DE.h"

using namespace std;

int main(int argc, char ** argv){

	srand(123456789);
	cout.precision(4);
	cout.setf( std::ios::fixed, std:: ios::floatfield );


	vector <Instance> data;

	readFile("../data.txt", data);


	//Normalizamos los datos en {0,0.5,1}
	db_normalized(data);


	vector <vector <Instance> > set;
	particionate(data, set);


	float train_tasa = 0;
	float test_tasa = 0;
	float tiempo = 0;

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


		exe_DE_rand(train_set, set[i], train_tasa, tiempo, test_tasa);
		cout << endl;

		train_set.clear();
	}


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


		exe_DE_current_best(train_set, set[i], train_tasa, tiempo, test_tasa);
		cout << endl;

		train_set.clear();
	}

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


		exe_DE_best(train_set, set[i], train_tasa, tiempo, test_tasa);
		cout << endl;

		train_set.clear();
	}




}