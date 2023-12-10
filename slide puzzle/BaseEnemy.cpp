#include "BaseEnemy.h"
#include<Shape.h>

void BaseEnemy::Init(Vec3 pos, Vec3 rot)
{
	position = pos;
	rotation = rot;
	pObject = Shape::CreateOBJ("sphere");

	hp_ = 10;

	BallCatch();
}

void BaseEnemy::Update()
{
	Move();
	StageCollision(40.0f);
}

void BaseEnemy::Draw()
{
	Object::Draw(pObject, position, Vec3(1.0f, 1.0f, 1.0f), rotation);
}

void BaseEnemy::Move()
{
	if (!actFlag_)
	{
		maxTime_ = (rand() % 60) + 60;
		act = static_cast<MoveAct>(rand() % 4);
		timer_ = 0;
		actFlag_ = true;
	}
	else
	{
		timer_++;
	}

	switch (act)
	{
	case BaseEnemy::Progress:
		position.z += 0.25f;
		break;
	case BaseEnemy::Retreat:
		position.z -= 0.25f;
		break;
	case BaseEnemy::Right:
		position.x += 0.25f;
		break;
	case BaseEnemy::Left:
		position.x -= 0.25f;
		break;
	default:
		break;
	}

	if (timer_ > maxTime_)
	{
		actFlag_ = false;
	}
}
