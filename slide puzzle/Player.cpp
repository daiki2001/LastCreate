#include "Player.h"
#include<Shape.h>
#include<Collision.h>
#include"Easing.h"
#include<random>
#include"GameInputManager.h"
namespace
{
	auto* input = GameInputManager::Get();
}

Player::Player()
{
}

Player::~Player()
{
}

void Player::Init()
{
	pObject = Shape::CreateOBJ("sphere");

	Model* model1 = FbxLoader::GetInstance()->LoadModelFromFile("player");
	m_model = std::make_unique<Model>();
	m_model = std::unique_ptr<Model>(model1);
	m_fbx = std::make_unique<FBXObject3d>();
	m_fbx->Initialize();
	m_fbx->SetModel(m_model.get());
	m_fbx->LoadAnumation();
	m_fbx->SetScale(Vec3(0.0025f, 0.0025f, 0.0025f));
	m_fbx->SetRotation(Vec3(0, 90, 0));

	m_fbx->PlayAnimation(m_fbx->GetArmature("run"), true);

	position = { 10.0f, 0.0f, 0.0f };
}

void Player::Update(float stageSize)
{
	Move();
	Jump();
	BallThrow();
	ComboCalculation(); 
	//TargetLockOn(Vec3{ 0.0f, 0.0f, 0.0f });
	StageCollision(stageSize);
	m_fbx->Update();
}

void Player::Draw()
{
	//Object::Draw(pObject, position, Vec3(1.0f, 1.0f, 1.0f), rotation);
	m_fbx->Draw();
	m_fbx->SetPosition(position);
	input->DebugDraw();
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
	XMFLOAT2 speed = {};
	if (input->IsForward())
	{
		speed.y = speed.y + 0.5f;
	}
	else if (input->IsBack())
	{
		speed.y = speed.y - 0.5f;
	}

	if (input->IsLeft())
	{
		speed.x = speed.x - 0.5f;
	}
	else if (input->IsRight())
	{
		speed.x = speed.x + 0.5f;
	}

	//
	if (input->IsDash())
	{
		speed.x *= 1.5f;
		speed.y *= 1.5f;
	}

	if (speed.y != 0.0f)
	{
		position.x += cosf((rotation.y * 3.14f) / 180.0f) * speed.y;
		position.z += -sinf((rotation.y * 3.14f) / 180.0f) * speed.y;
	}
	if (speed.x != 0.0f)
	{
		position.x += sinf((rotation.y * 3.14f) / 180.0f) * speed.x;
		position.z += cosf((rotation.y * 3.14f) / 180.0f) * speed.x;
	}
}

void Player::Jump()
{
	if (!onGround_)
	{
		const float fallAcc = -0.01f;
		const float fallVYMin = -0.5f;
		fallV_.y = max(fallV_.y + fallAcc, fallVYMin);
		position.x += fallV_.x;
		position.y += fallV_.y;
		position.z += fallV_.z;
	}

	else if (input->IsJamp())
	{
		onGround_ = false;
		const float jumpVYFist = 0.2f;
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
	// ボール無し
	if (ball_ == nullptr) {
		return;
	}

	// 溜める
	if (GameInputManager::Get()->IsCharge()) {
		ball_->SetChargeFlag(true);
	}
	// 投げる
	if (GameInputManager::Get()->IsThrow()) {
		ball_->SetThrowFlag(true);
		oldBall_ = ball_;
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

void Player::ComboCalculation() 
{
	// ボール無し
	if (oldBall_ == nullptr) {
		return;
	}

	// コンボ成功
	if (oldBall_->GetComboUpFlag()) {
		if (comboCount_ < maxComboCount_) {
			comboCount_++;
		}
		oldBall_->SetComboUpFlag(false);
		// oldBall_ = nullptr;
	}

	// コンボ失敗
	if (oldBall_->GetComboMissFlag()) {
		comboCount_ = 0;
		oldBall_->SetComboMissFlag(false);
		// oldBall_ = nullptr;
	}
}

void Player::StageCollision(const float stageSize)
{
	if (!Collision::CircleCollision(Vec2(position.x, position.z), Vec2(), 1.0f, stageSize))
	{
		float length = sqrt(position.x * position.x + position.z * position.z);
		float  difference = length - stageSize;
		Vec2 normalize = { position.x / length,position.z / length };
		position.x -= normalize.x * difference;
		position.z -= normalize.y * difference;
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
	playerRot.y += 90.0f;
	m_fbx->SetRotation(Vec3(-playerRot.z, playerRot.y, playerRot.x));
}
