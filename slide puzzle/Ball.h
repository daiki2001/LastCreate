#pragma once
#include "Object.h"
#include <memory>
#include <array>

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
	void Update(Vec3 havePos_, Vec3 haveRotation, Vec3 targetPos_, const float stageSize);
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	//
	void HaveAct(Vec3 havePos_);

	// 位置の設定
	void SetChainPosition(Vec3 havePos_);
	void SetPosition(Vec3 pos) { position = pos; }
	//
	void SetHaveFlag(bool flag) { haveFlag_ = flag; }
	//
	bool GetHaveFlag() { return haveFlag_; }
	//
	void SetThrowFlag(bool flag) { throwFlag_ = flag; }
	//
	bool GetThrowFlag() { return throwFlag_; }
	//
	void SetChargeFlag(bool flag) { chargeFlag_ = flag; }
	//
	bool GetChargeFlag() { return chargeFlag_; }


private:
	// ボールの挙動
	void ThrowAct(Vec3 targetPos_);
	// ボールが当たったフラグ
	bool BallHitFlag(Vec3 targetPos);
	// ボールが当たって反射
	void BallReflectBound(Vec3 havePos_, Vec3 targetPos_);
	// 　跳ね返る方向計算
	void ReflectCalculation(Vec3 havePos_);
	// ボールの落ちる位置
	void BallFallPoint(Vec3 havePos_, Vec3 playerRotation);
	// 投げる強さ調整
	void ThrowPowerChange();
	// 滞空時間調整
	void FlyTimeChange(Vec3 havePos_, Vec3 targetPos_);
	// 飛ぶ向きのランダム化
	void FlyVectorCal();
	// チャージ中の処理
	void StatusCalculation(Vec3 havePos_, Vec3 haveRotation, Vec3 targetPos_);
	//　ステージの壁当たり判定
	void StageCollision(const float stageSize);

private:
	ObjectData pObject;                 // プレイヤーオブジェクト
	Vec3 position = {5.0f, 0.0f, 0.0f}; // 位置
	Vec3 rotation = {};

	// 所持しているか
	bool haveFlag_ = false;
	// 投げているか
	bool throwFlag_ = false;
	// チャージしてるか
	bool chargeFlag_ = false;

	bool isHit = false; // 当たったかどうかのフラグ

	// 跳ね返り方向
	Vec3 reflectVector_ = {0.0f, 0.0f, 0.0f};

	float speed = 0.25f;            // ボールのスピード
	float baseReflectSpped = 0.25f; // 反射スピード
	float baseBound = 4.0f;         // バウンドの高さ
	float baseTime = 0.05f;         // バウンドの時間

	std::array<float, 6> time = {0, 0, 0, 0, 0, 0}; // タイム

	Vec3 fallPositionCal = {0.0f, 0.0f, 0.0f}; // ボールの跳ね返る座標

	float maxReflectSpeed = 4.0f; // 跳ね返りのスピードの最大値
	float chargeValue = 0.005f;   // チャージの速さ

	float flyVectorRandum = 0.0f; // 跳ね変える方向のランダム数値
};
