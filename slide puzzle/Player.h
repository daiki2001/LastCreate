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

class Player
{
private:
	

public:
	//コンストラクタ
	Player();
	//デスコンストラクタ
	~Player();
	/// <summary>
	/// 初期化
	/// </summary>
	void Init();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();
private:
	/// <summary>
	/// 移動
	/// </summary>
	void Move();
	/// <summary>
	/// ジャンプ
	/// </summary>
	void Jump();
	/// <summary>
	/// 投げる
	/// </summary>
	void BallThrow();
	/// <summary>
	/// 捕る
	/// </summary>
	void BallCatch();
private:
	std::unique_ptr<Model> m_model;		  //モデル
	std::unique_ptr<FBXObject3d>  m_fbx;//FBX

private:
	ObjectData pObject;                         //プレイヤーオブジェクト
	Vec3 position = {};                         //位置
	Vec3 rotation = {};

	//接地フラグ
	bool onGround_ = true;
	// 落下ベクトル
	Vec3 fallV_ = {};

	// キャッチ関係
	bool catchFlag_ = false;
	int catchTimer_ = 0;
};

