// AI.cpp : Defines the entry point for the console application.
//
// author : Mohammad Ali Kavian  
//	
#include "stdafx.h"
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <string.h>
using namespace std;

#define MAX 8
#define MAXFITNESSVAL 28
#define MISS 0.00000000001
struct domain {
	double start;
	double end;
};
struct chromosome {
	int  array[8];
	int fitness;
	double probability;
	domain probabilityDomain;
};
vector<chromosome> tempPopulation;
double frand(double fmin, double fmax);
void generateRandomeChromosome(chromosome * temp, int size);
void calculateProbabilityFunc(chromosome * temp, int size);
int calculateFitnessFunc(chromosome temp);
void calculateFitnessFuncGen(chromosome * temp, int size);
void initializeGeneration(chromosome * temp, int size);
void crossOver(chromosome  &first, chromosome  &second);
void mutation(chromosome  &temp);
chromosome randomSelection(chromosome * temp, int size);
void initializeNewGeneration(chromosome * temp, int size);
void copyFromVecToPtr(chromosome * temp , int size);
bool existMaxFitness(chromosome * temp, int size);
chromosome FindmaxFitness(chromosome * temp, int size);
void printResult(chromosome temp);
void printChromosome(chromosome temp);

void main()
{
	int size;
	int limit;
	int cntr = 0;
	bool checker ;
	cout << "Note : Maximize your console window to see the shapes correctly\n";
	cout << "Welcome, this is genetic algorithm in AI for solving 8-Queens problem\n";
	cout << "Please enter size of first population to generate : ";
	cin >> size;
	cout << "please enter a limit for Loop : ";
	cin >> limit;
	chromosome * generation = new chromosome[size];
	srand(time(NULL));
	initializeGeneration(generation , size);
	checker = existMaxFitness(generation, size);


	while (cntr < limit && checker == false ) {
		int z = rand() % 101;
		for (int i = 0; i < size; i++) {
			chromosome x = randomSelection(generation, size);
			chromosome y = randomSelection(generation, size);
			crossOver(x, y);
			
			if ( z >= 0 && z <10 ) {
				mutation(x);
			}
			z = rand() % 101;
			if (z >= 0 && z <10) {
				mutation(y);
			}
			tempPopulation.push_back(x);
			tempPopulation.push_back(y);
			
		}
		
		size = tempPopulation.size() ;
		
		chromosome * tempPtr;
		tempPtr = generation;
		delete[]tempPtr;
		generation = new chromosome[size];
		initializeNewGeneration(generation, size);
		checker = existMaxFitness(generation, size);
		cntr++;
	}
	chromosome result = FindmaxFitness(generation, size);
	cout << "The result chromosome is :\n";
	printChromosome(result);
	cout << "fitness " << result.fitness <<  endl;
	printResult(result);
	
	char endcharacter;
	cin >> endcharacter;
}

void initializeGeneration(chromosome * temp, int size) {
	generateRandomeChromosome(temp , size);
	calculateFitnessFuncGen(temp, size);
	calculateProbabilityFunc(temp, size);
}
void initializeNewGeneration(chromosome * temp, int size) {
	copyFromVecToPtr(temp, size);
	calculateFitnessFuncGen(temp, size);
	calculateProbabilityFunc(temp, size);
}
void generateRandomeChromosome(chromosome * temp , int size) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < 8; j++) {
			temp[i].array[j] = rand() % MAX;
		}
	}
}

int calculateFitnessFunc(chromosome temp) {
	int clashes = 0;
	int columnDifference;
	int rowDifference;
	for (int i = 0; i < MAX; i++) {
		for (int j = i + 1; j < MAX; j++) {
			if (temp.array[i] == temp.array[j])
				clashes++;
			columnDifference = abs(i - j);
			rowDifference = abs(temp.array[i] - temp.array[j]);
			if (columnDifference == rowDifference)
				clashes++;
		}
	}

	return (MAXFITNESSVAL - clashes);
}
void calculateFitnessFuncGen(chromosome * temp, int size) {
	for (int i = 0; i < size; i++) {
		temp[i].fitness = calculateFitnessFunc(temp[i]);
	}
}
void calculateProbabilityFunc(chromosome * temp, int size) {
	int fitnesstotalSum = 0;
	for (int i = 0; i < size; i++) {
		fitnesstotalSum += temp[i].fitness;
	}
	for (int i = 0; i < size; i++) {
		temp[i].probability = ((double)temp[i].fitness / (double)fitnesstotalSum);

		if (i != 0) {
			temp[i].probabilityDomain.start = temp[i - 1].probabilityDomain.end + MISS;
			temp[i].probabilityDomain.end = temp[i].probability + temp[i].probabilityDomain.start;
		}
		else {
			temp[i].probabilityDomain.start = 0;
			temp[i].probabilityDomain.end = temp[i].probabilityDomain.start + temp[i].probability;
		}

	}
	
}

void crossOver(chromosome  &first , chromosome  &second) {
	int point = 1 + rand() % (MAX - 1) ;
	int * tempArray = new int[point];
	for (int i = 0; i < point; i++) {
		tempArray[i] = first.array[i];
		first.array[i] = second.array[i];
		second.array[i] = tempArray[i];
	}
	
}

void mutation(chromosome  &temp) {
	int column = rand() % 8;
	int row = rand() % 8;
	while (temp.array[column] == row) {
		row = rand() % 8;
	}
	temp.array[column] = row;
}

chromosome randomSelection(chromosome * temp , int size) {
	double number = frand(0, 1);
	for (int i = 0; i < size; i++) {
		if ( number >= temp[i].probabilityDomain.start && number <= temp[i].probabilityDomain.end) {
			return temp[i];
		}
	}
	
}
void copyFromVecToPtr(chromosome * temp , int size) {
	int i = 0;
	for (int i = 0; i < size; i++){
		temp[i] = tempPopulation.at(i);
		
	}

	while (!tempPopulation.empty()) {
		tempPopulation.pop_back();
	}

}
bool completeFitness(chromosome * temp , int size) {
	for (int i = 0; i < size; i++) {
		if (temp[i].fitness == MAXFITNESSVAL) {
			return true;
		}
	}
	return false;
}

chromosome FindmaxFitness(chromosome * temp, int size) {
	chromosome max = temp[0];
	for (int i = 1; i < size; i++) {
		if (max.fitness == MAXFITNESSVAL) {
			return max;
		}
		else if (max.fitness < temp[i].fitness) {
			max = temp[i];
		}
	
	}
	return max;
}
bool existMaxFitness(chromosome * temp, int size) {
	int max = temp[0].fitness;
	for (int i = 1; i < size; i++) {
		if (max == MAXFITNESSVAL) {
			return true;
		}
		else if (max < temp[i].fitness) {
			max = temp[i].fitness;
		}

	}
	return false;
}

void printResult(chromosome temp) {
	cout << endl;
	cout << "				" << "  c0  c1  c2  c3  c4  c5  c6  c7 " << endl;
	for (int i = 0; i < MAX; i++) {
		cout << "				" << "---------------------------------" << endl;
		cout << "			      r" << i ;
		for (int j = 0; j < MAX; j++) {
			if (temp.array[j] == i) {
				cout << "| " << "*" << " ";
			}
			else
				cout << "|   " ;
		}
		cout << "|" << endl;
		
	}
	cout << "				" << "---------------------------------" << endl;
}
void printChromosome(chromosome temp) {
	for (int i = 0; i < MAX; i++) {
		cout << temp.array[i] << " ";
	}
	cout << endl;
}
double frand(double fmin, double fmax) {

	double f = (double)rand() / RAND_MAX;
	return fmin + f * (fmax - fmin);
		
	
}