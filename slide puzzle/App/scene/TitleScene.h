#pragma once
#include"BaseScene.h"
#include"../slide puzzle/SceneChange.h"
extern const int window_width;
extern const int window_height;
/// <summary>
/// �^�C�g���V�[��
/// </summary>
class TitleScene :public BaseScene
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	TitleScene();
	/// <summary>
	/// �f�X�R���X�g���N�^
	/// </summary>
	~TitleScene();

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
	/// �e�`��
	/// </summary>
	void ShadowDraw()override;

	void Finalize()override;
private:
	std::unique_ptr<LightGroup>lightGroup;
private://��`
	// �V�[���`�F���W
	std::unique_ptr<SceneChange> sceneChange_;
	
	ObjectData object;
	SpriteData back;

	ObjectData floor;
};