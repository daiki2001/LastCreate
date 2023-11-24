#pragma once
#include "Object.h"
#include <memory>

class Ball {
public:
	// �R���X�g���N�^
	Ball();
	// �f�X�R���X�g���N�^
	~Ball();
	/// <summary>
	/// ������
	/// </summary>
	void Init();
	/// <summary>
	/// �X�V
	/// </summary>
	void Update(Vec3 playerPos, Vec3 playerRotation, Vec3 enemyPos);
	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	//
	bool HaveHit(Vec3 pos);

	// �ʒu�̐ݒ�
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
	// �{�[���̋���
	void ThrowAct(Vec3 enemyPos);
	// �����Ҋ֌W
	void HaveAct();
	// �{�[�������������t���O
	bool BallHitFlag(Vec3 enemyPos);
	// �{�[�����������Ĕ���
	void BallReflectBound(Vec3 playerPos, Vec3 enemyPos);
	// �@���˕Ԃ�����v�Z
	void ReflectCalculation(Vec3 playerPos);
	// �{�[���̗�����ʒu
	Vec3 BallFallPoint(Vec3 playerPos, Vec3 playerRotation, Vec3 fallPos);
	// �����鋭������
	void ThrowPowerChange();
	// �؋󎞊Ԓ���
	void FlyTimeChange(Vec3 playerPos, Vec3 enemyPos);
	// ��Ԍ����̃����_����
	void FlyVectorCal();

private:
	ObjectData pObject;                 // �v���C���[�I�u�W�F�N�g
	Vec3 position = {5.0f, 0.0f, 0.0f}; // �ʒu
	Vec3 rotation = {};

	// �����҂̈ʒu
	Vec3 havePos_ = {};
	// �^�[�Q�b�g�̈ʒu
	Vec3 targetPos_ = {};
	// �{�[���̔�ԃx�N�g��
	Vec3 vector = {};
	// �������Ă��邩
	bool haveFlag_ = false;
	// �����Ă��邩
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
