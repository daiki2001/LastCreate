#include "Ball.h"
#include "Input.h"
#include <Shape.h>

Ball::Ball() {}

Ball::~Ball() {}

void Ball::Init() { pObject = Shape::CreateOBJ("Ball"); }

void Ball::Update(Vec3 playerPos, Vec3 playerRotation) {
	// ボール回収
	if (HaveHit(playerPos) && !isThrow) {
		SetChainPosition(playerPos);
		// 投げられたフラグ
		if (Input::Get()->KeybordPush(DIK_B)) {
			isThrow = true;
		}
	}
	BallFallPoint(playerPos, playerRotation, Vec3{-5.0f, 0.0f, 0.0f});
	HaveAct();
	ThrowAct();
	BallReflectBound(playerPos);
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

void Ball::ThrowAct() {
	// 投げた挙動
	if (isThrow) {
		Vec3 enemyPos = Vec3(-10.0f, 0.0f, 0.01f);
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

bool Ball::BallHitFlag() {
	Vec3 enemyPos = Vec3(-10.0f, 0.0f, 0.00f);
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

void Ball::BallReflectBound(Vec3 playerPos) {
	Vec3 startPos = {};
	if (BallHitFlag()) {
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
		isThrow = false;
		ReflectCalculation(playerPos);
		if (t < 1) {
			t += basetime;
			float y = t * (2 - t);
			position.x -= reflectVector.x * 0.5f;
			position.y = startPos.y * (1.0f - y) + baseBound * y;
			position.z -= reflectVector.z * 0.5f;
		}

		if (1 <= t && t2 < 1) {
			t2 += basetime;
			float y = t2 * t2;
			position.x -= reflectVector.x * 0.5f;
			position.y = baseBound * (1.0f - y) + 0.0f * y;
			position.z -= reflectVector.z * 0.5f;
		}

		if (1 <= t2 && t3 < 1) {
			t3 += basetime * 2;
			float y = t3 * (2 - t3);
			position.x -= reflectVector.x * 0.25f;
			position.y = startPos.y * (1.0f - y) + (baseBound / 2) * y;
			position.z -= reflectVector.z * 0.25f;
		}

		if (1 <= t3 && t4 < 1) {
			t4 += basetime * 2;
			float y = t4 * t4;
			position.x -= reflectVector.x * 0.25f;
			position.y = (baseBound / 2) * (1.0f - y) + 0.0f * y;
			position.z -= reflectVector.z * 0.25f;
		}

		if (1 <= t4 && t5 < 1) {
			t5 += basetime * 4;
			float y = t5 * (2 - t5);
			position.x -= reflectVector.x * 0.125f;
			position.y = startPos.y * (1.0f - y) + (baseBound / 4) * y;
			position.z -= reflectVector.z * 0.125f;
		}

		if (1 <= t5 && t6 < 1) {
			t6 += basetime * 4;
			float y = t6 * t6;
			position.x -= reflectVector.x * 0.125f;
			position.y = (baseBound / 4) * (1.0f - y) + 0.0f * y;
			position.z -= reflectVector.z * 0.125f;
		}

		if (1 <= t6 || HaveHit(playerPos)) {
			isHit = false;
		}
	}
}

void Ball::ReflectCalculation(Vec3 playerPos) {
	Vec3 refVec = {position.x - playerPos.x, position.y - playerPos.y, position.z - playerPos.z};
	float v = sqrtf((refVec.x * refVec.x) + (refVec.y * refVec.y) + (refVec.z * refVec.z));
	reflectVector = {
	    (refVec.x / v) * baseReflectSpped, (refVec.y / v) * baseReflectSpped,
	    (refVec.z / v) * baseReflectSpped};
}

Vec3 Ball::BallFallPoint(Vec3 playerPos, Vec3 playerRotation, Vec3 fallPos) {
	// 視点計算
	XMVECTOR v0 = {fallPos.x, fallPos.y, fallPos.z, 0};

	// angleラジアンだけy軸まわりに回転。半径は-5
	XMMATRIX rotM = XMMatrixIdentity();
	rotM *= XMMatrixRotationX(XMConvertToRadians(0.0f));
	rotM *= XMMatrixRotationY(XMConvertToRadians(180.0f));
	XMVECTOR v = XMVector3TransformNormal(v0, rotM);
	XMVECTOR cameraPos = {playerPos.x, playerPos.y, playerPos.z};
	XMVECTOR v3 = cameraPos + v;
	Vec3 f = {v3.m128_f32[0], v3.m128_f32[1], v3.m128_f32[2]};
	return f;
}
