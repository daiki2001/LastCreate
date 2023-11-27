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
	void SetBall(Ball* ball);

protected:
	void BallThrow();
	void BallCatch();
	void DamageHit(Vec3 pos, int comboCount_);
	void StageCollision(const float stageSize);

protected:
	std::unique_ptr<Model> m_model;
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

	Ball* ball_ = nullptr;

	bool catchFlag_ = false;
	int catchTimer_ = 0;
	int hp_ = 0;
};

