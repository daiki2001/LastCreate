#pragma once
#include"Object.h"
#include"Sprite.h"
#include<ParticleManager.h>
#include<memory>
#include<Audio.h>
#include <array>
#include <algorithm>
#include <functional>
#include"../slide puzzle/Ball.h"

class Player
{
private:


private: // 定数
	// 弧度法
	const float PI = 3.14159265359f;
	// 度数法
	const float ANGLE = 180.0f;

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

	const Vec3 GetPosition() { return position; }
	const Vec3 GetRotation() { return rotation; }
	void SetBall(Ball* ball);

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

	void TargetLockOn(Vec3 pos = {});

private:
	ObjectData pObject;                         //プレイヤーオブジェクト
	Vec3 position = {};                         //位置
	Vec3 rotation = {};

	Ball* ball_ = nullptr;

	//接地フラグ
	bool onGround_ = true;
	// 落下ベクトル
	Vec3 fallV_ = {};

	// キャッチ関係
	bool catchFlag_ = false;
	int catchTimer_ = 0;
};

