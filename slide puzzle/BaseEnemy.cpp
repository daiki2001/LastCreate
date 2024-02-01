#include "BaseEnemy.h"
#include<Shape.h>
#include "../../FBXModelManager.h"

void BaseEnemy::Init(Vec3 pos, Vec3 rot)
{
	position = pos;
	rotation = rot;

	m_fbx = std::make_unique<FBXObject3d>();
	m_fbx->Initialize();
	m_fbx->SetModel(FBXModelManager::GetInstance()->GetModel("enemy"));
	m_fbx->LoadAnumation();
	m_fbx->SetScale(Vec3(0.0025f, 0.0025f, 0.0025f));
	m_fbx->PlayAnimation(m_fbx->GetArmature("run"), true);

	hp_ = 10;

	BallCatch();
}

void BaseEnemy::Update()
{
	Move();
	StageCollision(40.0f);
	m_fbx->SetPosition(position);
	m_fbx->SetRotation(rotation);
	m_fbx->Update();
}

void BaseEnemy::Draw()
{
	//Object::Draw(pObject, position, Vec3(1.0f, 1.0f, 1.0f), rotation);
	m_fbx->Draw();
}

void BaseEnemy::Move()
{
	if (!actFlag_)
	{
		maxTime_ = (rand() % 60) + 60;
		act = static_cast<MoveAct>(rand() % 4);
		timer_ = 0;
		actFlag_ = true;
	}
	else
	{
		timer_++;
	}

	switch (act)
	{
	case BaseEnemy::Progress:
		position.z += 0.25f;
		rotation.y = 0;
		break;
	case BaseEnemy::Retreat:
		position.z -= 0.25f;
		rotation.y = 180;
		break;
	case BaseEnemy::Right:
		position.x += 0.25f;
		rotation.y = 90;
		break;
	case BaseEnemy::Left:
		position.x -= 0.25f;
		rotation.y = -90;
		break;
	default:
		break;
	}

	if (timer_ > maxTime_)
	{
		actFlag_ = false;
	}
}
