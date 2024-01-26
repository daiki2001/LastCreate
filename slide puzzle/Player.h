#pragma once
#include"Object.h"
#include"Sprite.h"
#include<ParticleManager.h>
#include<memory>
#include<Audio.h>
#include <array>
#include <algorithm>
#include <functional>
#include "Model.h"
#include "FbxLoader.h"
#include "FBXObject3d.h"
#include"../slide puzzle/Ball.h"
#include<ParticleManager.h>
class Player
{
private:


private:
	const float PI = 3.14159265359f;
	const float ANGLE = 180.0f;

public:
	Player();
	~Player();
	void Init();
	void Update(const float stageSize);
	void Draw();
	void ParticleDraw();

	const Vec3 GetPosition() { return position; }
	const Vec3 GetRotation() { return rotation; }
	void SetBall(Ball* ball);
	int GetComboCount() { return comboCount_; };
	void SetComboCount(int combo) { comboCount_ = combo; }

	const Vec3 GetTargetPos() { return targetPos_; }

	void TargetLockOn(Vec3 pos = {});

private:
	void Move();
	void Jump();
	void BallThrow();
	void BallCatch();
	void ComboCalculation();
	void StageCollision(const float stageSize);
private:
	std::unique_ptr<FBXObject3d>  m_fbx;

private:
	ObjectData pObject;
	Vec3 position = {};
	Vec3 rotation = {};

	Vec3 targetPos_ = {};

	Ball* ball_ = nullptr;

	Ball* oldBall_ = nullptr;

	bool onGround_ = true;
	Vec3 fallV_ = {};

	bool catchFlag_ = false;
	int catchTimer_ = 0;

	int maxComboCount_ = 10;
	int comboCount_ = 0;

	std::unique_ptr < ParticleManager> dustParticle = nullptr;
	TextureData dustGraph;
};

