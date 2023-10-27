#pragma once
#include"Object.h"
#include<memory>

class Ball
{
public:
	//コンストラクタ
	Ball();
	//デスコンストラクタ
	~Ball();
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

	//
	bool HaveHit(Vec3 pos);

	// 位置の設定
	void SetChainPosition(Vec3 pos);
	void SetPosition(Vec3 pos) { position = pos; }
	//
	void SetHaveFlag(bool flag) { haveFlag_ = flag; }
	//
	bool GetHaveFlag() { return haveFlag_; }
	//
	void SetThrowFlag(bool flag) { throwFlag_ = flag; }
	//
	bool GetThrowFlag() { return throwFlag_; }

private:
	// ボールの挙動
	void ThrowAct();
	// 所持者関係
	void HaveAct();

private:
	ObjectData pObject;                         //プレイヤーオブジェクト
	Vec3 position = {};                         //位置
	Vec3 rotation = {};

	// 所持者の位置
	Vec3 havePos_ = {};
	// ターゲットの位置
	Vec3 targetPos_ = {};

	// 所持しているか
	bool haveFlag_ = false;
	// 投げているか
	bool throwFlag_ = false;
};

