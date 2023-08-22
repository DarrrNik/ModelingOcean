#pragma once
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <Windows.h>
#include <math.h>

#include <SFML/Graphics.h>

#define START_NUM_PREDS 200
#define START_NUM_PEACES 400
#define START_NUM_PLANKTON 500
#define MAX_NEED_TO_PAIRING_PREDS 10
#define MAX_NEED_TO_PAIRING_PEACE 13
#define MAX_PREDS_AGE 38
#define MAX_PEACE_AGE 15
#define MAX_PLANKTON_AGE 4
#define PLANKTON_SPEED 1
#define FISH_SPEED 2
#define ROW_NUM 90
#define COL_NUM 180
#define NUM_STEPS_FOR_PREDS 10
#define NUM_STEPS_FOR_PEACES 2
#define PLANKTON_NUM_CHILDS 1
#define PLANKTON_MAX_NUM_NEIGHBOUR_FOR_CHILDS 4
#define PLANKTON_MAX_NUM_NEIGHBOUR_FOR_GO 3
#define NUM_NEIGHBOUR 8
#define MAX_NUM_PREDS 5000
#define MAX_NUM_PEACES 5000
#define MAX_NUM_PLANKTON 4000
#define MIN_HEALTH_FOR_PAIRING 80
#define DELTA_OF_FOOD 
#define PARTS 9


enum Sex {
	MALE,
	FEMALE
};

enum Priority {
	PAIRING,
	EATING
};

enum TypeOfFish {
	PEACE,
	PREDATOR
};


typedef struct Plankton {
	int timeOfLife;
	int age;
	int speed;
	int x, y;
}PLANKTON;

typedef struct Fish {
	enum Sex sex;
	enum Priority priority;				
	enum TypeOfFish typeOfFish;
	int timeOfLife;
	int needToPairing;		
	int deltaWithoutFood;	
	int DeltaWithFood;
	int deltaToPairing;
	int x, y;
	int curSpeed;
	int curToPairing;		
	int age;
	int health;
}FISH;

typedef struct Enemy {
	FISH* fish;
	PLANKTON* plank;
}ENEMY;

void ModelOcean();

sfSprite* CreateSprite(const char* textureFileName);

void DrawCreatures(sfRenderWindow* window, sfSprite* peaceSprite, sfSprite* predSprite, sfSprite* plankSprite, int col, int row, ENEMY** ocean);

void Start(ENEMY** ocean, int row, int col, int windowSize);

void SpawnEnemy(const char* typeOfMem, ENEMY** ocean, int row, int col);

ENEMY CreateFish(enum TypeOfFish type, int row, int col);

ENEMY CreatePredator(enum TypeOfFish type, int row, int col);

ENEMY CreatePeace(enum TypeOfFish type, int row, int col);

ENEMY CreateFishChild(enum TypeOfFish type, int x, int y);

ENEMY** InitAqua(int row, int col);

ENEMY CreatePlankton(int row, int col);

void LetsGo(ENEMY** ocean, int row, int col, int x, int y);

int FormulaForIndexes(int min, int max, int delta, int n);

void IncreaseNumPlankton(ENEMY** ocean, int row, int col, int x, int y);

ENEMY SpawnPlanktonChild(int x, int y);

int NumFreeAround(ENEMY** ocean, int row, int col, int x, int y, int speed, int** water_around, char** option);

int CountFree(ENEMY** ocean, int row, int col, int x, int y, int speed, int** water_around);

void NumAround(ENEMY** ocean, int row, int col, int x, int y, int** food_around, int* num_of_food, int* num_of_partnersM, int* num_of_partnersF, int* num_of_water);

void EatAndSex(ENEMY** ocean, int row, int col, int x, int y, int numOfGuys);

void ItsTimeToDie(ENEMY** ocean, int x, int y);

void PairForFish(ENEMY** ocean, enum TypeOfFish type, int** obj_around, int* visited, int x, int y, int num_of_partners, int* isPared, int sexID);