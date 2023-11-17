#pragma once
#include"Object.h"
class Stage
{

public:
	Stage();

	~Stage();

	void Init();
	void Update();
	void Draw();
private:
	ObjectData stageObj;

	Vec3 position = {};
	const float SCALE = 25.0f;
	Vec3 scale = { SCALE,SCALE,SCALE };
	Vec3 rotation = {};
};

