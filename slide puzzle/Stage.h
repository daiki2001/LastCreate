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
	float GetStageSize() { return SCALE; }
private:
	ObjectData stageObj;
	ObjectData sky;

	Vec3 position = {};
	const float SCALE = 40.0f;
	const float SCALEDRAW = 25.0f;
	Vec3 scale = { SCALEDRAW,SCALEDRAW,SCALEDRAW };
	Vec3 rotation = {};
};

