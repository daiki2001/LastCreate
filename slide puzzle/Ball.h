#pragma once
#include "Object.h"
#include <memory>
#include <array>
#include <DirectXMath.h>
#include <ParticleManager.h>
class Ball {
	enum wallHitType {
		front,
		back,
		right,
		left
	};

public:

	Ball();
	~Ball();
	void Init();
	void Update(Vec3 havePos_, Vec3 haveRotation, const float stageSize);
	void Draw();
	void AfterDraw();
	void HaveAct(Vec3 havePos_);
	void SetChainPosition(Vec3 havePos_);
	void SetPosition(Vec3 pos) { position = pos; }
	Vec3 GetPosition() { return position; }
	void SetHaveFlag(bool flag) { haveFlag_ = flag; }
	bool GetHaveFlag() { return haveFlag_; }
	void SetThrowFlag(bool flag) { throwFlag_ = flag; }
	bool GetThrowFlag() { return throwFlag_; }
	void SetChargeFlag(bool flag) { chargeFlag_ = flag; }
	bool GetChargeFlag() { return chargeFlag_; }
	void SetComboUpFlag(bool flag) { comboUpFlag_ = flag; }
	bool GetComboUpFlag() { return comboUpFlag_; }
	void SetComboMissFlag(bool flag) { comboMissFlag_ = flag; }
	bool GetComboMissFlag() { return comboMissFlag_; }
	void SetTargetPos(Vec3 pos) { targetPos_ = pos; }
	bool GetHitFlag();

private:
	void ThrowAct(Vec3 targetPos_);
	bool BallHitFlag(Vec3 targetPos);
	void BallReflectBound(Vec3 havePos_, Vec3 targetPos_);
	void ReflectCalculation(Vec3 havePos_);
	void BallFallPoint(Vec3 havePos_, Vec3 playerRotation);
	void ThrowPowerChange();
	void FlyTimeChange(Vec3 havePos_, Vec3 targetPos_);
	void FlyVectorCal();
	void StatusCalculation(Vec3 havePos_, Vec3 haveRotation, Vec3 targetPos_);
	void StageCollision(const float stageSize);
	void AngleCalculation(wallHitType hitType);
	void WallRefrectCal();
	void Effect();
	float Cross(Vec2 a, Vec2 b);
	void EnemyThrowAct(Vec3 targetPos_);
private:
	ObjectData pObject;
	Vec3 position = { 0.0f, 0.0f, -5.0f};
	Vec3 rotation = {};
	Vec3 targetPos_ = {};
	bool haveFlag_ = false;
	bool throwFlag_ = false;
	bool chargeFlag_ = false;
	bool hitFlag_ = false;
	bool comboUpFlag_ = false;
	bool comboMissFlag_ = false;
	float speed_ = 0.5f;
	float baseBound_ = 0.0f;
	float nearBound_ = 4.0f;
	float farBound_ = 6.0f;
	float baseTime_ = 0.05f;
	float nearTime_ = 0.01625f;
	float farTime_ = 0.0125f;
	std::array<float, 6> time_ = { 0, 0, 0, 0, 0, 0 };
	float boundChangeR = 22.5f;
	float chargeValue_ = 0.005f;
	Vec3 fallPositionCal_ = { 0.0f, 0.0f, 0.0f };
	Vec3 reflectVector_ = { 0.0f, 0.0f, 0.0f };
	float maxReflectSpeed_ = 3.0f;
	float baseReflectSpped_ = 0.25f;
	int maxFlyVectorRandom_ = 5;
	int minFlyVectorRandom_ = -5;
	float flyVectorRandom_ = 0.0f;
	Vec3 wallPos = { 0.0f, 0.0f, 0.0f };
	Vec3 oldThrowPos = { 0.0f, 0.0f, 0.0f };
	float wallRefVec = 0.0f;
	bool refflaga = false;
	float abc = 0;
	int effectCount_ = 0;
	ObjectData landmarkObj;
	TextureData landmarkTex;
	std::unique_ptr<ParticleManager>damegeParticle = nullptr;
	TextureData damegeGraph;
	std::unique_ptr<ParticleManager>ballLineParticle = nullptr;
	TextureData ballLineGraph;

	bool enemyHave = false;

	bool wallReflectFlag = false;
};
