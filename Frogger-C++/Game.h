#pragma once
#include <math.h>
#include "Player.h"
#include "Enemy.h"
class Game
{
	bool onetime = true;
	Player * player=nullptr;
	bool player_initialized=false;
	bool loc_enemy[15];
	int numOfEnemys = 15;
	bool enemys_alive = false;
	Enemy* enemys[15];
	float base_pos = 615;
	void spownEnemy(int start);
	void checkEnemy();
	bool checkCollision();
	float dxCal(float a, float b);
	
	

public:
	
	void update();
	void draw();
	void init();
	Game();
	~Game();
};
