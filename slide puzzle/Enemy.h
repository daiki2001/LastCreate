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
	Enemy();
	~Enemy();
	void Init(Vec3 pos = {}, Vec3 rot = {});
	void Update();
	void Draw();

	const Vec3 GetPosition() { return position; }
	const Vec3 GetRotation() { return rotation; }
	void SetBall(Ball* ball);

private:
	void Move();
	void BallThrow();
	void BallCatch();

private:
	std::unique_ptr<Model> m_model;
	std::unique_ptr<FBXObject3d>  m_fbx;

private:
	ObjectData pObject;
	Vec3 position = {};
	Vec3 rotation = {};

	Ball* ball_ = nullptr;

	bool catchFlag_ = false;
	int catchTimer_ = 0;
};
