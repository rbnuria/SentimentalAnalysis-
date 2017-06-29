#include <random>
#include "genetics.h"


using namespace std;

/************** ALGORITMOS GENÉTICOS GENERACIONALES ****************/

vector <float> AGG_CA(vector <Instance> & train, vector < vector <float> > & generation, vector < vector <float> > & second_generation){
	int ev_fit = 0;
	vector <float> best_solution;

	//Guardamos en un vector el fitness de todas las soluciones para no llamarlo más
	//veces de las necesarias
	vector <float> fit_first;

 	for(unsigned i = 0; i < generation.size(); i++){
 		ev_fit++;
		fit_first.push_back(fitness(train, generation[i]));
	}


	vector <float> fit_second;

	int nc = (int)((generation.size() * 0.7)); //número de cruces


	int nm = round((generation.size() * generation[0].size() * 0.001));


	//MEJORA: Hacer la probabilidad de mutación muy alta al principio e ir disminuyendo de forma proporcional.
	//int nm = round((generation.size() * generation[0].size() * 0.5)); 



	while(ev_fit < 100000){

		//Elegimos los mejores de la generación para la fase de reemplazamiento
		int max_pos = findMax(fit_first);
		best_solution = generation[max_pos];
		float best_fitness = fit_first[max_pos];

		/////////FASE DE SELECCION

		//Realizamos tantos torneos como individuos en la generacion para conseguir la generacion estacionaria
		for(unsigned i = 0; i < generation.size(); i++){
			//Seleccionamos dos elementos de forma aleatoria de la primera generacion

			int pos1 = (int)(rand()%generation.size());
			int pos2;
			do{
				pos2 = (int)(rand()%generation.size());
			}while(pos1 == pos2); //Los elementos tienen que ser distintos

			//Añadimos a la generacion el mejor de los dos
			if(fit_first[pos1] > fit_first[pos2]){
				second_generation.push_back(generation[pos1]);
				fit_second.push_back(fit_first[pos1]);

			}else{
				second_generation.push_back(generation[pos2]);
				fit_second.push_back(fit_first[pos2]);

			}
		}//Ya tenemos una generacion de padres del mismo tamaño que la de hijos


		/////////////FASE DE CRUCE: 
		//La probabilidad de cruce será de 0.7, elegimos de antemano 
		//los cromosomas q se cruzan y los cruzamos (en vez de generar tantos números aleatorios)

		//Creamos una generación auxiliar para almacenar el resultado de los cruces.

		//Variable auxiliar q usamos para ir modificando la generacion
		int pos = 0;
		vector <float> new_crom;

		vector< vector <float> > cruces;
		//Si es impar lo hacemos par (necesitamos dos para cada cruce.)
		if(nc % 2 == 1){
			nc++;
		}


		for(int i = 0; i < nc ; i++){
			cruces.push_back(second_generation[i]);
		}



		//Lo realizamos el doble de veces pues cada cruce genera un solo individuo
		for(int i = 0; i < nc; i = i+2){
			new_crom = CA(cruces[i], cruces[i+1]);

			second_generation[pos] = new_crom;

			//Actualizamos fitness 
			fit_second[pos] = fitness(train, new_crom);
			ev_fit++;
			pos++;
		}

		//Repetimos el proceso dos veces para obtener los hijos necesarios
		for(int i = 0; i < nc/2; i++){
 			new_crom = CA(cruces[i], cruces[nc-i-1]);

			second_generation[pos] = new_crom;

			//Actualizamos fitness 
			fit_second[pos] = fitness(train, new_crom);
			ev_fit++;
			pos++;
		}

		cruces.clear();

		//Los otros 8 cromosomas restantes se quedan sin cruzar.

		///////////FASE DE MUTACIÓN: 
		//Elegimos de antemano los que se van a mutar.

		//Elegimos con la esperanza matemática el número de mutaciones a realizar.

		int c, gen; //Cromosoma y gen a mutar

		for(int i = 0; i < nm; i++){
			c = (int)(rand()%nc);
			gen = (int)(rand()%second_generation[c].size());

			//Actualizamos el fitness
			mutate(second_generation[c], gen);
			normalize_solution(second_generation[c]);
			ev_fit++;
			fit_second[c] = fitness(train, second_generation[c]);
		}

		///////////FASE DE REEMPLAZAMIENTO:
		//Realizamos un reemplazamiento por elitismo, buscamos el mejor de la primera y lo añadimos
		//a la segunda.


		//Comprobamos si habíamos añadido al mejor de la otra
		bool max_add = false;
		for(unsigned i = 0; i < second_generation.size() && !max_add; i++){
			if(second_generation[i] == best_solution) max_add = true;
		}

		if(!max_add){
			//Buscamos la peor solución de la segunda generación y sustituímos
			int pos_min = findMin(fit_second);
			second_generation[pos_min] = best_solution;
			fit_second[pos_min] = best_fitness;
		}



		//Guardamos las variables de la segunda generación en las de la primera
		generation = second_generation;
		fit_first = fit_second;

		//Las variables de la segunda generación las limpiamos
		second_generation.clear();
		fit_second.clear();

		//MEJORA: Disminuimos la probabilidad de mutación al fnial.
		//nm = nm * 0.9;

	}
	//Buscamos la mejor solución en la última generación y lo devolvemos.
	int max = findMax(fit_first);
	return generation[max];

}


/**************** ALGORITMOS GENÉTICOS ESTACIONARIOS **********************/


vector <float> AGE_CA(vector <Instance> & train, vector <vector <float> > & generation){
	
	int ev_fit = 0;
	vector <float> fit_first;

 	for(unsigned i = 0; i < generation.size(); i++){
 		ev_fit++;
		fit_first.push_back(fitness(train, generation[i]));
	}


	//Generamos 15000 * 2 cromosomas, cada cromosoma tiene train[0].size() genes, luego queremos mutar
	float nm =  2 * generation.at(0).size() * 0.001; //Probabilidad de cruce en una vuelta
	int n_iter = round(1.0/nm); //número de iteraciones necesarias para q la probabilidad sea 1 y mutamos un hijo (el primero por ejemplo)

	int iter_count=0; //Contador de iteraciones para contabilizar las mutaciones


	while(ev_fit < 100000){


		////FASE DE SELECCIÓN: Realizamos dos torneos binarios, de donde elegiremos a los padres que
		//serán posteriormente recombinados

		vector <float> father1;
		vector <float> father2;
		vector <float> father3;
		vector <float> father4;

		//Obtenemos los padres a partir de los cuales obtendremos dos hijos
		int pos1 = (int)(rand()%generation.size());
		int pos2;
		do{
			pos2 = (int)(rand()%generation.size());
		}while(pos1 == pos2); //Hasta que tengamos dos posiciones diferentes
		
		if(fit_first[pos1] > fit_first[pos2]){
			father1 = generation[pos1];
		}else{
			father1 = generation[pos2];
		} 

		pos1 = (int)(rand()%generation.size());
		do{
			pos2 = (int)(rand()%generation.size());
		}while(pos1 == pos2); //Hasta que tengamos dos posiciones diferentes
		
		if(fit_first[pos1] > fit_first[pos2]){
			father2 = generation[pos1];
		}else{
			father2 = generation[pos2];
		}



		int pos3 = (int)(rand()%generation.size());
		int pos4;
		do{
			pos4 = (int)(rand()%generation.size());
		}while(pos3 == pos4); //Hasta que tengamos dos posiciones diferentes
		
		if(fit_first[pos3] > fit_first[pos4]){
			father3 = generation[pos3];
		}else{
			father3 = generation[pos4];
		} 

		pos3 = (int)(rand()%generation.size());
		do{
			pos4 = (int)(rand()%generation.size());
		}while(pos3 == pos4); //Hasta que tengamos dos posiciones diferentes
		
		if(fit_first[pos3] > fit_first[pos4]){
			father4 = generation[pos3];
		}else{
			father4 = generation[pos4];
		}

		//Ya tenemos los dos padres

		//FASE DE CRUCE:
		//Cruzamos los dos padres obtenidos anteriormente, guardando en ello cada uno de los hijos
		//generados tras el cruce

		//Cruzamos a los dos padres obteniendo un único hijo (que duplicaremos más adelante)
		vector <float> son1 = CA(father1, father2);
		vector <float> son2 = CA(father3, father4);

		//FASE DE MUTACIÓN: La probabilidad de mutación es de 0.001 por gen, así creamos una generación
		//Para ello creamos una generación auxiliar

		//Hacemos randon para elegir el gen a mutar
		int gen = (int)(rand()%son1.size());

		if(iter_count == n_iter){
			mutate(son1, gen);
			normalize_solution(son1);

			iter_count = 0;
		}


		//FASE DE REEMPLAZAMIENTO: Los dos padres sustituyen a los dos peores de la población
		//si son ellos pues no. Como ya los habíamos incluído, basta con elimiar los dos peores.

		generation.push_back(son1);
		generation.push_back(son2);


		ev_fit++;
		ev_fit++;
		float aux_fit = fitness(train, son1);
		float aux_fit_2 = fitness(train, son2);
		fit_first.push_back(aux_fit);
		fit_first.push_back(aux_fit_2); 


		

		int pos_min = findMin(fit_first);
		generation.erase(generation.begin() + pos_min);
		fit_first.erase(fit_first.begin() + pos_min);



		//Otra vez 
		pos_min = findMin(fit_first);
		generation.erase(generation.begin() + pos_min);
		fit_first.erase(fit_first.begin() + pos_min);

	}

	//obtener aqui el maximo
	int pos_max = findMax(fit_first);
	return generation[pos_max];

}

default_random_engine uniform_generator;

/****** MÉTODOS AUXILIARES *****/

vector <float> CA(vector <float> & first, vector <float> & second){
	vector <float> aux;

	for(unsigned i = 0; i < first.size(); i++){
		aux.push_back((first[i] + second[i]) / 2.0);
	}

	//MEJORA
	for(unsigned i = 0; i < aux.size(); i++){
		if(aux[i] < 0.1){
			aux[i] = 0;
		}

		if(aux[i] > 0.9){
			aux[i] = 1;
		}

		normalize_solution(aux);
	}


	return aux;
}


default_random_engine generator2;

normal_distribution<double> distribution2(0.0,0.3);

void mutate(vector <float> & crom, int pos_gen){
	float rand_numb = distribution2(generator2);
	float aux = crom[pos_gen] + rand_numb;
	if(aux < 0){
		aux = 0.0;
	}

	if(aux > 1){
		aux = 1.0;
	}

	crom[pos_gen] = aux;


}


vector <vector <float> > generate_generation(vector <Instance> & problem){
	vector <vector <float> > generation;

	//Una generación está formada por 30 cromosomas
	for(int i = 0; i < 30; i++){
		generation.push_back(generateRandomSolution(problem[0].values.size()));
	}

	return generation;
}


/******* MÉTODOS DE EJECUCIÓN *****/

void exe_AGGCA(vector <Instance> & train, vector <Instance> & test, float & tasa, float & time1, float & t_train){



		//Variables auxilaires
	unsigned t0, t1;
	float elapsed_time;


	vector <vector <float>> first  = generate_generation(train);
	vector <vector <float> > second;



	t0 = clock();
	vector <float> sol  = AGG_CA(train, first, second);
	t1 = clock();

	float tasa_test = clasification_fitness(test,sol);
	float tasa_train = clasification_fitness(train,sol);

	elapsed_time = (float)((t1-t0)*1.0 / CLOCKS_PER_SEC);


	tasa+=tasa_test;
	t_train+=tasa_train;
	time1+= elapsed_time;

	cout << "\t" << tasa_test << "\t" << elapsed_time << "\t" << tasa_train;

	cout << "\nSolución obtenida: \t";
	for(unsigned i = 0; i < sol.size(); i++){
		cout << sol[i] << "\t";
	}

}

void exe_AGECA(vector <Instance> & train, vector <Instance> & test, float & tasa, float & time1, float & t_train){


	//Variables auxilaires
	unsigned t0, t1;
	float elapsed_time;


	vector <vector <float>> first  = generate_generation(train);


	t0 = clock();
	vector <float> sol  = AGE_CA(train, first);
	t1 = clock();

	float tasa_test = clasification_fitness(test,sol);
	float tasa_train = clasification_fitness(train,sol);

	elapsed_time = (float)((t1-t0)*1.0 / CLOCKS_PER_SEC);


	tasa+=tasa_test;
	t_train+=tasa_train;
	time1+= elapsed_time;

	cout << "\t" << tasa_test << "\t" << elapsed_time << "\t" << tasa_train;

	cout << "\nSolución obtenida: \t";
	for(unsigned i = 0; i < sol.size(); i++){
		cout << sol[i] << "\t";
	}

}