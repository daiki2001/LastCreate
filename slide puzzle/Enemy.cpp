#include "Enemy.h"
#include<Collision.h>

void Enemy::BallThrow()
{
	// �{�[������
	if (ball == nullptr) { return; }

	// ������
	ball->SetChargeFlag(true);	
	ball->SetThrowFlag(true);
	oldBall = std::move(ball);
	ball = nullptr;
}

void Enemy::BallCatch()
{
}

void Enemy::DamageHit(Vec3 pos ,int comboCount_)
{
	// �U��������������
	const float playerR = 3.0f;
	const float ballR = 3.0f;

	Vec3 axyz = (position - pos) * (position - pos);
	float ar = (ballR + playerR) * (ballR + playerR);

	// �{�[���ɓ�������
	if (axyz.x + axyz.y + axyz.z <= ar) {
		hp_ -= 1 * (comboCount_ + 10);
		if (hp_ <= 0) {
			forcusChangeFlag_ = true;
		}
	}
}

void Enemy::StageCollision(const float stageSize)
{
	/*if (!Collision::CircleCollision(Vec2(position.x, position.z), Vec2(), 1.0f, stageSize))
	{
		float length = sqrt(position.x * position.x + position.z * position.z);
		float  difference = length - stageSize;
		Vec2 normalize = { position.x / length,position.z / length };
		position.x -= normalize.x * difference;
		position.z -= normalize.y * difference;
		actFlag_ = false;
	}*/

	if (position.z <= 0.0f || position.z >= 50.0f || position.x <= -29.0f || position.x >= 29.0f) { actFlag_ = false; }
	float aaaa = 29.0f;
	position.x = std::clamp(position.x, -aaaa, aaaa);
	position.z = std::clamp(position.z, 0.0f, 50.0f);
}

void Enemy::BallCreate()
{
	if (ball == nullptr) { return; }
	createTimer_--;
	if (createTimer_ > 0) { return; }
	std::unique_ptr<Ball> ball = std::make_unique<Ball>();
	ball->Init();
	ball->SetHaveFlag(true);
	ball = std::move(ball);
	createTimer_ = (rand() % 1800) + 1800;
}

void Enemy::EnemyBallAct()
{
	ball->Update(
		position, rotation,
		15.0f);
}
