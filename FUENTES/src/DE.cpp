#include <iostream>
#include <random>
#include "DE.h"

using namespace std;


default_random_engine unif_generator_2;
uniform_real_distribution<float> unif_distribution_2(0,1);

void DE_rand(vector <Instance> & train, vector <float> & sol, float CR, float F){
	//Inicializamos la población
	vector <vector <float>> population = initialize(50, sol.size());

	//Evaluaciones de la funcion objetivo
	int eval = 0;
	int dim = sol.size();
	int size_pop = population.size();
	float best_fitness = -10000;
	vector <float> best_sol;
	vector <int> parents;


	vector <float> fit;
	for(unsigned i = 0; i < population.size(); i++){
		float aux_fitness = fitness(train,population[i]);
		fit.push_back(aux_fitness);
		eval++;


		if(aux_fitness > best_fitness){
			best_fitness = aux_fitness;
			best_sol = population[i];
		}
	}


	while(eval < 5000000){

		for(unsigned i = 0; i < size_pop; i++){
			//Seleccionamos tres padres, excluyentes entre sí
			parents = select3Parents(i);
			vector<float> offspring;
			offspring.resize(dim);

			for(unsigned j = 0; j < dim;j++){ //Para cada gen
				if(unif_distribution_2(unif_generator_2) < CR){
					//Aplicamos transformación
					offspring[j] = population[parents[0]][j] + F*(population[parents[1]][j] - population[parents[2]][j]); 
				
				}else{
					//Lo dejamos igual que el padre
					offspring[j] = population[i][j];
				}
			}

			//Normalizamos la solución para que representen tantos por ciento.
			normalize_solution(offspring);

			//Miro si el nuevo hijo es mejor que el padre
			float offspring_fit = fitness(train, offspring);
			eval++;

			if(offspring_fit > fit[i]){
				population[i] = offspring;
				fit[i] = offspring_fit;
			}

			if(offspring_fit > best_fitness){
				best_fitness = offspring_fit;
				best_sol = offspring;
			}

		}
	}

	sol = best_sol;
}




default_random_engine unif_generator_3;
uniform_real_distribution<float> unif_distribution_3(0,1);

void DE_current_best(vector <Instance> & train, vector <float> & sol, float CR, float F){

	//Inicializamos la población
	vector <vector <float>> population = initialize(50, sol.size());

	//Evaluaciones de la funcion objetivo
	int eval = 0;
	int dim = sol.size();
	int size_pop = population.size();
	float best_fitness = -10000;
	vector <float> best_sol;
	int best_index;

	vector <float> fit;
	for(unsigned i = 0; i < population.size(); i++){
		float aux_fitness = fitness(train,population[i]);
		fit.push_back(aux_fitness);
		eval++;

		if(aux_fitness > best_fitness){
			best_fitness = aux_fitness;
			best_sol = population[i];
			best_index = i;
		}
	}

	while(eval < 500000){
		vector <int> parents;

		for(unsigned i = 0; i < size_pop; i++){
			//Seleccionamos tres padres, excluyentes entre sí
			parents = select2Parents(i);
			vector<float> offspring;
			offspring.resize(dim);

			for(unsigned j = 0; j < dim;j++){ //Para cada gen
				if(unif_distribution_3(unif_generator_3) < CR){
					//Aplicamos transformación
					offspring[j] = population[i][j] + F*(population[best_index][j]-population[i][j]) +
					F*(population[parents[0]][j] - population[parents[1]][j] ); 
				}else{
					//Lo dejamos igual que el padre
					offspring[j] = population[i][j];
				}
			}

			//Normalizamos la solución para que representen tantos por ciento.
			normalize_solution(offspring);

			//Miro si el nuevo hijo es mejor que el padre
			float offspring_fit = fitness(train, offspring);
			eval++;

			if(offspring_fit > fit[i]){
				population[i] = offspring;
				fit[i] = offspring_fit;
			}

			if(offspring_fit > best_fitness){
				best_fitness = offspring_fit;
				best_sol = offspring;
				best_index = i;
			}

		}
	}

	sol = best_sol;
}

void DE_best(vector <Instance> & train, vector <float> & sol, float CR, float F){

	//Inicializamos la población
	vector <vector <float>> population = initialize(50, sol.size());

	//Evaluaciones de la funcion objetivo
	int eval = 0;
	int dim = sol.size();
	int size_pop = population.size();
	float best_fitness = -10000;
	vector <float> best_sol;
	int best_index;

	vector <float> fit;
	for(unsigned i = 0; i < population.size(); i++){
		float aux_fitness = fitness(train,population[i]);
		fit.push_back(aux_fitness);
		eval++;

		if(aux_fitness > best_fitness){
			best_fitness = aux_fitness;
			best_sol = population[i];
			best_index = i;
		}
	}

	while(eval < 500000){
		vector <int> parents;

		for(unsigned i = 0; i < size_pop; i++){
			//Seleccionamos tres padres, excluyentes entre sí
			parents = select2Parents(i);
			vector<float> offspring;
			offspring.resize(dim);

			for(unsigned j = 0; j < dim;j++){ //Para cada gen
				if(unif_distribution_3(unif_generator_3) < CR){
					//Aplicamos transformación
					offspring[j] = population[best_index][j] + F*(population[parents[0]][j] - population[parents[1]][j] ); 
				}else{
					//Lo dejamos igual que el padre
					offspring[j] = population[i][j];
				}
			}

			//Normalizamos la solución para que representen tantos por ciento.
			normalize_solution(offspring);

			//Miro si el nuevo hijo es mejor que el padre
			float offspring_fit = fitness(train, offspring);
			eval++;

			if(offspring_fit > fit[i]){
				population[i] = offspring;
				fit[i] = offspring_fit;
			}

			if(offspring_fit > best_fitness){
				best_fitness = offspring_fit;
				best_sol = offspring;
				best_index = i;
			}

		}
	}

	sol = best_sol;
}

vector <vector <float>> initialize(int size1, int size2){
	vector <vector <float>> returned;

	for(int i = 0; i < size1; i++){
		returned.push_back(generateRandomSolution(size2));
	}

	return returned;
}


default_random_engine unif_generator_1;
uniform_int_distribution<int> unif_distribution_1(0,49);

vector <int> select3Parents(int pos){
	int number_1;
	int number_2;
	int number_3;
	vector <int> returned;

	do{
		number_1 = unif_distribution_1(unif_generator_1);
	}while(number_1 == pos);
	returned.push_back(number_1);

	do{
		number_2 = unif_distribution_1(unif_generator_1);
	}while(number_1 == number_2 || number_2 == pos);
	returned.push_back(number_2);
	
	do{
		number_3 = unif_distribution_1(unif_generator_1);
	}while(number_3 == number_1 || number_3 == number_2 || number_3 == pos);
	returned.push_back(number_3);

	return returned;
}



default_random_engine unif_generator_4;
uniform_int_distribution<int> unif_distribution_4(0,49);

vector <int> select2Parents(int pos){
	int number_1;
	int number_2;
	vector <int> returned;

	do{
		number_1 = unif_distribution_4(unif_generator_4);
	}while(number_1 == pos);
	returned.push_back(number_1);

	do{
		number_2 = unif_distribution_4(unif_generator_4);
	}while(number_1 == number_2 || number_2 == pos);
	returned.push_back(number_2);
	
	return returned;
}

void exe_DE_rand(vector <Instance> & train, vector <Instance> & test, float & tasa, float & time1, float & t_train){
	//Variables auxilaires
	unsigned t0, t1;
	float elapsed_time;


	vector <float> sol;

	//Asignamos al vector sol el tamaño que hace falta y lo inicializamos todo a 0
	for(unsigned i = 0; i < train.at(0).values.size(); i++){
		sol.push_back(0);
	}

	t0 = clock();
	DE_rand(train, sol, 0.5, 0.5);

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


void exe_DE_current_best(vector <Instance> & train, vector <Instance> & test, float & tasa, float & time1, float & t_train){
	//Variables auxilaires
	unsigned t0, t1;
	float elapsed_time;


	vector <float> sol;
	//Asignamos al vector sol el tamaño que hace falta y lo inicializamos todo a 0
	for(unsigned i = 0; i < train.at(0).values.size(); i++){
		sol.push_back(0);
	}

	t0 = clock();
	DE_rand(train, sol, 0.5, 0.5);
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

void exe_DE_best(vector <Instance> & train, vector <Instance> & test, float & tasa, float & time1, float & t_train){
	//Variables auxilaires
	unsigned t0, t1;
	float elapsed_time;


	vector <float> sol;
	//Asignamos al vector sol el tamaño que hace falta y lo inicializamos todo a 0
	for(unsigned i = 0; i < train.at(0).values.size(); i++){
		sol.push_back(0);
	}

	t0 = clock();
	DE_rand(train, sol, 0.5, 0.5);
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

