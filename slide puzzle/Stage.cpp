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

}

void Stage::Update()
{
}

void Stage::Draw()
{
	Object::Draw(stageObj, position, scale, rotation);
}
