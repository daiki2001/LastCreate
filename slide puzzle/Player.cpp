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

	position = { 5.0f, 0.0f, 0.0f };
}

void Player::Update()
{
	Move();
	Jump();
	BallThrow();
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
	float speed = 0.5f;

	//
	if (Input::Get()->KeybordPush(DIK_LSHIFT))
	{
		speed = 1.0f;
	}

	if (Input::Get()->KeybordPush(DIK_W))
	{
		position.z += speed;
	}
	else if (Input::Get()->KeybordPush(DIK_S))
	{
		position.z -= speed;
	}

	if (Input::Get()->KeybordPush(DIK_A))
	{
		position.x -= speed;
	}
	else if (Input::Get()->KeybordPush(DIK_D))
	{
		position.x += speed;
	}
}

void Player::Jump()
{
	// ��������
	if (!onGround_)
	{
		// �����������x
		const float fallAcc = -0.01f;
		const float fallVYMin = -0.5f;
		// ����
		fallV_.y = max(fallV_.y + fallAcc, fallVYMin);
		// �ړ�
		position.x += fallV_.x;
		position.y += fallV_.y;
		position.z += fallV_.z;
	}
	// �W�����v����
	else if (Input::Get()->KeybordTrigger(DIK_SPACE))
	{
		onGround_ = false;
		const float jumpVYFist = 0.2f; //�W�����v�����������
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

	ball_->SetPosition(position);

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
