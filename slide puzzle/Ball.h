#pragma once
#include "Object.h"
#include <memory>

class Ball {
public:
	// コンストラクタ
	Ball();
	// デスコンストラクタ
	~Ball();
	/// <summary>
	/// 初期化
	/// </summary>
	void Init();
	/// <summary>
	/// 更新
	/// </summary>
	void Update(Vec3 playerPos, Vec3 playerRotation, Vec3 enemyPos);
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	//
	bool HaveHit(Vec3 pos);

	// 位置の設定
	void SetChainPosition(Vec3 pos);
	void SetPosition(Vec3 pos) { position = pos; }
	const Vec3 GetPosition() { return position; }
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
	void ThrowAct(Vec3 enemyPos);
	// 所持者関係
	void HaveAct();
	// ボールが当たったフラグ
	bool BallHitFlag(Vec3 enemyPos);
	// ボールが当たって反射
	void BallReflectBound(Vec3 playerPos, Vec3 enemyPos);
	// 　跳ね返る方向計算
	void ReflectCalculation(Vec3 playerPos);
	// ボールの落ちる位置
	Vec3 BallFallPoint(Vec3 playerPos, Vec3 playerRotation, Vec3 fallPos);
	// 投げる強さ調整
	void ThrowPowerChange();
	// 滞空時間調整
	void FlyTimeChange(Vec3 playerPos, Vec3 enemyPos);
	// 飛ぶ向きのランダム化
	void FlyVectorCal();

private:
	ObjectData pObject;                 // プレイヤーオブジェクト
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
	float baseReflectSpped = 0.25f;
	float baseBound = 4.0f;
	float basetime = 0.05f;
	float t = 0;
	float t2 = 0;
	float t3 = 0;
	float t4 = 0;
	float t5 = 0;
	float t6 = 0;
	float fallPosition = 0.0f;
	Vec3 fallPositionCal = {0.0f, 0.0f, 0.0f};
	float throwPower = 0;
	bool chargeFlag = false;
	bool trigerFlag = false;
	float flyVectorRandum = 0.0f;
};
