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

class Enemy
{
public:
	virtual void Init(Vec3 pos = {}, Vec3 rot = {}) = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	const Vec3 GetPosition() { return position; }
	const Vec3 GetRotation() { return rotation; }
	bool GetForcusChangeFlag() { return forcusChangeFlag_; };
	void SetForcusChangeFlag(bool changeFlag_) { forcusChangeFlag_ = changeFlag_; }

	int GetHp() { return hp_; }

	void DamageHit(Vec3 pos, int comboCount_);
protected:
	void BallThrow();
	void BallCatch();
	void StageCollision(const float stageSize);
	void BallCreate();
	void EnemyBallAct();

protected:
	std::unique_ptr<FBXObject3d>  m_fbx;

protected:
	ObjectData pObject;
	Vec3 position = {};
	Vec3 rotation = {};

	/*-----hpŠÖ˜A-----*/
	ObjectData hpObj;
	Vec3 hpPosition = {};
	Vec3 hpScale = { 0.3f,0.2f,0.3f };
	/*---------------*/

	std::unique_ptr<Ball> ball;
	std::unique_ptr<Ball> oldBall;
	int createTimer_ = 0;
	bool actFlag_ = false;
	bool catchFlag_ = false;
	int catchTimer_ = 0;
	int hp_ = 0;
	bool forcusChangeFlag_ = false;
};

