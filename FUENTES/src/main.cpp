#include <iostream>
#include "global.h"
#include "DE.h"

using namespace std;

int main(int argc, char ** argv){

	srand(123456789);
	cout.precision(4);
	cout.setf( std::ios::fixed, std:: ios::floatfield );


	vector <Instance> data;


	readFile("../data2.text", data);

	cout << "Se lee fichero ok" << endl;

	//Normalizamos los datos en {0,0.5,1}
	db_normalized(data);

	vector <vector <Instance> > set;
	particionate(data, set);


	float train_tasa = 0;
	float test_tasa = 0;
	float tiempo = 0;

	for(int i = 0; i < 5; i++){
		cout << "Partición " << i+1;
		vector <Instance> train_set;


		for(int j = 0; j <5; j++){
			//Obtenemos tamaño del vector
			if(j != i){
				train_set.insert(train_set.end(), set[j].begin(), set[j].end());
			}
		}

		cout << "Entro ejecucion" << endl;
		exe_DE_rand(train_set, set[i], train_tasa, tiempo, test_tasa);
		cout << endl;

		train_set.clear();
	}



}