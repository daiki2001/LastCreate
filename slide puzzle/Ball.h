#pragma once
#include "Object.h"
#include <memory>
#include <array>

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
	void Update(Vec3 havePos_, Vec3 haveRotation, Vec3 targetPos_, const float stageSize);
	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	//
	void HaveAct(Vec3 havePos_);

	// �ʒu�̐ݒ�
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
	// �{�[���̋���
	void ThrowAct(Vec3 targetPos_);
	// �{�[�������������t���O
	bool BallHitFlag(Vec3 targetPos);
	// �{�[�����������Ĕ���
	void BallReflectBound(Vec3 havePos_, Vec3 targetPos_);
	// �@���˕Ԃ�����v�Z
	void ReflectCalculation(Vec3 havePos_);
	// �{�[���̗�����ʒu
	void BallFallPoint(Vec3 havePos_, Vec3 playerRotation);
	// �����鋭������
	void ThrowPowerChange();
	// �؋󎞊Ԓ���
	void FlyTimeChange(Vec3 havePos_, Vec3 targetPos_);
	// ��Ԍ����̃����_����
	void FlyVectorCal();
	// �`���[�W���̏���
	void StatusCalculation(Vec3 havePos_, Vec3 haveRotation, Vec3 targetPos_);
	//�@�X�e�[�W�̕Ǔ����蔻��
	void StageCollision(const float stageSize);

private:
	ObjectData pObject;                 // �v���C���[�I�u�W�F�N�g
	Vec3 position = {5.0f, 0.0f, 0.0f}; // �ʒu
	Vec3 rotation = {};

	// �������Ă��邩
	bool haveFlag_ = false;
	// �����Ă��邩
	bool throwFlag_ = false;
	// �`���[�W���Ă邩
	bool chargeFlag_ = false;

	bool isHit = false; // �����������ǂ����̃t���O

	// ���˕Ԃ����
	Vec3 reflectVector_ = {0.0f, 0.0f, 0.0f};

	float speed = 0.25f;            // �{�[���̃X�s�[�h
	float baseReflectSpped = 0.25f; // ���˃X�s�[�h
	float baseBound = 4.0f;         // �o�E���h�̍���
	float baseTime = 0.05f;         // �o�E���h�̎���

	std::array<float, 6> time = {0, 0, 0, 0, 0, 0}; // �^�C��

	Vec3 fallPositionCal = {0.0f, 0.0f, 0.0f}; // �{�[���̒��˕Ԃ���W

	float maxReflectSpeed = 4.0f; // ���˕Ԃ�̃X�s�[�h�̍ő�l
	float chargeValue = 0.005f;   // �`���[�W�̑���

	float flyVectorRandum = 0.0f; // ���˕ς�������̃����_�����l
};
