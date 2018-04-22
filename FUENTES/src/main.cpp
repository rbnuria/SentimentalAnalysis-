#include <iostream>

using namespace std;

int main(int argc, char ** argv){

	srand(123456789);
	cout.precision(4);
	cout.setf( std::ios::fixed, std:: ios::floatfield );


	vector <Instance> data;

	readFile("../data/data.arff", data, false);
	//Normalizamos los datos en {0,0.5,1}
	db_normalized(data);

	vector <vector <Instance> > set;
	particionate(data, set);


	


}