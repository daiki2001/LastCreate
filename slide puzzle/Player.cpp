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
	//FBX関連
	Model* model1 = FbxLoader::GetInstance()->LoadModelFromFile("player");
	m_model = std::make_unique<Model>();
	m_model = std::unique_ptr<Model>(model1);
	m_fbx = std::make_unique<FBXObject3d>();
	m_fbx->Initialize();
	m_fbx->SetModel(m_model.get());
	m_fbx->SetScale(Vec3(0.005f, 0.005f, 0.005f));
	
	m_fbx->PlayAnimation(true);
}

void Player::Update()
{
	Move();
	Jump();
	m_fbx->Update();
}

void Player::Draw()
{
	//Object::Draw(pObject, position, Vec3(1.0f, 1.0f, 1.0f), rotation);
	m_fbx->Draw(true);
	m_fbx->SetPosition(position);
}

void Player::Move()
{
	bool dashFlag = false;

	if (Input::Get()->KeybordPush(DIK_LSHIFT))
	{
		dashFlag = true;
	}

	float a = 1.0f;
	float b = 0.5f;


	if (Input::Get()->KeybordPush(DIK_W))
	{
		if (dashFlag)
		{
			position.z += a;
		}
		else
		{
			position.z += b;
		}
	}
	else if (Input::Get()->KeybordPush(DIK_S))
	{
		if (dashFlag)
		{
			position.z -= a;
		}
		else
		{
			position.z -= b;
		}
	}

	if (Input::Get()->KeybordPush(DIK_A))
	{
		if (dashFlag)
		{
			position.x -= a;
		}
		else
		{
			position.x -= b;
		}
	}
	else if (Input::Get()->KeybordPush(DIK_D))
	{
		if (dashFlag)
		{
			position.x += a;
		}
		else
		{
			position.x += b;
		}
	}
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
	if (Input::Get()->MouseTriggerLeft())
	{

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
