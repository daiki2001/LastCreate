#include "Player.h"
#include<Shape.h>
#include<Collision.h>
#include"Easing.h"
#include<random>
#include<Input.h>

Player::Player()
{
}

Player::~Player()
{
}

void Player::Init()
{
	pObject = Shape::CreateOBJ("sphere");

	position = { 10.0f, 0.0f, 0.0f };
}

void Player::Update()
{
	Move();
	Jump();
	BallThrow();
	TargetLockOn();
}

void Player::Draw()
{
	Object::Draw(pObject, position, Vec3(1.0f, 1.0f, 1.0f), rotation);
}

void Player::SetBall(Ball* ball)
{
	if (ball_ != nullptr) { return; }

	ball_ = ball;
	ball_->SetHaveFlag(true);
}

void Player::Move()
{
	//
	float speed = 0.0f;

	

	if (Input::Get()->KeybordPush(DIK_W) || Input::Get()->KeybordPush(DIK_D))
	{
		speed = speed + 0.5f;
	}
	else if (Input::Get()->KeybordPush(DIK_S) || Input::Get()->KeybordPush(DIK_A))
	{
		speed = speed - 0.5f;
	}

	//
	if (Input::Get()->KeybordPush(DIK_LSHIFT))
	{
		speed *= 1.5f;
	}

	XMFLOAT2 playerAngle_ = { rotation.x, rotation.y };

	position.x +=  cosf((playerAngle_.y * 3.14f) / 180.0f) * speed;
	position.z += -sinf((playerAngle_.y * 3.14f) / 180.0f) * speed;
}

void Player::Jump()
{
	// 落下処理
	if (!onGround_)
	{
		// 下向き加速度
		const float fallAcc = -0.01f;
		const float fallVYMin = -0.5f;
		// 加速
		fallV_.y = max(fallV_.y + fallAcc, fallVYMin);
		// 移動
		position.x += fallV_.x;
		position.y += fallV_.y;
		position.z += fallV_.z;
	}
	// ジャンプ操作
	else if (Input::Get()->KeybordTrigger(DIK_SPACE))
	{
		onGround_ = false;
		const float jumpVYFist = 0.2f; //ジャンプ時上向き初速
		fallV_ = { 0, jumpVYFist, 0 };
	}

	if (!onGround_ && position.y < 0)
	{
		position.y = 0.0f;
		fallV_ = {};
		onGround_ = true;
	}
}

void Player::BallThrow()
{
	if (ball_ == nullptr) { return; }

	ball_->SetChainPosition(position);

	if (Input::Get()->MouseTriggerLeft())
	{
		ball_->SetThrowFlag(true);
		ball_->SetHaveFlag(false);
		ball_ = nullptr;
	}
}

void Player::BallCatch()
{
	catchFlag_ = true;
	catchTimer_++;

	if (catchTimer_ >= 120)
	{
		catchFlag_ = false;
		catchTimer_ = 0;
	}
}

void Player::TargetLockOn(Vec3 pos)
{
	Vec3 vector = { pos.x - position.x, pos.y - position.y, pos.z - position.z };
	Vec3 playerRot = rotation;

	playerRot.y = -atan2(vector.z - 0.0f, vector.x - 0.0f) * (ANGLE / PI);
	XMMATRIX  rotM = XMMatrixIdentity();
	rotM *= XMMatrixRotationY(XMConvertToRadians(-playerRot.y));
	float w = vector.x * rotM.r[0].m128_f32[3] + vector.y * rotM.r[1].m128_f32[3] + vector.z * rotM.r[2].m128_f32[3] + rotM.r[3].m128_f32[3];
	XMFLOAT3 result
	{
		(vector.x * rotM.r[0].m128_f32[0] + vector.y * rotM.r[1].m128_f32[0] + vector.z * rotM.r[2].m128_f32[0] + rotM.r[3].m128_f32[0]) / w,
		(vector.x * rotM.r[0].m128_f32[1] + vector.y * rotM.r[1].m128_f32[1] + vector.z * rotM.r[2].m128_f32[1] + rotM.r[3].m128_f32[1]) / w,
		(vector.x * rotM.r[0].m128_f32[2] + vector.y * rotM.r[1].m128_f32[2] + vector.z * rotM.r[2].m128_f32[2] + rotM.r[3].m128_f32[2]) / w,
	};
	playerRot.z = atan2(result.y - 0.0f, result.x - 0.0f) * (ANGLE / PI);

	rotation = playerRot;
}
