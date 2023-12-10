#pragma once
#include<sstream>
#include<iomanip>
#include"Camera.h"
#include"Shape.h"
#include"Input.h"
#include"Texture.h"
#include "DirectXBase.h"
#include"DebugText.h"
#include"Collision.h"
#include"Audio.h"
#include"FBXObject3d.h"
#include"Object.h"
#include"Sprite.h"
#include<memory>
#include "Safe_delete.h"
#include"ParticleManager.h"
class SceneManager;


class BaseScene
{
public:
	virtual ~BaseScene() = default;
	/// <summary>
	/// ‰Šú‰»
	/// </summary>
	virtual void Init() = 0;
	/// <summary>
	/// XV
	/// </summary>
	virtual void Update() = 0;
	/// <summary>
	/// •`‰æ
	/// </summary>
	virtual void Draw() = 0;
	/// <summary>
	/// ‰e•`‰æ
	/// </summary>
	virtual void ShadowDraw() = 0;

	virtual void Finalize() = 0;
	//
	void SetSceneManager(SceneManager* sceneManager) { sceneManager_ = sceneManager; }
protected:
	SceneManager* sceneManager_ = nullptr;
};

