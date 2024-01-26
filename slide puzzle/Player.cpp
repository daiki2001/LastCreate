#include "Player.h"
#include<Shape.h>
#include<Collision.h>
#include"Easing.h"
#include<random>
#include"GameInputManager.h"
#include "../../FBXModelManager.h"

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

	m_fbx = std::make_unique<FBXObject3d>();
	m_fbx->Initialize();
	m_fbx->SetModel(FBXModelManager::GetInstance()->GetModel("player"));
	m_fbx->LoadAnumation();
	m_fbx->SetScale(Vec3(0.0025f, 0.0025f, 0.0025f));
	m_fbx->SetRotation(Vec3(0, 90, 0));

	position = { 0.0f, 0.0f, 10.0f };

	dustParticle = std::make_unique<ParticleManager>();
	dustParticle->Initialize();
	dustGraph = Texture::Get()->LoadTexture(L"Resources/Paricle/particle.jpg");
}

void Player::Update(float stageSize)
{
	Move();
	Jump();
	BallThrow();
	ComboCalculation();
	StageCollision(stageSize);
	dustParticle->Update();
	Action();
}

void Player::Draw()
{
	//Object::Draw(pObject, position, Vec3(1.0f, 1.0f, 1.0f), rotation);
	m_fbx->Draw();
	m_fbx->SetPosition(position);
	input->DebugDraw();
}

void Player::ParticleDraw()
{
	dustParticle->Draw(dustGraph);
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
		speed.y = speed.y + 0.25f;
	}
	else if (input->IsBack())
	{
		speed.y = speed.y - 0.25f;
	}

	if (input->IsRight())
	{
		speed.x = speed.x - 0.25f;
	}
	else if (input->IsLeft())
	{
		speed.x = speed.x + 0.25f;
	}

	//
	if (input->IsDash())
	{
		speed.x *= 1.25f;
		speed.y *= 1.25f;
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
	if (onGround_ && (input->IsForward() || input->IsBack() || input->IsLeft() || input->IsRight()))
	{

		m_fbx->PlayAnimation(m_fbx->GetArmature("run"), true);
		dustParticle->DustAdd(position,
			0.0f, 0.4f, 0.0f,
			Vec4(0.3f, 0.1f, 0.1f, 0.3f), Vec4(0.0f, 0.0f, 0.0f, 0.0f));
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
		isAction = true;
		animeNo = JAMP;
		
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
		oldBall_ = nullptr;
	}
	// 投げる
	if (GameInputManager::Get()->IsThrow()) {
		ball_->SetThrowFlag(true);
		oldBall_ = ball_;
		ball_ = nullptr;
		isAction = true;
		animeNo = THROW;
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
	}

	// コンボ失敗
	if (oldBall_->GetComboMissFlag()) {
		comboCount_ = 0;
		oldBall_->SetComboMissFlag(false);
	}
}

void Player::StageCollision(const float stageSize)
{
	/*if (!Collision::CircleCollision(Vec2(position.x, position.z), Vec2(), 1.0f, stageSize))
	{
		float length = sqrt(position.x * position.x + position.z * position.z);
		float  difference = length - stageSize;
		Vec2 normalize = { position.x / length,position.z / length };
		position.x -= normalize.x * difference;
		position.z -= normalize.y * difference;
	}*/
	float aaaa = 29.0f;
	position.x = std::clamp(position.x, -aaaa, aaaa);
	position.z = std::clamp(position.z, -50.0f, 0.0f);
}

void Player::TargetLockOn(Vec3 pos)
{
	targetPos_ = pos;
	Vec3 vector = { targetPos_.x - position.x, targetPos_.y - position.y, targetPos_.z - position.z };
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
	playerRot.z = 0.0f;
	m_fbx->SetRotation(Vec3(-playerRot.z, playerRot.y, playerRot.x));
}

void Player::Action()
{
	if (!input->IsForward() && !input->IsBack() && !input->IsLeft() && !input->IsRight() && !isAction)
	{
		m_fbx->PlayAnimation(m_fbx->GetArmature("wait"), true);
	}

	if (isAction)
	{
		if (animeNo == JAMP)
		{
			m_fbx->PlayAnimation(m_fbx->GetArmature("jump"), true);

			if (m_fbx->GetAnimeCurrentTime(m_fbx->GetArmature("jump")) >= m_fbx->GetAnimeEndTime(m_fbx->GetArmature("jump")))
			{
				isAction = false;
			}
		}
		else if (animeNo == THROW)
		{
			m_fbx->PlayAnimation(m_fbx->GetArmature("throw"), true);

			if (m_fbx->GetAnimeCurrentTime(m_fbx->GetArmature("throw")) >= m_fbx->GetAnimeEndTime(m_fbx->GetArmature("throw")))
			{
				isAction = false;
			}
		}
	}

	
	m_fbx->Update();
}
