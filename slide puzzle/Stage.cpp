#include "Stage.h"
#include"Shape.h"

Stage::Stage()
{
}

Stage::~Stage()
{
}

void Stage::Init()
{

	stageObj = Shape::CreateOBJ("stage", false);
	sky = Shape::CreateOBJ("sky", false);
	building = Shape::CreateOBJ("back", false);
	signage = Shape::CreateOBJ("signage", false);
	ground = Shape::CreateOBJ("ground", false);

	skyData.scale = { SCALEDRAW, SCALEDRAW, SCALEDRAW };

	stageData.position = { 0.0f, 10.0f, 0.0f };
	stageData.scale = { SCALE,SCALE,SCALE };
}

void Stage::Update()
{
}

void Stage::Draw()
{
	Object::Draw(sky, skyData.position, skyData.scale, skyData.rotation);
	Object::Draw(stageObj, stageData.position, stageData.scale, stageData.rotation);
	Object::Draw(building, stageData.position, stageData.scale, stageData.rotation);
	Object::Draw(signage, stageData.position, stageData.scale, stageData.rotation);
	Object::Draw(ground, stageData.position, stageData.scale, stageData.rotation);
}
