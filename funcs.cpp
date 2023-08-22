#include "Header.h"

ENEMY** InitAqua(int row, int col)
{
	ENEMY** ocean = (ENEMY**)calloc(row, sizeof(ENEMY*));
	for (int i = 0; i < row; i++)
		ocean[i] = (ENEMY*)calloc(col, sizeof(ENEMY));
	return ocean;
}

ENEMY CreatePeace(enum TypeOfFish type, int row, int col)
{
	ENEMY enemy = {
		(FISH*)malloc(sizeof(FISH)),
		NULL
	};
	*enemy.fish = (FISH){
		rand() % 2,
		rand() % 2,
		PEACE,
		MAX_PEACE_AGE,
		rand() % MAX_NEED_TO_PAIRING_PEACE + 1,
		20,
		30,
		0,
		rand() % row,
		rand() % col,
		rand() % FISH_SPEED + 1,
		0,
		0,
		100
	};
	enemy.fish->deltaToPairing = rand() % enemy.fish->needToPairing + 1;
	return enemy;
}

ENEMY CreateFishChild(enum TypeOfFish type, int x, int y)
{
	ENEMY enemy = CreateFish(type, ROW_NUM, COL_NUM);
	enemy.fish->x = x;
	enemy.fish->y = y;
	return enemy;
}

ENEMY CreatePredator(enum TypeOfFish type, int row, int col)
{

	ENEMY enemy = {
		(FISH*)malloc(sizeof(FISH)),
		NULL
	};
	*enemy.fish = (FISH){
		rand() % 2,
		rand() % 2,
		PREDATOR,
		MAX_PREDS_AGE,
		rand() % MAX_NEED_TO_PAIRING_PEACE + 1,
		20,
		30,
		0,
		rand() % row,
		rand() % col,
		rand() % FISH_SPEED + 1,
		0,
		0,
		100
	};
	enemy.fish->deltaToPairing = rand() % enemy.fish->needToPairing + 1;
	return enemy;
}

ENEMY CreateFish(enum TypeOfFish type, int row, int col)
{
	if (type == PEACE) return CreatePeace(type, row, col);
	else return CreatePredator(type, row, col); 
}

ENEMY CreatePlankton(int row, int col) {
	ENEMY enemy = {
		NULL,
		(PLANKTON*)malloc(sizeof(PLANKTON))
	};
	*enemy.plank = (PLANKTON){
		MAX_PLANKTON_AGE,
		0,
		PLANKTON_SPEED,
		rand() % row,
		rand() % col
	};
	return enemy;
}

ENEMY SpawnPlanktonChild(int x, int y) {
	ENEMY enemy = CreatePlankton(ROW_NUM, COL_NUM);
	enemy.plank->x = x;
	enemy.plank->y = y;
	return enemy;
}

void SpawnEnemy(const char* typeOfMem, ENEMY** ocean, int row, int col)
{
	if (!strcmp(typeOfMem, "PREDATOR")) {
		for (int i = 0; i < START_NUM_PREDS; ++i) {
			ENEMY enemy = CreateFish(PREDATOR, row, col);
			int X = -1;
			int Y = -1;
			if (START_NUM_PREDS < 100) {
				X = rand() % ((int)sqrt(START_NUM_PREDS)) + (ROW_NUM / 2);
				Y = rand() % ((int)sqrt(START_NUM_PREDS)) + (COL_NUM / 2);
			}
			else {
				X = rand() % ((int)sqrt(START_NUM_PREDS - (START_NUM_PREDS % 100))) + (ROW_NUM / 2);
				Y = rand() % ((int)sqrt(START_NUM_PREDS - (START_NUM_PREDS % 100))) + (COL_NUM / 2);
			}
			enemy.fish->x = X;
			enemy.fish->y = Y;
			ocean[X][Y].fish = enemy.fish;
			ocean[X][Y].plank = NULL;
		}
	}
	if (!strcmp(typeOfMem, "PEACE")) {
		for (int i = 0; i < START_NUM_PEACES; ++i) {
			ENEMY enemy = CreateFish(PEACE, row, col);
			int X = -1;
			int Y = -1;
			if (START_NUM_PEACES < 100) {
				X = rand() % ((int)sqrt(START_NUM_PEACES)) + (ROW_NUM / 2);
				Y = rand() % ((int)sqrt(START_NUM_PEACES)) + (COL_NUM / 2);
			}
			else {
				X = rand() % ((int)sqrt(START_NUM_PEACES - (START_NUM_PEACES % 100))) + (ROW_NUM / 2);
				Y = rand() % ((int)sqrt(START_NUM_PEACES - (START_NUM_PEACES % 100))) + (COL_NUM / 2);
			}
			enemy.fish->x = X;
			enemy.fish->y = Y;
			ocean[X][Y].fish = enemy.fish;
			ocean[X][Y].plank = NULL;
		}
	}
	if (!strcmp(typeOfMem, "PLANKTON")) {
		for (int i = 0; i < START_NUM_PLANKTON; ++i) {
			ENEMY enemy = CreatePlankton(row, col);
			int X = -1;
			int Y = -1;
			if (START_NUM_PLANKTON >= 100) {
				X = rand() % ((int)sqrt(START_NUM_PLANKTON - (START_NUM_PLANKTON % 100))) + (ROW_NUM / 2);
				Y = rand() % ((int)sqrt(START_NUM_PLANKTON - (START_NUM_PLANKTON % 100))) + (COL_NUM / 2);
			}
			else {
				X = rand() % ((int)sqrt(START_NUM_PLANKTON)) + (ROW_NUM / 2);
				Y = rand() % ((int)sqrt(START_NUM_PLANKTON)) + (COL_NUM / 2);
			}
			enemy.plank->x = X;
			enemy.plank->y = Y;
			ocean[enemy.plank->x][enemy.plank->y].plank = enemy.plank;
			ocean[X][Y].fish = NULL;
		}
	}
}

void LetsGo(ENEMY** ocean, int row, int col, int x, int y) {
	int** waterAround;
	if (ocean[x][y].plank) { 
		int numAround = 8 * ocean[x][y].plank->speed;
		waterAround = (int**)calloc(numAround, sizeof(int*));
		for (int i = 0; i < 8 * ocean[x][y].plank->speed; i++)
			waterAround[i] = (int*)calloc(3, sizeof(int));     
		int numFree = NumFreeAround(ocean, row, col, x, y, ocean[x][y].plank->speed, waterAround, "GO");
		if (!numFree) {
			for (int i = 0; i < numAround; ++i)
				free(waterAround[i]);
			free(waterAround);
			return;
		}
		int choice = rand() % (8 * ocean[x][y].plank->speed);
		while (waterAround[choice][2] != 0)
			choice = rand() % (8 * ocean[x][y].plank->speed);
		ocean[x][y].plank->x = waterAround[choice][0];
		ocean[x][y].plank->y = waterAround[choice][1];
		ocean[waterAround[choice][0]][waterAround[choice][1]] = ocean[x][y];
		ocean[x][y].plank = NULL;
		for (int i = 0; i < numAround; ++i)
			free(waterAround[i]);
		free(waterAround);
	}
	else if (ocean[x][y].fish)
	{
		int numAround = 8 * ocean[x][y].fish->curSpeed;
		waterAround = (int**)calloc(numAround, sizeof(int*));
		for (int i = 0; i < 8 * ocean[x][y].fish->curSpeed; i++)
			waterAround[i] = (int*)calloc(3, sizeof(int));     
		int numFree = NumFreeAround(ocean, row, col, x, y, ocean[x][y].fish->curSpeed, waterAround, "GO");
		if (!numFree) {
			for (int i = 0; i < numAround; ++i)
				free(waterAround[i]);
			free(waterAround);
			return;
		}
		int choice = rand() % (8 * ocean[x][y].fish->curSpeed);
		while (waterAround[choice][2] != 0)
			choice = rand() % (8 * ocean[x][y].fish->curSpeed);
		ocean[x][y].fish->x = waterAround[choice][0];
		ocean[x][y].fish->y = waterAround[choice][1];
		ocean[waterAround[choice][0]][waterAround[choice][1]] = ocean[x][y];
		ocean[x][y].fish = NULL;
		for (int i = 0; i < numAround; ++i)
			free(waterAround[i]);
		free(waterAround);
	}
}

int FormulaForIndexes(int min, int max, int delta, int n) {
	return min + (n + delta + max - min) % (max - min);
}

void IncreaseNumPlankton(ENEMY** ocean, int row, int col, int x, int y) {
	int** waterAround = (int**)calloc(8 * ocean[x][y].plank->speed, sizeof(int*));
	for (int i = 0; i < 8 * ocean[x][y].plank->speed; i++)
		waterAround[i] = (int*)calloc(3, sizeof(int));    
	int numFree = NumFreeAround(ocean, row, col, x, y, ocean[x][y].plank->speed, waterAround, "PAIR");
	if (!numFree) {
		for (int i = 0; i < 8 * ocean[x][y].plank->speed; ++i)
			free(waterAround[i]);
		free(waterAround);
		return;
	}
	int choice = rand() % 8;        
	while (waterAround[choice][2] != 0)
		choice = rand() % 8;
	ocean[waterAround[choice][0]][waterAround[choice][1]] = SpawnPlanktonChild(waterAround[choice][0], waterAround[choice][1]);
	for (int i = 0; i < 8 * ocean[x][y].plank->speed; ++i)
		free(waterAround[i]);
	free(waterAround);
}

int NumFreeAround(ENEMY** ocean, int row, int col, int x, int y, int speed, int** waterAround, char** option) {
	int numFree;
	if (!strcmp(option, "GO"))
		numFree = CountFree(ocean, row, col, x, y, speed, waterAround);
	else if (!strcmp(option, "PAIR"))
		numFree = CountFree(ocean, row, col, x, y, 1, waterAround);
	return numFree;
}
int CountFree(ENEMY** ocean, int row, int col, int x, int y, int speed, int** waterAround)
{
	int numFree = 0, ind = 0;
	for (int x1 = -speed; x1 <= speed; x1++) {
		for (int y1 = -speed; y1 <= speed; y1++) {
			if (x1 >= -speed + 1 && x1 <= speed - 1 && y1 > -speed && y1 < speed) continue;
			int newX = FormulaForIndexes(0, row, x1, x);
			int newY = FormulaForIndexes(0, col, y1, y);
			if (!ocean[newX][newY].plank && !ocean[newX][newY].fish) {
				++numFree;
				waterAround[ind][0] = newX;
				waterAround[ind][1] = newY;
				waterAround[ind][2] = 0;
				ind++;
			}
			else
			{
				waterAround[ind][0] = newX;
				waterAround[ind][1] = newY;
				waterAround[ind][2] = 1;
				ind++;
			}
		}
	}
	return numFree;
}

void NumAround(ENEMY** ocean, int row, int col, int x, int y, int** objAround, int* numOfFood, int* numOfPartnersM, int* numOfPartnersF, int* numOfWater) {
	int ind = 0;
	for (int x1 = -1; x1 <= 1; ++x1) {
		for (int y1 = -1; y1 <= 1; ++y1) {
			if (!x1 && !y1) continue;
			int newX = FormulaForIndexes(0, row, x1, x);
			int newY = FormulaForIndexes(0, col, y1, y);

			switch (ocean[x][y].fish->typeOfFish)
			{
			case PEACE:
				if (ocean[newX][newY].plank) {
					objAround[ind][0] = newX;
					objAround[ind][1] = newY;
					objAround[ind][2] = 0;
					ind++;
					(*numOfFood)++;
				}
				else if (ocean[newX][newY].fish && ocean[newX][newY].fish->typeOfFish == PEACE)
				{
					if (ocean[newX][newY].fish->sex == MALE) {
						objAround[ind][2] = 1;
						(*numOfPartnersM)++;
					}
					else {
						objAround[ind][2] = 2;
						(*numOfPartnersF)++;
					}
					objAround[ind][0] = newX;
					objAround[ind][1] = newY;
					ind++;

				}
				else if (!ocean[newX][newY].fish && !ocean[newX][newY].plank)
				{
					objAround[ind][0] = newX;
					objAround[ind][1] = newY;
					objAround[ind][2] = -1;
					ind++;
					(*numOfWater)++;
				}
				break;
			case PREDATOR:
				if (ocean[newX][newY].fish && ocean[newX][newY].fish->typeOfFish == PEACE) {
					objAround[ind][0] = newX;
					objAround[ind][1] = newY;
					objAround[ind][2] = 0;
					ind++;
					(*numOfFood)++;
				}
				else if (ocean[newX][newY].fish && ocean[newX][newY].fish->typeOfFish == PREDATOR)
				{
					if (ocean[newX][newY].fish->sex == MALE) {
						objAround[ind][2] = 1;
						(*numOfPartnersM)++;
					}
					else {
						objAround[ind][2] = 2;
						(*numOfPartnersF)++;
					}
					objAround[ind][0] = newX;
					objAround[ind][1] = newY;
					ind++;

				}
				else if (!ocean[newX][newY].fish && !ocean[newX][newY].plank)
				{
					objAround[ind][0] = newX;
					objAround[ind][1] = newY;
					objAround[ind][2] = -1;
					ind++;
					(*numOfWater)++;
				}
				break;
			}
		}
	}
}

void EatAndSex(ENEMY** ocean, int row, int col, int x, int y, int numOfGuys) {
	int numOfFood = 0, numOfPartnersM = 0, numOfPartnersF = 0, numOfWater = 0;
	int choice = 0;
	int** objAround = (int**)malloc(8 * sizeof(int*));
	for (int i = 0; i < 8; i++)
		objAround[i] = (int*)malloc(3 * sizeof(int));
	if (ocean[x][y].fish) {
		enum Sex curSex = ocean[x][y].fish->sex;
		enum TypeOfFish type = ocean[x][y].fish->typeOfFish;
		int isHungry = 1, isPared = 0;
		NumAround(ocean, row, col, x, y, objAround, &numOfFood, &numOfPartnersM, &numOfPartnersF, &numOfWater);
		int visited[8] = { 0 };
		if (ocean[x][y].fish->health <= MIN_HEALTH_FOR_PAIRING) ocean[x][y].fish->priority = EATING;
		else ocean[x][y].fish->priority = PAIRING;
		switch (ocean[x][y].fish->priority)
		{
		case EATING:
			if (numOfFood)
			{
				while (objAround[choice][2] != 0)
					choice = rand() % 8;
				ocean[x][y].fish->x = objAround[choice][0];
				ocean[x][y].fish->y = objAround[choice][1];
				if (ocean[objAround[choice][0]][objAround[choice][1]].plank) free(ocean[objAround[choice][0]][objAround[choice][1]].plank);
				else free(ocean[objAround[choice][0]][objAround[choice][1]].fish);
				ocean[objAround[choice][0]][objAround[choice][1]] = ocean[x][y];
				ocean[x][y].fish = NULL;
				isHungry = 0;   
				ocean[objAround[choice][0]][objAround[choice][1]].fish->health += ocean[objAround[choice][0]][objAround[choice][1]].fish->DeltaWithFood;
				if (ocean[objAround[choice][0]][objAround[choice][1]].fish->health > 100)
					ocean[objAround[choice][0]][objAround[choice][1]].fish->health = 100;
			}
			else
			{
				ocean[x][y].fish->health -= ocean[x][y].fish->deltaWithoutFood;
				if (ocean[x][y].fish->health <= 0)
				{
					free(ocean[x][y].fish);
					ocean[x][y].fish = NULL;   
					for (int i = 0; i < 8; ++i)
						free(objAround[i]);
					free(objAround);
					return;
				}
			}

			if (isHungry && numOfWater)
			{
				if (type == PEACE && numOfGuys >= MAX_NUM_PEACES) break;
				if (type == PREDATOR && numOfGuys >= MAX_NUM_PREDS) break;
				PairForFish(ocean, type, objAround, visited, x, y, numOfPartnersM, &isPared, 1);
				PairForFish(ocean, type, objAround, visited, x, y, numOfPartnersF, &isPared, 2);
			}
			break;
		case PAIRING:
			if (numOfWater)
			{
				if (type == PEACE && numOfGuys >= MAX_NUM_PEACES);
				else if (type == PREDATOR && numOfGuys >= MAX_NUM_PREDS);
				else {
					PairForFish(ocean, type, objAround, visited, x, y, numOfPartnersM, &isPared, 1);
					PairForFish(ocean, type, objAround, visited, x, y, numOfPartnersF, &isPared, 2);
				}
			}

			if (!isPared && numOfFood)
			{
				while (objAround[choice][2] != 0)
					choice = rand() % 8;
				ocean[x][y].fish->x = objAround[choice][0];
				ocean[x][y].fish->y = objAround[choice][1];
				if (ocean[objAround[choice][0]][objAround[choice][1]].plank) free(ocean[objAround[choice][0]][objAround[choice][1]].plank);
				else free(ocean[objAround[choice][0]][objAround[choice][1]].fish);
				ocean[objAround[choice][0]][objAround[choice][1]] = ocean[x][y];
				ocean[x][y].fish = NULL;
				if (x == objAround[choice][0] && y == objAround[choice][1]) {
					int k;
					k = 1;
				}
				ocean[objAround[choice][0]][objAround[choice][1]].fish->health += ocean[objAround[choice][0]][objAround[choice][1]].fish->DeltaWithFood;
				if (ocean[objAround[choice][0]][objAround[choice][1]].fish->health > 100)
					ocean[objAround[choice][0]][objAround[choice][1]].fish->health = 100;
			}
			else
			{
				ocean[x][y].fish->health -= ocean[x][y].fish->deltaWithoutFood;
				if (ocean[x][y].fish->health <= 0)
				{
					free(ocean[x][y].fish);
					ocean[x][y].fish = NULL;    
					for (int i = 0; i < 8; ++i)
						free(objAround[i]);
					free(objAround);
					return;
				}
			}
			break;
		}
	}
	for (int i = 0; i < 8; ++i)
		free(objAround[i]);
	free(objAround);
}

void ItsTimeToDie(ENEMY** ocean, int x, int y) {
	if (ocean[x][y].plank) {
		PLANKTON* planky = ocean[x][y].plank;
		++planky->age;
		if (planky->age >= planky->timeOfLife) {
			free(ocean[x][y].plank);
			ocean[x][y].plank = NULL;
		}
	}
	if (ocean[x][y].fish) {
		FISH* fish = ocean[x][y].fish;
		++fish->age;
		if (fish->age >= fish->timeOfLife) {
			free(ocean[x][y].fish);
			ocean[x][y].fish = NULL;
		}
	}
}

void PairForFish(ENEMY** ocean, enum TypeOfFish type, int** objAround, int* visited, int x, int y, int numOfPartners, int* isPared, int sexID)
{
	int choice = -1, count = 0;

	while (1) {
		choice = rand() % 8;
		if (objAround[choice][2] == sexID)
		{
			if (!visited[choice])
				count++;
			visited[choice] = 1;
			if (ocean[objAround[choice][0]][objAround[choice][1]].fish->curToPairing == ocean[objAround[choice][0]][objAround[choice][1]].fish->needToPairing
				&& ocean[x][y].fish->curToPairing == ocean[x][y].fish->needToPairing)
			{
				while (objAround[choice][2] != -1)
					choice = rand() % 8;
				(*isPared) = 1;
				ocean[objAround[choice][0]][objAround[choice][1]] = CreateFishChild(type, objAround[choice][0], objAround[choice][1]);
				ocean[objAround[choice][0]][objAround[choice][1]].fish->curToPairing = ocean[x][y].fish->curToPairing = 0;
				return;
			}
		}
		if (count == numOfPartners)
			break;
	}
	ocean[x][y].fish->curToPairing += ocean[x][y].fish->deltaToPairing;
	if (ocean[x][y].fish->curToPairing > ocean[x][y].fish->needToPairing)
		ocean[x][y].fish->curToPairing = ocean[x][y].fish->needToPairing;
}

void NumToChar(int num, char* str) {
	int k = 10;
	int count = 1;
	while (num % k != num) {
		k *= 10;
		++count;
	}
	k /= 10;
	char newstr[6] = { 0 };
	int i = 0;
	if (num == 0) newstr[0] = '0';
	while (k != 0) {
		int num1 = num / k;
		num %= k;
		k /= 10;
		newstr[i++] = num1 + 48;
	}

	strcat(str, newstr);
}

void ModelOcean() {
	srand(time(0));
	int windowSize = COL_NUM * ROW_NUM;
	ENEMY** ocean = InitAqua(ROW_NUM, COL_NUM);

	Start(ocean, ROW_NUM, COL_NUM, windowSize);
}

sfSprite* CreateSprite(const char* textureFileName)
{
	sfSprite* sprite = sfSprite_create();
	sfSprite_setTexture(sprite, sfTexture_createFromFile(textureFileName, NULL), NULL);
	return sprite;
}

void DrawCreatures(sfRenderWindow* window, sfSprite* peaceSprite, sfSprite* predSprite, sfSprite* plankSprite, int col, int row, ENEMY** ocean)
{
	sfRenderWindow_clear(window, sfCyan);
	for (int x = 0; x < row; ++x) {
		for (int y = 0; y < col; ++y) {
			if (ocean[x][y].fish) {
				if (ocean[x][y].fish->typeOfFish == PEACE) {
					sfSprite_setPosition(peaceSprite, (sfVector2f) { y * 10, x * 10 });
					sfRenderWindow_drawSprite(window, peaceSprite, NULL);
				}
				else {
					sfSprite_setPosition(predSprite, (sfVector2f) { y * 10, x * 10 });
					sfRenderWindow_drawSprite(window, predSprite, NULL);
				}
			}
			else if (ocean[x][y].plank) {
				sfSprite_setPosition(plankSprite, (sfVector2f) { y * 10, x * 10 });
				sfRenderWindow_drawSprite(window, plankSprite, NULL);
			}
		}
	}
}

void Start(ENEMY** ocean, int row, int col, int windowSize)
{
	sfRenderWindow* window = 
		sfRenderWindow_create((sfVideoMode) { col * 10, row * 10, 32 }, "window", sfClose | sfTitlebar, NULL);

	sfEvent event;

	sfSprite* predSprite = CreateSprite("PredTexture.png");
	sfSprite* plankSprite = CreateSprite("PlankTexture.png");
	sfSprite* peaceSprite = CreateSprite("PeaceTexture.png");

	int step = 0;

	if (START_NUM_PLANKTON)
		SpawnEnemy("PLANKTON", ocean, row, col);

	int numOfPlanks = 0;
	int numOfPreds = 0;
	int numOfPeaces = 0;

	while (sfRenderWindow_isOpen(window)) {
		while (sfRenderWindow_pollEvent(window, &event)) {
			if (event.type == sfEvtClosed) {
				sfRenderWindow_close(window);
			}
		}
		if (step == NUM_STEPS_FOR_PEACES && START_NUM_PEACES)
			SpawnEnemy("PEACE", ocean, row, col);

		if (step == NUM_STEPS_FOR_PREDS && START_NUM_PREDS)
			SpawnEnemy("PREDATOR", ocean, row, col);

		for (int x = 0; x < row; ++x)
			for (int y = 0; y < col; ++y) {
				LetsGo(ocean, row, col, x, y);
			}

		for (int x = 0; x < row && step; ++x) 
			for (int y = 0; y < col; ++y)
				if (ocean[x][y].plank) {
					IncreaseNumPlankton(ocean, row, col, x, y);
					++numOfPlanks;
				}

		for (int x = 0; x < row && step % 2 == 0; ++x)
			for (int y = 0; y < col; ++y)
				if (ocean[x][y].fish) {
					if (ocean[x][y].fish->typeOfFish == PEACE) {
						EatAndSex(ocean, row, col, x, y, numOfPeaces);
					}
				}

		for (int x = 0; x < row && step % 4 == 0; ++x)
			for (int y = 0; y < col; ++y)
				if (ocean[x][y].fish) {
					if (ocean[x][y].fish->typeOfFish == PREDATOR) {
						EatAndSex(ocean, row, col, x, y, numOfPreds);
					}
				}

		for (int x = 0; x < row; ++x)
			for (int y = 0; y < col; ++y)
				ItsTimeToDie(ocean, x, y);

		numOfPlanks = 0;
		numOfPreds = 0;
		numOfPeaces = 0;

		for (int x = 0; x < row; ++x) {
			for (int y = 0; y < col; ++y) {
				if (ocean[x][y].fish && ocean[x][y].fish->typeOfFish == PEACE) ++numOfPeaces;
				if (ocean[x][y].fish && ocean[x][y].fish->typeOfFish == PREDATOR) ++numOfPreds;
				if (ocean[x][y].plank) ++numOfPlanks;
			}
		}

		DrawCreatures(window, peaceSprite, predSprite, plankSprite, col, row, ocean);

		char title[100] = { 0 };
		strcat(title, "Generation: ");
		NumToChar(step, title);
		strcat(title, " Planktons: ");
		NumToChar(numOfPlanks, title);
		strcat(title, " Peaces: ");
		NumToChar(numOfPeaces, title);
		strcat(title, " Predators: ");
		NumToChar(numOfPreds, title);

		sfWindow_setTitle(window, title);
		sfRenderWindow_display(window);

		if (step > 30 && (!numOfPlanks || !numOfPeaces || !numOfPreds)) break;

		step++;
	}

	while (sfRenderWindow_isOpen(window)) {
		while (sfRenderWindow_pollEvent(window, &event)) {
			if (event.type == sfEvtClosed) {
				sfRenderWindow_close(window);
			}
		}
	}

	sfRenderWindow_destroy(window);
	return;
}