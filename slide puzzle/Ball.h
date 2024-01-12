#pragma once
#include "Object.h"
#include <memory>
#include <array>
#include <DirectXMath.h>
#include <ParticleManager.h>
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
	void Update(Vec3 havePos_, Vec3 haveRotation, const float stageSize);
	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	void AfterDraw();
	//
	void HaveAct(Vec3 havePos_);

	// �ʒu�̐ݒ�
	void SetChainPosition(Vec3 havePos_);
	void SetPosition(Vec3 pos) { position = pos; }
	Vec3 GetPosition() { return position; }
	// �����t���O�Z�b�g
	void SetHaveFlag(bool flag) { haveFlag_ = flag; }
	// �����t���O�Q�b�g
	bool GetHaveFlag() { return haveFlag_; }
	// ������t���O�Z�b�g
	void SetThrowFlag(bool flag) { throwFlag_ = flag; }
	// ������t���O�Q�b�g
	bool GetThrowFlag() { return throwFlag_; }
	// �`���[�W�t���O�Z�b�g
	void SetChargeFlag(bool flag) { chargeFlag_ = flag; }
	// �`���[�W�t���O�Q�b�g
	bool GetChargeFlag() { return chargeFlag_; }
	// �R���{�A�b�v�t���O�Z�b�g
	void SetComboUpFlag(bool flag) { comboUpFlag_ = flag; }
	// �R���{�A�b�v�t���O�Q�b�g
	bool GetComboUpFlag() { return comboUpFlag_; }
	// �R���{���s�t���O�Z�b�g
	void SetComboMissFlag(bool flag) { comboMissFlag_ = flag; }
	// �R���{���s�t���O�Q�b�g
	bool GetComboMissFlag() { return comboMissFlag_; }

	void SetTargetPos(Vec3 pos) { targetPos_ = pos; }

	bool GetHitFlag();

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
	// �ǒ��˕Ԃ�̊p�x�v�Z
	void AngleCalculation();
	// �ǒ��˕Ԃ�̈ʒu���̑���
	void WallRefrectCal();

	void Effect();

	float Cross(Vec2 a, Vec2 b);
	//���X�|�[��
	void Respawn();

	void LoadRespawn();
private:
	struct RespawnPos
	{
		Vec3 pos = {};
	};
private:
	ObjectData pObject;                 // �v���C���[�I�u�W�F�N�g
	Vec3 position = { 5.0f, 0.0f, 0.0f }; // �ʒu
	Vec3 position = {0.0f, 0.0f, -5.0f}; // �ʒu
	Vec3 rotation = {};                 // ��]��

	Vec3 targetPos_ = {};

	// �������Ă��邩
	bool haveFlag_ = false;
	// �����Ă��邩
	bool throwFlag_ = false;
	// �`���[�W���Ă邩
	bool chargeFlag_ = false;
	// �����������ǂ����̃t���O
	bool hitFlag_ = false;
	// �R���{�A�b�v�t���O
	bool comboUpFlag_ = false;
	// �R���{�~�X�t���O
	bool comboMissFlag_ = false;

	// �{�[���̃X�s�[�h
	float speed_ = 0.5f;
	// �o�E���h�̍���
	float baseBound_ = 0.0f;
	// �o�E���h�̍���(�߂���)
	float nearBound_ = 4.0f;
	// �o�E���h�̍���(������)
	float farBound_ = 6.0f;
	// �o�E���h�̎���
	float baseTime_ = 0.05f;
	// �o�E���h�̎���(�߂���)
	float nearTime_ = 0.01625f;
	// �o�E���h�̎���(������)
	float farTime_ = 0.0125f;
	// �C�[�W���O�̃^�C��
	std::array<float, 6> time_ = { 0, 0, 0, 0, 0, 0 };
	// �o�E���h�̒����ς�鋗��
	float boundChangeR = 22.5f;


	// �`���[�W�̑���
	float chargeValue_ = 0.005f;
	// �{�[���̒��˕Ԃ���W
	Vec3 fallPositionCal_ = { 0.0f, 0.0f, 0.0f };
	// ���˕Ԃ����
	Vec3 reflectVector_ = { 0.0f, 0.0f, 0.0f };
	// ���˕Ԃ�̃X�s�[�h�̍ő�l
	float maxReflectSpeed_ = 3.0f;
	// ���˕Ԃ�X�s�[�h
	float baseReflectSpped_ = 0.25f;
	// ���˕Ԃ������MAX���l
	int maxFlyVectorRandom_ = 5;
	// ���˕Ԃ������MIN���l
	int minFlyVectorRandom_ = -5;
	// ���˕Ԃ�����̃����_�����l
	float flyVectorRandom_ = 0.0f;
	// �ǂ̌����m�F�̂��߂̍��W
	Vec3 wallPos = { 0.0f, 0.0f, 0.0f };
	// ������ꂽ�ꏊ�̃|�W�V����
	Vec3 oldThrowPos = { 0.0f, 0.0f, 0.0f };
	//
	float wallRefVec = 0.0f;
	bool refflaga = false;
	float abc = 0;

	int effectCount_ = 0;


	std::vector<RespawnPos*>respawnPos;//�G�̏o���ʒu
	ObjectData respawnObj;


	ObjectData landmarkObj;
	TextureData landmarkTex;
	//�_���[�W�p�[�e�B�N��
	std::unique_ptr<ParticleManager>damegeParticle = nullptr;
	TextureData damegeGraph;
	//�{�[���̏W�����p�[�e�B�N��
	std::unique_ptr<ParticleManager>ballLineParticle = nullptr;
	TextureData ballLineGraph;
};
