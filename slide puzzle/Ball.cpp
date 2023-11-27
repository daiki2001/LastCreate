#include "Ball.h"
#include "GameInputManager.h"
#include "Input.h"
#include <Shape.h>
#include <random>
#include <Collision.h>
Ball::Ball() {}

Ball::~Ball() {}

void Ball::Init() { pObject = Shape::CreateOBJ("Ball"); }

void Ball::Update(Vec3 havePos_, Vec3 haveRotation, const float stageSize) {
	// ボール回収
	HaveAct(havePos_);

	// 所有者に追尾
	SetChainPosition(havePos_);

	// ボールが飛ぶ
	ThrowAct(targetPos_);

	// ボール跳ね返り
	BallReflectBound(havePos_, targetPos_);

	//ボールの挙動のステータスチェンジ
	StatusCalculation(havePos_, haveRotation, targetPos_);

	StageCollision(stageSize);
}

void Ball::Draw() { Object::Draw(pObject, position, Vec3(0.5f, 0.5f, 0.5f), rotation); }

void Ball::HaveAct(Vec3 havePos_) {
	// 投げてるときは捕らない
	if (throwFlag_) {
		haveFlag_ = false;
		return;
	}

	// 半径指定
	const float haveR = 1.0f;
	const float ballR = 1.0f;

	// 距離計算
	Vec3 axyz = (position - havePos_) * (position - havePos_);
	float ar = (ballR + haveR) * (ballR + haveR);

	// ボールに当たった
	if (axyz.x + axyz.y + axyz.z <= ar) {
		haveFlag_ = true;
		return;
	}

	// 当たってない
	haveFlag_ = false;
}

void Ball::SetChainPosition(Vec3 havePos_) {
	// 持っていないとき返す
	if (!haveFlag_) {return;}

	//位置リンク
	position = havePos_;
}

bool Ball::GetHitFlag()
{
	if (BallHitFlag(targetPos_)) {
		throwFlag_ = false;
		return true;
	}

	return false;
}

void Ball::ThrowAct(Vec3 targetPos) {
	// ボールの飛ぶベクトル
	Vec3 vector_ = {};

	// 投げた挙動
	if (throwFlag_) {
		vector_ = {targetPos.x - position.x, targetPos.y - position.y, targetPos.z - position.z};
		vector_ = vector_.normalize() * speed_;
		position += vector_;
	}
}

bool Ball::BallHitFlag(Vec3 targetPos) {
	// 半径指定
	const float targetR = 1.0f;
	const float ballR = 1.0f;

	// 距離計算
	Vec3 axyz = (position - targetPos) * (position - targetPos);
	float ar = (ballR + targetR) * (ballR + targetR);

	// ボールとターゲットの衝突判定
	if (axyz.x + axyz.y + axyz.z <= ar) {
		return true;
	}

	return false;
}

void Ball::BallReflectBound(Vec3 havePos_, Vec3 targetPos_) {
	Vec3 startPos = {};
	if (BallHitFlag(targetPos_)) {
		comboUpFlag_ = true;
		hitFlag_ = true;
	}

	if (!hitFlag_) {
		Vec3 startPos = position;
		for (auto& t : time_) {
			t = 0;
		}
	}
	// ボールが敵に当たったフラグ
	else {
		if (throwFlag_) {
			ReflectCalculation(havePos_);
			oldThrowPos = position;
		}
		
		chargeFlag_ = false;
		if (time_[0] < 1) {
			time_[0] += baseTime_;
			float y = time_[0] * (2 - time_[0]);
			position.x -= reflectVector_.x * 0.5f;
			position.y = startPos.y * (1.0f - y) + baseBound_ * y;
			position.z -= reflectVector_.z * 0.5f;
		} else if (1 <= time_[0] && time_[1] < 1) {
			time_[1] += baseTime_;
			float y = time_[1] * time_[1];
			position.x -= reflectVector_.x * 0.5f;
			position.y = baseBound_ * (1.0f - y) + 0.0f * y;
			position.z -= reflectVector_.z * 0.5f;
		} else if (1 <= time_[1] && time_[2] < 1) {
			comboMissFlag_ = true;
			time_[2] += baseTime_ * 2;
			float y = time_[2] * (2 - time_[2]);
			position.x -= reflectVector_.x * 0.25f;
			position.y = startPos.y * (1.0f - y) + (baseBound_ / 2) * y;
			position.z -= reflectVector_.z * 0.25f;
		} else if (1 <= time_[2] && time_[3] < 1) {
			time_[3] += baseTime_ * 2;
			float y = time_[3] * time_[3];
			position.x -= reflectVector_.x * 0.25f;
			position.y = (baseBound_ / 2) * (1.0f - y) + 0.0f * y;
			position.z -= reflectVector_.z * 0.25f;
		} else if (1 <= time_[3] && time_[4] < 1) {
			time_[4] += baseTime_ * 4;
			float y = time_[4] * (2 - time_[4]);
			position.x -= reflectVector_.x * 0.125f;
			position.y = startPos.y * (1.0f - y) + (baseBound_ / 4) * y;
			position.z -= reflectVector_.z * 0.125f;
		} else if (1 <= time_[4] && time_[5] < 1) {
			time_[5] += baseTime_ * 4;
			float y = time_[5] * time_[5];
			position.x -= reflectVector_.x * 0.125f;
			position.y = (baseBound_ / 4) * (1.0f - y) + 0.0f * y;
			position.z -= reflectVector_.z * 0.125f;
		}

		if (1 <= time_[5]) {
			baseReflectSpped_ = 0.25f;
			hitFlag_ = false;
		} else if (haveFlag_ && 0 <= time_[0]) {
			baseReflectSpped_ = 0.25f;
			SetChainPosition(havePos_);
			hitFlag_ = false;
		}
	}
}

void Ball::ReflectCalculation(Vec3 havePos_) {
	// 跳ね返り向きの計算
	Vec3 refVec = position - fallPositionCal_;
	float v = sqrtf((refVec.x * refVec.x) + (refVec.y * refVec.y) + (refVec.z * refVec.z));
	reflectVector_ = (refVec / v) * baseReflectSpped_;
}

void Ball::BallFallPoint(Vec3 havePos_, Vec3 playerRotation) {
	// 跳ね返る方向の座標決定
	XMVECTOR v0 = {0.0f, 0.0f, flyVectorRandom_, 0};

	// プレイヤーの左右のどちらかに座標を指定
	XMMATRIX rotM = XMMatrixIdentity();
	rotM *= XMMatrixRotationX(XMConvertToRadians(0.0f));
	rotM *= XMMatrixRotationY(XMConvertToRadians(180.0f));
	XMVECTOR v = XMVector3TransformNormal(v0, rotM);
	XMVECTOR havePosXMVec = {havePos_.x, havePos_.y, havePos_.z};
	XMVECTOR v3 = havePosXMVec + v;
	fallPositionCal_ = {v3.m128_f32[0], v3.m128_f32[1], v3.m128_f32[2]};
}

void Ball::ThrowPowerChange() {
	//跳ね返り距離チャージ
	baseReflectSpped_ += chargeValue_;

	// マックスに到達したらそれ以上上がらない
	if (maxReflectSpeed_ <= baseReflectSpped_) {
		baseReflectSpped_ = maxReflectSpeed_;
	}
}

void Ball::FlyTimeChange(Vec3 havePos_, Vec3 targetPos_) {
	// 敵との距離計算
	Vec3 axyz = (targetPos_ - havePos_) * (targetPos_ - havePos_);
	float r = boundChangeR * boundChangeR;

	// ボールに当たった
	if (axyz.x + axyz.z <= r) // 近い時
	{
		// 時間短い
		baseBound_ = nearBound_;
		baseTime_ = nearTime_;
	} else // 遠い時
	{
		// 時間長い
		baseBound_ = farBound_;
		baseTime_ = farTime_;
	}
}

void Ball::FlyVectorCal() {
	// 跳ね返る方向のランダム数値
	std::random_device rnd;
	std::mt19937 mt(rnd());
	std::uniform_int_distribution<> rand2(minFlyVectorRandom_, maxFlyVectorRandom_);
	flyVectorRandom_ = float(rand2(mt)); // -5 ~ +5の範囲
}

void Ball::StatusCalculation(Vec3 havePos_, Vec3 haveRotation, Vec3 targetPos_){
	//持っていないときとチャージしていないとき返す
	if (!haveFlag_ || !chargeFlag_) {return;}
	
	// 投げる強さ調整
	ThrowPowerChange();

	// 滞空時間調整
	FlyTimeChange(havePos_, targetPos_);

	// 飛ぶ向きのランダム化
	FlyVectorCal();

	// ボールの落ちる位置
	BallFallPoint(havePos_, haveRotation);
}


void Ball::StageCollision(const float stageSize) {
	// 円の当たり判定
	if (!Collision::CircleCollision(Vec2(position.x, position.z), Vec2(), 1.0f, stageSize) &&
	    !refflaga) {
		AngleCalculation();
		WallRefrectCal();
		ReflectCalculation({0.0f, 0.0f, 0.0f});
		refflaga = true;
	} else {
		refflaga = false;
	}
}

void Ball::AngleCalculation() {
	wallPos = position;
	Vec3 centerPos = {0.0f, 0.0f, 0.0f};

	Vec3 vecAB = {centerPos.x - wallPos.x, 0.0f, centerPos.z - wallPos.z};
	Vec3 vecAC = {oldThrowPos.x - wallPos.x, 0.0f, oldThrowPos.z - wallPos.z};
	float abb =
	    vecAB.normalize().x * vecAC.normalize().x + vecAB.normalize().z * vecAC.normalize().z;
	float angleABC = acosf(abb);
	wallRefVec = XMConvertToDegrees(angleABC);
	// float aToB = ((wallPos.x * centerPos.y) - (centerPos.x * wallPos.y));
	// float aToC = ((wallPos.x * oldThrowPos.y) - (oldThrowPos.x * wallPos.y));
	float aToB = Cross(Vec2(wallPos.x, wallPos.z), Vec2(centerPos.x, centerPos.z));
	float aToC = Cross(Vec2(wallPos.x, wallPos.z), Vec2(oldThrowPos.x, oldThrowPos.z));

	if (aToC < 0) {
		wallRefVec *= -1;
	}

	abc = XMConvertToDegrees(float(atan2(vecAC.x, vecAC.z)));
}

void Ball::WallRefrectCal() {
	// 跳ね返る方向の座標決定
	XMVECTOR v0 = {0.0f, 0.0f, 5.0f, 0};
	// プレイヤーの左右のどちらかに座標を指定
	XMMATRIX rotM = XMMatrixIdentity();
	rotM *= XMMatrixRotationX(XMConvertToRadians(0.0f));
	rotM *= XMMatrixRotationY(XMConvertToRadians(abc + (-wallRefVec * 2)));
	XMVECTOR v = XMVector3TransformNormal(v0, rotM);
	XMVECTOR cameraPos = {position.x, position.y, position.z};
	XMVECTOR v3 = cameraPos + v;
	fallPositionCal_ = {v3.m128_f32[0], v3.m128_f32[1], v3.m128_f32[2]};
}

float Ball::Cross(Vec2 a, Vec2 b) { return a.x * b.y - a.y * b.x; }
