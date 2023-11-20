#include "BaseEnemy.h"
#include<Shape.h>

void BaseEnemy::Init(Vec3 pos, Vec3 rot)
{
	position = pos;
	rotation = rot;
	pObject = Shape::CreateOBJ("sphere");

	BallCatch();
}

void BaseEnemy::Update()
{
}

void BaseEnemy::Draw()
{
	Object::Draw(pObject, position, Vec3(1.0f, 1.0f, 1.0f), rotation);
}

void BaseEnemy::Move()
{
	if (!actFlag_)
	{
		maxTime_ = (rand() % 150) + 100;
		act = static_cast<MoveAct>(rand() % 3);
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
		position.z += 0.5f;
		break;
	case BaseEnemy::Retreat:
		position.z -= 0.5f;
		break;
	case BaseEnemy::Right:
		position.x += 0.5f;
		break;
	case BaseEnemy::Left:
		position.x -= 0.5f;
		break;
	default:
		break;
	}

	if (timer_ > maxTime_)
	{
		actFlag_ = false;
	}
}
