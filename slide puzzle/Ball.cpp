#include "Ball.h"
#include<Shape.h>

Ball::Ball()
{
}

Ball::~Ball()
{
}

void Ball::Init()
{
	pObject = Shape::CreateOBJ("Ball");
}

void Ball::Update()
{
	HaveAct();
	ThrowAct();
}

void Ball::Draw()
{
	Object::Draw(pObject, position, Vec3(0.5f, 0.5f, 0.5f), rotation);
}

bool Ball::HaveHit(Vec3 pos)
{
	if (pos == position)
	{
		return true;
	}

	return false;
}

void Ball::SetChainPosition(Vec3 pos)
{
	if (!haveFlag_ && !throwFlag_) { return; }

	position = havePos_ = pos;
}

void Ball::ThrowAct()
{
	if (!throwFlag_) { return; }

	// 方向ベクトルの算出
	Vec3 vector = { targetPos_.x - havePos_.x, targetPos_.y - havePos_.y, targetPos_.z - havePos_.z };
	float speed = 0.1f;
	float v = sqrtf((vector.x * vector.x) + (vector.y * vector.y) + (vector.z * vector.z));
	vector = { (vector.x / v) * speed, (vector.y / v) * speed, (vector.z / v) * speed };

	position += vector;
}

void Ball::HaveAct()
{
	if (!haveFlag_) { return; }

	// ターゲットの位置
	targetPos_ = {};
}
