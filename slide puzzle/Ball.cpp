#include "Ball.h"
#include "GameInputManager.h"
#include "Input.h"
#include <Shape.h>
#include <random>
#include <Collision.h>
Ball::Ball() {}

Ball::~Ball() {}

void Ball::Init() { pObject = Shape::CreateOBJ("Ball"); }

void Ball::Update(Vec3 playerPos, Vec3 playerRotation, Vec3 enemyPos, float stageSize) {
	// ボール回収
	if (HaveHit(playerPos) && !isThrow) {
		SetChainPosition(playerPos);
		// 投げられたフラグ
		if (GameInputManager::Get()->IsThrow() || Input::Get()->KeybordPush(DIK_B)) {
			ThrowPowerChange();
			FlyTimeChange(playerPos, enemyPos);
			FlyVectorCal();
		} else {
			chargeFlag = false;
		}

		if (chargeFlag == false && trigerFlag == true) {
			fallPositionCal =
			    BallFallPoint(playerPos, playerRotation, Vec3{0.0f, 0.0f, fallPosition});
			isThrow = true;
		}
	}

	HaveAct();
	ThrowAct(enemyPos);
	BallReflectBound(playerPos, enemyPos);
}

void Ball::Draw() { Object::Draw(pObject, position, Vec3(0.5f, 0.5f, 0.5f), rotation); }

bool Ball::HaveHit(Vec3 pos) {
	const float playerR = 1.0f;
	const float ballR = 1.0f;

	Vec3 axyz = (position - pos) * (position - pos);
	float ar = (ballR + playerR) * (ballR + playerR);

	// ボールに当たった
	if (axyz.x + axyz.y + axyz.z <= ar) {
		return true;
	}

	// 当たってない
	return false;
}

void Ball::SetChainPosition(Vec3 pos) {
	// if (!haveFlag_ && !throwFlag_) { return; }

	position = havePos_ = pos;
}

void Ball::ThrowAct(Vec3 enemyPos) {
	// 投げた挙動
	if (isThrow) {
		vector = {enemyPos.x - position.x, enemyPos.y - position.y, enemyPos.z - position.z};
		float speed = 0.25f;
		vector = vector.normalize() * speed;
		position += vector;
	}
}

void Ball::HaveAct() {
	if (!haveFlag_) {
		return;
	}

	// ターゲットの位置
	targetPos_ = {};
}

bool Ball::BallHitFlag(Vec3 enemyPos) {
	const float enemyR = 1.0f;
	const float ballR = 1.0f;

	Vec3 axyz = (position - enemyPos) * (position - enemyPos);
	float ar = (ballR + enemyR) * (ballR + enemyR);

	// ボールに当たった
	if (axyz.x + axyz.y + axyz.z <= ar) {
		return true;
	}

	return false;
}

void Ball::BallReflectBound(Vec3 playerPos, Vec3 enemyPos) {
	Vec3 startPos = {};
	if (BallHitFlag(enemyPos)) {
		isHit = true;
	}

	if (!isHit) {
		Vec3 startPos = position;
		t = 0;
		t2 = 0;
		t3 = 0;
		t4 = 0;
		t5 = 0;
		t6 = 0;
	}
	// ボールが敵に当たったフラグ
	else {
		if (isThrow) {
			ReflectCalculation(playerPos);
		}
		isThrow = false;

		if (t < 1) {
			t += basetime;
			float y = t * (2 - t);
			position.x -= reflectVector.x * 0.5f;
			position.y = startPos.y * (1.0f - y) + baseBound * y;
			position.z -= reflectVector.z * 0.5f;
		} else if (1 <= t && t2 < 1) {
			t2 += basetime;
			float y = t2 * t2;
			position.x -= reflectVector.x * 0.5f;
			position.y = baseBound * (1.0f - y) + 0.0f * y;
			position.z -= reflectVector.z * 0.5f;
		} else if (1 <= t2 && t3 < 1) {
			t3 += basetime * 2;
			float y = t3 * (2 - t3);
			position.x -= reflectVector.x * 0.25f;
			position.y = startPos.y * (1.0f - y) + (baseBound / 2) * y;
			position.z -= reflectVector.z * 0.25f;
		} else if (1 <= t3 && t4 < 1) {
			t4 += basetime * 2;
			float y = t4 * t4;
			position.x -= reflectVector.x * 0.25f;
			position.y = (baseBound / 2) * (1.0f - y) + 0.0f * y;
			position.z -= reflectVector.z * 0.25f;
		} else if (1 <= t4 && t5 < 1) {
			t5 += basetime * 4;
			float y = t5 * (2 - t5);
			position.x -= reflectVector.x * 0.125f;
			position.y = startPos.y * (1.0f - y) + (baseBound / 4) * y;
			position.z -= reflectVector.z * 0.125f;
		} else if (1 <= t5 && t6 < 1) {
			t6 += basetime * 4;
			float y = t6 * t6;
			position.x -= reflectVector.x * 0.125f;
			position.y = (baseBound / 4) * (1.0f - y) + 0.0f * y;
			position.z -= reflectVector.z * 0.125f;
		}

		if (1 <= t6) {
			baseReflectSpped = 0.25f;
			trigerFlag = false;
			isHit = false;
		} else if (HaveHit(playerPos) && 0 <= t) {
			baseReflectSpped = 0.25f;
			trigerFlag = false;
			SetChainPosition(playerPos);
			isHit = false;
		}
	}
}

void Ball::ReflectCalculation(Vec3 playerPos) {
	Vec3 refVec = {
	    position.x - fallPositionCal.x, position.y - fallPositionCal.y,
	    position.z - fallPositionCal.z};
	float v = sqrtf((refVec.x * refVec.x) + (refVec.y * refVec.y) + (refVec.z * refVec.z));
	reflectVector = {
		(refVec.x / v) * baseReflectSpped, (refVec.y / v) * baseReflectSpped,
		(refVec.z / v) * baseReflectSpped };
}

Vec3 Ball::BallFallPoint(Vec3 playerPos, Vec3 playerRotation, Vec3 fallPos) {
	// 視点計算
	XMVECTOR v0 = {fallPos.x, fallPos.y, fallPos.z + flyVectorRandum - 5.0f, 0};

	// angleラジアンだけy軸まわりに回転。半径は-5
	XMMATRIX rotM = XMMatrixIdentity();
	rotM *= XMMatrixRotationX(XMConvertToRadians(0.0f));
	rotM *= XMMatrixRotationY(XMConvertToRadians(180.0f));
	XMVECTOR v = XMVector3TransformNormal(v0, rotM);
	XMVECTOR cameraPos = { playerPos.x, playerPos.y, playerPos.z };
	XMVECTOR v3 = cameraPos + v;
	Vec3 f = { v3.m128_f32[0], v3.m128_f32[1], v3.m128_f32[2] };
	return f;
}

void Ball::ThrowPowerChange() {
	chargeFlag = true;
	trigerFlag = true;
	baseReflectSpped += 0.005f;
	if (0.4f <= baseReflectSpped) {
		baseReflectSpped = 0.4f;
	}
}

void Ball::FlyTimeChange(Vec3 playerPos, Vec3 enemyPos) {
	Vec3 axyz = (enemyPos - playerPos) * (enemyPos - playerPos);
	float r = 22.5f * 22.5f;
	// ボールに当たった
	if (axyz.x + axyz.z <= r) {
		baseBound = 4.0f;
		basetime = 0.01625f;
	} else {
		baseBound = 6.0f;
		basetime = 0.0125f;
	}
}

void Ball::FlyVectorCal() {
	std::random_device rnd;
	std::mt19937 mt(rnd());
	std::uniform_int_distribution<> rand2(0, 10); // 0~2の範囲
	flyVectorRandum = float(rand2(mt));
}

void Ball::StageCollision(const float stageSize)
{
	if (!haveFlag_) {
		return;
	}
	if (!Collision::CircleCollision(Vec2(position.x, position.z), Vec2(), 1.0f, stageSize))
	{
		float length = sqrt(position.x * position.x + position.z * position.z);
		//差
		float  difference = length - stageSize;
		Vec2 normalize = { position.x / length,position.z / length };
		position.x -= normalize.x * difference;
		position.z -= normalize.y * difference;
	}
}