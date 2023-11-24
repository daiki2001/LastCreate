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
	void Update(Vec3 playerPos, Vec3 playerRotation,const float stageSize);
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
	// ボールが当たったフラグ
	bool BallHitFlag();
	// ボールが当たって反射
	void BallReflectBound(Vec3 playerPos);
	// 　跳ね返る方向計算
	void ReflectCalculation(Vec3 playerPos);
	// ボールの落ちる位置
	Vec3 BallFallPoint(Vec3 playerPos, Vec3 playerRotation, Vec3 fallPos);
	//当たり判定
	void StageCollision(const float stageSize);
private:
	ObjectData pObject;                         //プレイヤーオブジェクト
	Vec3 position = {5.0f, 0.0f, 0.0f}; // 位置
	Vec3 rotation = {};

	// 所持者の位置
	Vec3 havePos_ = {};
	// ターゲットの位置
	Vec3 targetPos_ = {};
	// ボールの飛ぶベクトル
	Vec3 vector = {};
	// 所持しているか
	bool haveFlag_ = false;
	// 投げているか
	bool throwFlag_ = false;

	bool ballThrow = false;

	Vec3 reflectVector = {0.0f, 0.0f, 0.0f};
	bool isThrow = false;
	bool isHit = false;
	float baseReflectSpped = 0.5f;
	float baseBound = 4.0f;
	float basetime = 0.05f;
	float t = 0;
	float t2 = 0;
	float t3 = 0;
	float t4 = 0;
	float t5 = 0;
	float t6 = 0;
};

