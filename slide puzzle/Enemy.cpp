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
