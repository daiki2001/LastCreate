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
