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

}

void Stage::Update()
{
}

void Stage::Draw()
{
	Object::Draw(sky, position, scale, rotation);
	Object::Draw(stageObj, position, scale, rotation);
}
