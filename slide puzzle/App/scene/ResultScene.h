#pragma once
#include"BaseScene.h"
#include"../slide puzzle/SceneChange.h"

/// <summary>
/// ���U���g�V�[��
/// </summary>
class ResultScene:public BaseScene
{
private:

public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	ResultScene();
	/// <summary>
	/// �f�X�R���X�g���N�^
	/// </summary>
	~ResultScene();
	/// <summary>
	/// ���[�v������
	/// </summary>
	void Init()override;
	/// <summary>
	/// �X�V
	/// </summary>
	void Update()override;
	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="stageNum">�X�e�[�W</param>
	void Draw()override;
	/// <summary>
	/// �e�`��
	/// </summary>
	void ShadowDraw()override;

	void Finalize()override;
private:
	std::unique_ptr<LightGroup> lightGroup;
private:
	// �V�[���`�F���W
	std::unique_ptr<SceneChange> sceneChange_;
	ObjectData object;
	SpriteData back;
	SpriteData gameoverGraph;
};