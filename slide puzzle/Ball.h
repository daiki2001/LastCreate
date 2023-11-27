#pragma once
#include "Object.h"
#include <memory>
#include <array>
#include <DirectXMath.h>
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
	
	// 所持フラグセット
	void SetHaveFlag(bool flag) { haveFlag_ = flag; }
	// 所持フラグゲット
	bool GetHaveFlag() { return haveFlag_; }
	// 投げるフラグセット
	void SetThrowFlag(bool flag) { throwFlag_ = flag; }
	// 投げるフラグゲット
	bool GetThrowFlag() { return throwFlag_; }
	// チャージフラグセット
	void SetChargeFlag(bool flag) { chargeFlag_ = flag; }
	// チャージフラグゲット
	bool GetChargeFlag() { return chargeFlag_; }
	// コンボアップフラグセット
	void SetComboUpFlag(bool flag) { comboUpFlag_ = flag; }
	// コンボアップフラグゲット
	bool GetComboUpFlag() { return comboUpFlag_; }
	// コンボ失敗フラグセット
	void SetComboMissFlag(bool flag) { comboMissFlag_ = flag; }
	// コンボ失敗フラグゲット
	bool GetComboMissFlag() { return comboMissFlag_; }

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
	// 壁跳ね返りの角度計算
	void AngleCalculation();
	// 壁跳ね返りの位置情報の送り
	void WallRefrectCal();

	float Cross(Vec2 a, Vec2 b);

private:
	ObjectData pObject;                 // プレイヤーオブジェクト
	Vec3 position = {5.0f, 0.0f, 0.0f}; // 位置
	Vec3 rotation = {};                 // 回転軸

	// 所持しているか
	bool haveFlag_ = false;
	// 投げているか
	bool throwFlag_ = false;
	// チャージしてるか
	bool chargeFlag_ = false;
	// 当たったかどうかのフラグ
	bool hitFlag_ = false; 
	// コンボアップフラグ
	bool comboUpFlag_ = false;
	// コンボミスフラグ
	bool comboMissFlag_ = false;

	// ボールのスピード
	float speed_ = 0.5f;
	// バウンドの高さ
	float baseBound_ = 0.0f;
	// バウンドの高さ(近い時)
	float nearBound_ = 4.0f;
	// バウンドの高さ(遠い時)
	float farBound_ = 6.0f;
	// バウンドの時間
	float baseTime_ = 0.05f;
	// バウンドの時間(近い時)
	float nearTime_ = 0.01625f;
	// バウンドの時間(遠い時)
	float farTime_ = 0.0125f;
	// イージングのタイム
	std::array<float, 6> time_ = {0, 0, 0, 0, 0, 0};
	// バウンドの長さ変わる距離
	float boundChangeR = 22.5f;


	// チャージの速さ
	float chargeValue_ = 0.005f;   
	// ボールの跳ね返る座標
	Vec3 fallPositionCal_ = {0.0f, 0.0f, 0.0f}; 
	// 跳ね返り方向
	Vec3 reflectVector_ = {0.0f, 0.0f, 0.0f};
	// 跳ね返りのスピードの最大値
	float maxReflectSpeed_ = 4.0f;
	// 跳ね返りスピード
	float baseReflectSpped_ = 0.25f;
	// 跳ね返る方向のMAX数値
	int maxFlyVectorRandom_ = 5;
	// 跳ね返る方向のMIN数値
	int minFlyVectorRandom_ = -5;
	// 跳ね返る方向のランダム数値
	float flyVectorRandom_ = 0.0f; 
	// 壁の向き確認のための座標
	Vec3 wallPos = {0.0f, 0.0f, 0.0f};
	// 投げられた場所のポジション
	Vec3 oldThrowPos = {0.0f, 0.0f, 0.0f};
	//
	float wallRefVec = 0.0f;
	bool refflaga = false;
	float abc = 0;
};
