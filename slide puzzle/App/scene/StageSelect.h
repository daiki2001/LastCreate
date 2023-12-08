#pragma once
#include"BaseScene.h"
#include"../slide puzzle/SceneChange.h"
extern const int window_width;
extern const int window_height;
/// <summary>
/// �X�e�[�W�Z���N�g
/// </summary>
class StageSelect:public BaseScene
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	StageSelect();
	/// <summary>
	/// �f�X�R���X�g���N�^
	/// </summary>
	~StageSelect();
	/// <summary>
	/// ���������[�v
	/// </summary>
	void Init()override;
	/// <summary>
	/// �X�V
	/// </summary>
	void Update()override;
	/// <summary>
	/// �`��
	/// </summary>
	void Draw()override;
	/// <summary>
	/// �e�̕`��
	/// </summary>
	void ShadowDraw()override;

	void Finalize()override;
public:
	std::unique_ptr<LightGroup>lightGroup;
private://��`
	SpriteData back;
	SpriteData manipulate;
	ObjectData object;
	// �V�[���`�F���W
	std::unique_ptr<SceneChange> sceneChange_;
};