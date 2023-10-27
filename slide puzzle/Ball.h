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
	void Update();
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

private:
	ObjectData pObject;                         //�v���C���[�I�u�W�F�N�g
	Vec3 position = {};                         //�ʒu
	Vec3 rotation = {};

	// �����҂̈ʒu
	Vec3 havePos_ = {};
	// �^�[�Q�b�g�̈ʒu
	Vec3 targetPos_ = {};

	// �������Ă��邩
	bool haveFlag_ = false;
	// �����Ă��邩
	bool throwFlag_ = false;
};

