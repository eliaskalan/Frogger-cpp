#include "Game.h"
#include "graphics.h"
#include "Config.h"

void Game::update()
{
	if (!player_initialized && graphics::getGlobalTime() > 1500)
	{
		river = new RiverCollision(*this);
		player = new Player(*this);
		player_initialized = true;

	}
	if (player)
		player->update();


	checkEnemy();
	spownEnemy(0);
	checkTurtles();
	spownTurtles(0);
	if (graphics::getGlobalTime() > 2500)
	{
		spownEnemy(5);

	}
	if (graphics::getGlobalTime() > 5000)
	{
		spownEnemy(10);
		spownTurtles(10);
	}
	for (int i = 0; i < numOfEnemys; ++i)
	{
		if (enemys[i])
		{
			enemys[i]->update();
		}
	}
	for (int i = 0; i < numOfTurtles; ++i)
	{
		if (turtle[i])
		{
			turtle[i]->update();

		}
	}


	if (checkEnemyCollision() || checkRiverPlayerCollision()||checkFinishCollision())
	{
		if (player->getPosY() <= 55)
		{
			score += 100;
		}
		delete player;
		player = nullptr;
		player = new Player(*this);
		player_initialized = true;
	}
	checkTurtlePlayerCollision();

}

void Game::draw()
{
	graphics::Brush br;
	br.texture = std::string(ASSET_PATH) + "frogger-background.png";
	br.outline_opacity = 0.0f;

	//draw background
	graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_WIDTH, br);
	for (int i = 0; i < numOfTurtles; ++i)
	{
		if (turtle[i])
		{
			turtle[i]->draw();

		}
	}
	//draw player
	if (player)
		player->draw();

	for (int i = 0; i < numOfEnemys; ++i)
	{
		if (enemys[i])
		{
			enemys[i]->draw();

		}
	}

	//UI INFO for debug

	if (player)
	{
		char time[10];

		br.fill_color[0] = 0.f;
		br.fill_color[1] = 0.f;
		br.fill_color[2] = 0.f;
		sprintf_s(time, "(%2.0f)", (60000 - graphics::getGlobalTime()) / 1000);
		graphics::drawText(CANVAS_WIDTH - 60, 40, 20, time, br);
		graphics::resetPose();
		river->draw();
		char info[40];
		sprintf_s(info, "(%f,%f)", player->getPosX(), player->getPosY());
		graphics::drawText(50, 50, 20, info, br);
		char scoref[10];
		sprintf_s(scoref, "(%2.0f)", score);
		graphics::drawText(50, 80, 20, scoref, br);
		graphics::resetPose();
	}






}
void Game::spownTurtles(int start)
{

	base_t_pos = 265;
	float directionSetter = -1.f;
	for (int i = start; i < start + 5; ++i)
	{
		if (!loc_turtle[i])
		{
			turtle[i] = new Turtle(*this);
			loc_turtle[i] = true;
			turtle[i]->set_diraction(directionSetter);
			if (i % 3 == 0) {

				turtle[i]->setSpeed(0.077);
			}
			if (i % 4 == 0) {

				turtle[i]->setSpeed(0.083);
			}
			if (directionSetter == 1.f)
			{

				turtle[i]->set_x(0);
			}
			else
			{

				turtle[i]->set_x(CANVAS_WIDTH + 50);
			}

			turtle[i]->set_y(base_t_pos - (i - start) * 45);

		}
		directionSetter *= -1.f;

	}


}


//mabe must go to the ~Game
void Game::spownEnemy(int start)
{
	//maybe if must leave an numOfEnemys become static 
	int depos_y = -1;
	float directionSetter = -1.f;
	for (int i = start; i < start + 5; ++i)
	{
		if (!loc_enemy[i])
		{
			enemys[i] = new Enemy(*this);
			loc_enemy[i] = true;
			if (i % 3 == 0) {
				char colo[40] = "pixil-frame-1.png";
				enemys[i]->setCarColor(colo);
				enemys[i]->setSpeed(0.1020);
			}
			if (i % 4 == 0) {
				char colo[40] = "pixil-frame-2.png";
				enemys[i]->setCarColor(colo);
				enemys[i]->setSpeed(0.1015);
			}
			enemys[i]->set_diraction(directionSetter);
			if (directionSetter == 1.f)
			{
				enemys[i]->set_x(0);
			}
			else
			{
				enemys[i]->set_x(CANVAS_WIDTH + 50);
			}
			enemys[i]->set_y(base_pos - (i - start) * 60);


		}
		directionSetter *= -1.f;
	}
	base_pos = 615;
	directionSetter = -1.f;
}
void Game::checkEnemy()
{

	for (int i = 0; i < numOfEnemys; ++i)
	{
		if (enemys[i] && !enemys[i]->getState())
		{
			delete enemys[i];
			enemys[i] = nullptr;
			loc_enemy[i] = false;
		}
	}


}
bool Game::checkRiverTurtleCollision(int i, int j)
{
	if (!player)
		return false;
	for (int l = 0; l < numOfTurtles; ++l)
	{
		if (loc_turtle[l])
		{
			Disk d1 = turtle[l]->getCollisionHull(20, 0, 3.f);
			Disk d2 = turtle[l]->getCollisionHull(-20, 0, 3.f);
			Disk d3 = turtle[l]->getCollisionHull(55, 0, 3.f);
			Disk de = river->getCollisionHull(i * 60, -j * 45, 4.f);
			float dx1;
			float dx2;
			float dx3;
			dx1 = dxCal(d1.cx, de.cx);
			dx2 = dxCal(d2.cx, de.cx);
			dx3 = dxCal(d3.cx, de.cx);
			float dy1;
			float dy2;
			float dy3;
			dy1 = dxCal(d1.cy, de.cy);
			dy2 = dxCal(d2.cy, de.cy);
			dy3 = dxCal(d3.cy, de.cy);
			if (sqrt(dx1 * dx1 + dy1 * dy1) < d1.radius + de.radius || sqrt(dx2 * dx2 + dy2 * dy2) < d2.radius + de.radius || sqrt(dx3 * dx3 + dy3 * dy3) < d3.radius + de.radius)
				return true;

		}
	}
	return false;



}
void Game::checkTurtles()
{

	for (int i = 0; i < numOfTurtles; ++i)
	{
		if (turtle[i] && !turtle[i]->getState())
		{
			delete turtle[i];
			turtle[i] = nullptr;
			loc_turtle[i] = false;
		}
	}


}

bool Game::checkEnemyCollision()
{

	for (int i = 0; i < numOfEnemys; ++i)
	{
		if (!player || !enemys[i])
		{
			return false;
		}

		Disk d1 = player->getCollisionHull();
		//de=disk x of enemy 
		Disk de[2];
		de[0] = enemys[i]->getCollisionHull(10, -3, 4.5f);
		de[1] = enemys[i]->getCollisionHull(10, -3, 4.5f);
		float dx1;
		float dx2;
		dx1 = dxCal(d1.cx, de[0].cx);
		dx2 = dxCal(d1.cx, de[1].cx);
		float dy1;
		float dy2;
		dy1 = dxCal(d1.cy, de[0].cy);
		dy2 = dxCal(d1.cy, de[1].cy);


		if (sqrt(dx1 * dx1 + dy1 * dy1) < d1.radius + de[0].radius || sqrt(dx2 * dx2 + dy2 * dy2) < d1.radius + de[1].radius)
			return true;

	}
	return false;
}

/**
bool Game::checkTurtleCollision()
{

	for (int i = 0; i < numOfTurtles; ++i)
	{
		if (!player || !turtle[i])
		{
			return false;
		}

		Disk d1 = player->getCollisionHull();
		//de=disk x of enemy
		Disk de[2];
		de[0] = turtle[i]->getCollisionHull(10, -3, 4.5f);
		de[1] = turtle[i]->getCollisionHull(10, -3, 4.5f);
		float dx1;
		float dx2;
		dx1 = dxCal(d1.cx, de[0].cx);
		dx2 = dxCal(d1.cx, de[1].cx);
		float dy1;
		float dy2;
		dy1 = dxCal(d1.cy, de[0].cy);
		dy2 = dxCal(d1.cy, de[1].cy);


		if (sqrt(dx1 * dx1 + dy1 * dy1) < d1.radius + de[0].radius || sqrt(dx2 * dx2 + dy2 * dy2) < d1.radius + de[1].radius)
			return true;

	}
	return false;
}
**/
void Game::init()
{
	graphics::setFont(std::string(ASSET_PATH) + "timer.ttf");
}
Game::Game()
{
}



Game::~Game()
{
	if (player)
	{
		delete player;
	}
}



float  Game::dxCal(float a, float b)
{
	return a - b;
};

bool Game::checkRiverPlayerCollision()
{

	
	for (int j = 0; j < 5; ++j)
	{
		for (int i = 0; i < 13; i++)
		{
			if (!checkRiverTurtleCollision(i, j) && player_initialized)
			{
				river->setPosX(0);
				river->setPosY(265);
				Disk d1 = player->getCollisionHull();
				Disk de = river->getCollisionHull(i * 60, -j * 45, 3.25f);
				float dx1;
				dx1 = dxCal(d1.cx, de.cx);
				float dy1;
				dy1 = dxCal(d1.cy, de.cy);
				if (sqrt(dx1 * dx1 + dy1 * dy1) < d1.radius + de.radius)
					return true;
			}
		}
	}
	


	return false;



}
bool Game::checkTurtlePlayerCollision()
{
	if (!player)
	{
		return false;
	}
	for (int l = 0; l < numOfTurtles; ++l)
	{
		if (loc_turtle[l])
		{
			Disk d1 = turtle[l]->getCollisionHull(20, 0, 5.5f);
			Disk d2 = turtle[l]->getCollisionHull(-20, 0, 5.5f);
			Disk d3 = turtle[l]->getCollisionHull(55, 0, 5.5f);
			Disk de = player->getCollisionHull(0, 0, 1.25);
			float dx1;
			float dx2;
			float dx3;
			dx1 = dxCal(d1.cx, de.cx);
			dx2 = dxCal(d2.cx, de.cx);
			dx3 = dxCal(d3.cx, de.cx);
			float dy1;
			float dy2;
			float dy3;
			dy1 = dxCal(d1.cy, de.cy);
			dy2 = dxCal(d2.cy, de.cy);
			dy3 = dxCal(d3.cy, de.cy);
			if (sqrt(dx1 * dx1 + dy1 * dy1) < d1.radius + de.radius)
			{
				player->setPosX((player->getPosX() + turtle[l]->getDiraction() * 1.3));
			}
			if (sqrt(dx2 * dx2 + dy2 * dy2) < d2.radius + de.radius)
			{
				player->setPosX((player->getPosX() + turtle[l]->getDiraction() * 1.3));
			}
			if (sqrt(dx3 * dx3 + dy3 * dy3) < d3.radius + de.radius)
			{
				player->setPosX((player->getPosX() + turtle[l]->getDiraction() * 1.3));
			}
		}

	}
	return true;
}
bool Game::checkFinishCollision()
{
	if (!player)
	{
		return false;
	}
	
	for (int i = 0; i < 11; i=i+1)
	{
		river->setPosX(35);
		if(i%2==1)
			river->setPosY(30);
		else
			river->setPosY(0);
		Disk d1 = river->getCollisionHull(i * 60, 0, 5.f);
		Disk dp = player->getCollisionHull();
		float dx = dxCal(d1.cx, dp.cx);
		float dy = dxCal(d1.cy, dp.cy);
		
		if (sqrt(dx * dx + dy * dy) < d1.radius + dp.radius)
		{
			
			return true;
		}
		
	}

	return false;
}






