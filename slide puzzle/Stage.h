#pragma once
#include"Object.h"
class Stage
{
private:
	struct Data
	{
		Vec3 position = {};
		Vec3 rotation = {};
		Vec3 scale = {};
	};

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

	Data skyData;
	Data stageData;

	const float SCALE = 15.0f;
	const float SCALEDRAW = 30.0f;
};

