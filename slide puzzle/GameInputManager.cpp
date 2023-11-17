#include "GameInputManager.h"
#include <Input.h>

namespace
{
auto* input = Input::Get();
}

GameInputManager::~GameInputManager()
{
	if (this->ball) delete this->ball;
}

void GameInputManager::Init()
{
	ball = BallController::Create();
}

void GameInputManager::Update()
{
	ball->Update();
}

void GameInputManager::DebugDraw()
{
#ifndef _DEBUG
	return;
#endif // !_DEBUG

	ball->Load();
	ball->DrawGraph();
}

bool GameInputManager::IsDecide() const
{
	bool result = false;
	result |= input->KeybordTrigger(DIK_SPACE);
	result |= input->ControllerUp(ButtonKind::ButtonA);
	result |= ball->GetFlagTriger();
	return result;
}

bool GameInputManager::IsForward() const
{
	bool result = false;
	result |= input->KeybordPush(DIK_W);
	result |= input->ControllerPush(ButtonKind::LButtonUp);
	return result;
}

bool GameInputManager::IsBack() const
{
	bool result = false;
	result |= input->KeybordPush(DIK_S);
	result |= input->ControllerPush(ButtonKind::LButtonDown);
	return result;
}

bool GameInputManager::IsLeft() const
{
	bool result = false;
	result |= input->KeybordPush(DIK_A);
	result |= input->ControllerPush(ButtonKind::LButtonLeft);
	return result;
}

bool GameInputManager::IsRight() const
{
	bool result = false;
	result |= input->KeybordPush(DIK_D);
	result |= input->ControllerPush(ButtonKind::LButtonRight);
	return result;
}

bool GameInputManager::IsDash() const
{
	bool result = false;
	result |= input->KeybordPush(DIK_LSHIFT);
	result |= input->ControllerPush(ButtonKind::LButtonPush);
	return result;
}

bool GameInputManager::IsJamp() const
{
	bool result = false;
	result |= input->KeybordTrigger(DIK_SPACE);
	result |= input->ControllerUp(ButtonKind::ButtonA);
	result |= ball->GetFlagTriger();
	return result;
}

bool GameInputManager::IsThrow() const
{
	bool result = false;
	result |= input->MouseTriggerLeft();
	result |= input->ControllerUp(ButtonKind::ButtonRT);
	result |= ball->IsBallThrow();
	return result;
}

bool GameInputManager::IsCatch() const
{
	bool result = false;
	result |= input->MouseTriggerLeft();
	result |= input->ControllerUp(ButtonKind::ButtonRT);
	result |= ball->IsBallThrow();
	return result;
}
