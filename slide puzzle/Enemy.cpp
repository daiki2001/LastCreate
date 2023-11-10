#include "Enemy.h"
#include<Shape.h>

Enemy::Enemy()
{
}

Enemy::~Enemy()
{
}

void Enemy::Init(Vec3 pos, Vec3 rot)
{
	position = pos;
	rotation = rot;
	pObject = Shape::CreateOBJ("sphere");
}

void Enemy::Update()
{
}

void Enemy::Draw()
{
	Object::Draw(pObject, position, Vec3(1.0f, 1.0f, 1.0f), rotation);
}

void Enemy::SetBall(Ball* ball)
{
	if (ball_ != nullptr) { return; }

	ball_ = ball;
	ball_->SetHaveFlag(true);
}
