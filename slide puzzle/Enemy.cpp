#include "Enemy.h"

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
	const float playerR = 1.0f;
	const float ballR = 1.0f;

	Vec3 axyz = (position - pos) * (position - pos);
	float ar = (ballR + playerR) * (ballR + playerR);

	// �{�[���ɓ�������
	if (axyz.x + axyz.y + axyz.z <= ar) {
		hp_ -= 1 * comboCount_;
	}
}
