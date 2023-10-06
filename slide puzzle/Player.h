#pragma once
#include"Object.h"
#include"Sprite.h"
#include<ParticleManager.h>
#include<memory>
#include<Audio.h>
#include <array>
#include <algorithm>
#include <functional>
#include "Model.h"
#include "FbxLoader.h"
#include "FBXObject3d.h"

class Player
{
private:
	

public:
	//�R���X�g���N�^
	Player();
	//�f�X�R���X�g���N�^
	~Player();
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
private:
	/// <summary>
	/// �ړ�
	/// </summary>
	void Move();
	/// <summary>
	/// �W�����v
	/// </summary>
	void Jump();
	/// <summary>
	/// ������
	/// </summary>
	void BallThrow();
	/// <summary>
	/// �߂�
	/// </summary>
	void BallCatch();
private:
	std::unique_ptr<Model> m_model;		  //���f��
	std::unique_ptr<FBXObject3d>  m_fbx;//FBX

private:
	ObjectData pObject;                         //�v���C���[�I�u�W�F�N�g
	Vec3 position = {};                         //�ʒu
	Vec3 rotation = {};

	//�ڒn�t���O
	bool onGround_ = true;
	// �����x�N�g��
	Vec3 fallV_ = {};

	// �L���b�`�֌W
	bool catchFlag_ = false;
	int catchTimer_ = 0;
};

