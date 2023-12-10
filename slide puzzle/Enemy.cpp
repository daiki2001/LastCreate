#include "Enemy.h"
#include<Collision.h>

void Enemy::SetBall(Ball* ball)
{
	if (ball_ != nullptr) { return; }

	ball_ = ball;
	ball_->SetHaveFlag(true);
}

void Enemy::BallThrow()
{
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
	if (!Collision::CircleCollision(Vec2(position.x, position.z), Vec2(), 1.0f, stageSize))
	{
		float length = sqrt(position.x * position.x + position.z * position.z);
		float  difference = length - stageSize;
		Vec2 normalize = { position.x / length,position.z / length };
		position.x -= normalize.x * difference;
		position.z -= normalize.y * difference;
		actFlag_ = false;
	}

	if (position.z <= 0.0f) { actFlag_ = false; }

	position.z = std::clamp(position.z, 0.0f, 50.0f);
}
