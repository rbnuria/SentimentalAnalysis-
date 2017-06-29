#include "memetic.h"
#include <utility>
#include <random>

/***** MÉTODOS AUXILIARES *****/


vector <vector <float> > generate_generation_memetic(vector <Instance> & problem){
	vector <vector <float> > generation;

	//Una generación está formada por 30 cromosomas
	for(int i = 0; i < 10; i++){
		generation.push_back(generateRandomSolution(problem.at(0).values.size()));
	}

	return generation;
}

bool comp(pair <vector<float>, float> & p1, pair <vector<float>, float> & p2){
	if(p1.second > p2.second){
		return true;
	}else{
		return false;
	}
}

float LS_memetic(vector <Instance> & train, vector<float> & sol, int & ev){
	//Generamos solución actual -> Con nuestro algoritmo greedy -> relief	

	vector <float> new_sol;


	//Auxiliares
	int ne_generate = 0;
	float b_sol = fitness(train, sol);
	float n_sol = 0;
	int pos = 0;

	//Creamos un vector de posiciones
	vector <int> p;
	for(int i = 0; i < sol.size(); i++){
		p.push_back(i);
	}

	//Sol ahora mismo tiene la solución "inicial" -> greedy

	while(ne_generate < 5 * sol.size()){
			
		//Generamos nuevo vecino cambiando la característica de la posición elegida
		generateNeighbour(sol, new_sol, p[pos]);

		//Contabilizamos las llamadas a la función objetivo
		ev++;
		n_sol = fitness(train, new_sol);

		if(n_sol > b_sol){
			sol = new_sol;
			b_sol = n_sol;
		}

		if(pos < sol.size() - 1){
			pos++;
			ne_generate++;
		}else{
			pos = 0;
			shuffle(p.begin(), p.end(), default_random_engine(1));
			ne_generate++;
		}

	}

	return b_sol; 
}


/************ ALGORITMOS MEMÉTICOS ***********/

vector <float> AM_10_1(vector <Instance> & train, vector <vector <float> > & generation,  vector < vector <float> >  & second_generation){
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
	int n_generation = 0;
	
	int nc = (int)((generation.size() * 0.7)); //número de cruces
	//Elegimos con la esperanza matemática el número de mutaciones a realizar.
	int nm = round((generation.size() * generation[0].size() * 0.001));


	while(ev_fit < 150000){


		//Cada 10 generaciones aplicamos búsqueda local
		if(n_generation == 10){
			for(unsigned i = 0; i < generation.size(); i++){
				//Cambia la solucion y cambia el fitness de la solucion obtenida
				fit_first[i] = LS_memetic(train, generation.at(i), ev_fit);
				normalize_solution(generation[i]);

			}

			//Empezamos a contar de 0.
			n_generation = 0;

		}

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

			//Añadimos a la generacion el mejor de los dos y guaradmos sus fitness
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

		for(int i = 0; i < nc; i = i+2){
			//Los cruzamos el 70% de los cromosomas (los primeros)
			CA(second_generation[i], second_generation[i+1]);
			//Actualizamos fitness 
			ev_fit++;
			fit_second[i] = fitness(train, second_generation[i]);
			ev_fit++;
			fit_second[i+1] = fitness(train, second_generation[i+1]);
		}

		///////////FASE DE MUTACIÓN: 
		//Elegimos de antemano los que se van a mutar.


		int c, gen; //Cromosoma y gen a mutar


		for(int i = 0; i < nm; i++){
			c = (int)(rand()%second_generation.size());
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
			if(second_generation[i] == best_solution){
				max_add = true;
			}
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

		n_generation++;

	}

	//Buscamos la mejor solución en la última generación y lo devolvemos.
	int max = findMax(fit_first);

	return generation[max];



}






vector <float> AM_10_01(vector <Instance> & train, vector <vector <float> > & generation,  vector < vector <float> >  & second_generation){
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
	int n_generation = 0;
	

	int nls = generation.size() * 0.1;

	int nc = (int)((generation.size() * 0.7)); //número de cruces
	//Elegimos con la esperanza matemática el número de mutaciones a realizar.
	int nm = round((generation.size() * generation[0].size() * 0.001));


	while(ev_fit < 150000){

		//Cada 10 generaciones aplicamos búsqueda local
		if(n_generation == 10){

			//Barajamos el vector de la generación para no coger siempre los mismos.
			shuffle(generation.begin(), generation.end(), default_random_engine(7));
		
			//Aplicamos tantas ls como necesitemos (nls)
			for(unsigned i = 0; i < nls; i++){
				//Actualizamos la solución y también el fitness
				fit_first[i] = LS_memetic(train, generation.at(i), ev_fit);
				normalize_solution(generation[i]);
			}

			//Empezamos a contar de 0.
			n_generation = 0;
		}

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

			//Añadimos a la generacion el mejor de los dos y guaradmos sus fitness
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

		for(int i = 0; i < nc; i = i+2){
			//Los cruzamos el 70% de los cromosomas (los primeros)
			CA(second_generation[i], second_generation[i+1]);
			//Actualizamos fitness 
			ev_fit++;
			fit_second[i] = fitness(train, second_generation[i]);
			ev_fit++;
			fit_second[i+1] = fitness(train, second_generation[i+1]);
		}

		///////////FASE DE MUTACIÓN: 
		//Elegimos de antemano los que se van a mutar.


		int c, gen; //Cromosoma y gen a mutar


		for(int i = 0; i < nm; i++){
			c = (int)(rand()%second_generation.size());
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
			if(second_generation[i] == best_solution){
				max_add = true;
			}
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

		n_generation++;
	}



	//Buscamos la mejor solución en la última generación y lo devolvemos.
	int max = findMax(fit_first);

	return generation[max];



}


vector <float> AM_10_01mej(vector <Instance> & train, vector <vector <float> > & generation,  vector < vector <float> >  & second_generation){
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
	int n_generation = 0;

	int nls = generation.size() * 0.3;
	
	int nc = (int)((generation.size() * 0.7)); //número de cruces
	//Elegimos con la esperanza matemática el número de mutaciones a realizar.
	int nm = round((generation.size() * generation[0].size() * 0.001));


	while(ev_fit < 150000){

		//Cada 10 generaciones aplicamos búsqueda local
		if(n_generation == 10){

			//Ordenamos el vector para aplicarlo con los mejores.
			vector <pair <vector <float> , float> > pair_generation;
			pair < vector<float> , float> aux;
			for(int i = 0; i < generation.size(); i++){
				aux = make_pair(generation.at(i), fit_first.at(i));
				pair_generation.push_back(aux);
			}

			//Los ordenados
			sort(pair_generation.begin(), pair_generation.end(), comp);

			//Volvemos los vectores a su sitio
			for(int i = 0; i < generation.size(); i++){
				generation.at(i) = pair_generation.at(i).first;
				fit_first.at(i) = pair_generation.at(i).second;
			}

			//Ya tenemos en generation y en fit_first ordenados según fitness.
		
			//Aplicamos tantas ls como necesitemos (nls)
			for(unsigned i = 0; i < nls; i++){
				//Actualizamos la solución y también el fitness
				fit_first[i] = LS_memetic(train, generation.at(i), ev_fit);
				normalize_solution(generation[i]);
			}

			//Empezamos a contar de 0.
			n_generation = 0;
		}

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

			//Añadimos a la generacion el mejor de los dos y guaradmos sus fitness
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

		for(int i = 0; i < nc; i = i+2){
			//Los cruzamos el 70% de los cromosomas (los primeros)
			CA(second_generation[i], second_generation[i+1]);
			//Actualizamos fitness 
			ev_fit++;
			fit_second[i] = fitness(train, second_generation[i]);
			ev_fit++;
			fit_second[i+1] = fitness(train, second_generation[i+1]);
		}

		///////////FASE DE MUTACIÓN: 
		//Elegimos de antemano los que se van a mutar.


		int c, gen; //Cromosoma y gen a mutar


		for(int i = 0; i < nm; i++){
			c = (int)(rand()%second_generation.size());
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
			if(second_generation[i] == best_solution){
				max_add = true;
			}
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

		n_generation++;

	}


	//Buscamos la mejor solución en la última generación y lo devolvemos.
	int max = findMax(fit_first);

	return generation[max];



}


/***** MÉTODOS PARA LA EJECUCIÓN *****/


void exe_AM_10_1(vector <Instance> & train, vector <Instance> & test, float & tasa, float & time1, float & t_train){
	//Variables auxilaires

	unsigned t0, t1;
	float elapsed_time;


	vector <vector <float>> first  = generate_generation(train);
	vector <vector <float> > second;



	t0 = clock();
	vector <float> sol  = AM_10_1(train, first, second);
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

void exe_AM_10_01(vector <Instance> & train, vector <Instance> & test, float & tasa, float & time1, float & t_train){
	//Variables auxilaires

	unsigned t0, t1;
	float elapsed_time;


	vector <vector <float>> first  = generate_generation(train);
	vector <vector <float> > second;



	t0 = clock();
	vector <float> sol  = AM_10_01(train, first, second);
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

void exe_AM_10_01mej(vector <Instance> & train, vector <Instance> & test, float & tasa, float & time1, float & t_train){
	//Variables auxilaires

	unsigned t0, t1;
	float elapsed_time;


	vector <vector <float>> first  = generate_generation(train);
	vector <vector <float> > second;



	t0 = clock();
	vector <float> sol  = AM_10_01mej(train, first, second);
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
