#include"graphics.h"
#include "Enemy.h"
#include "Config.h"
#include "Game.h"
#include <iostream>
using namespace std;

Enemy::Enemy(const class Game& mygame)
	:GameObject(mygame)
{
	init();
}

void Enemy::update()
{
	pos_x += speed * graphics::getDeltaTime() * diraction;
	if (pos_x < -size || pos_x>CANVAS_WIDTH + size)
	{
		active = false;
	}
}


void Enemy::draw()
{
	
	graphics::Brush br;
	br.texture = std::string(ASSET_PATH) + ex1;
	br.outline_opacity = 0.f;
	graphics::setOrientation(-diraction * 90);
	graphics::drawRect(pos_x, pos_y, size, size, br);
	graphics::resetPose();
	
	/**for debug/
	br.outline_opacity = 1.f;
	br.texture = "";
	br.fill_color[0] = 1.f;
	br.fill_color[1] = 0.3f;
	br.fill_color[2] = 0.3f;
	br.fill_opacity = 0.3f;
	br.gradient = false;
	
	Disk hull1 = getCollisionHull(10, -3, 4.5f);
	graphics::drawDisk(hull1.cx, hull1.cy, hull1.radius, br);
	Disk hull2 = getCollisionHull(-15, -3, 4.5f);
	graphics::drawDisk(hull2.cx, hull2.cy, hull2.radius, br);
	**/
}

void Enemy::init()
{
	speed = 0.1f;
	size = 100;
	active = true;
	

}


Enemy::~Enemy()
{
}


Disk Enemy::getCollisionHull() const
{
	Disk disk;
	disk.cx = pos_x;
	disk.cy = pos_y;
	disk.radius = size;
	return disk;
}

Disk Enemy::getCollisionHull(int ofset_x, int ofset_y, float red_size) const
{
	Disk disk;
	disk.cx = pos_x + ofset_x;
	disk.cy = pos_y + ofset_y;
	disk.radius = size / red_size;
	return disk;
}
