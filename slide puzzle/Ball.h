#pragma once
#include"Object.h"
#include<memory>
class Ball
{
public:
	//�R���X�g���N�^
	Ball();
	//�f�X�R���X�g���N�^
	~Ball();
	/// <summary>
	/// ������
	/// </summary>
	void Init();
	/// <summary>
	/// �X�V
	/// </summary>
	void Update(Vec3 playerPos, Vec3 playerRotation,const float stageSize);
	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	//
	bool HaveHit(Vec3 pos);

	// �ʒu�̐ݒ�
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
	// �{�[���̋���
	void ThrowAct();
	// �����Ҋ֌W
	void HaveAct();
	// �{�[�������������t���O
	bool BallHitFlag();
	// �{�[�����������Ĕ���
	void BallReflectBound(Vec3 playerPos);
	// �@���˕Ԃ�����v�Z
	void ReflectCalculation(Vec3 playerPos);
	// �{�[���̗�����ʒu
	Vec3 BallFallPoint(Vec3 playerPos, Vec3 playerRotation, Vec3 fallPos);
	//�����蔻��
	void StageCollision(const float stageSize);
private:
	ObjectData pObject;                         //�v���C���[�I�u�W�F�N�g
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

