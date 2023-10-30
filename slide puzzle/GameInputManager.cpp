#include "GameInputManager.h"
#include<Input.h>

namespace
{
auto* input = Input::Get();
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
	return result;
}

bool GameInputManager::IsJamp() const
{
	bool result = false;
	result |= input->KeybordTrigger(DIK_SPACE);
	result |= input->ControllerUp(ButtonKind::ButtonA);
	return result;
}

bool GameInputManager::IsThrow() const
{
	bool result = false;
	result |= input->MouseTriggerLeft();
	result |= input->ControllerUp(ButtonKind::ButtonRT);
	return result;
}

bool GameInputManager::IsCatch() const
{
	bool result = false;
	result |= input->MouseTriggerLeft();
	result |= input->ControllerUp(ButtonKind::ButtonRT);
	return result;
}
