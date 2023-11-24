#pragma once
#include"BaseScene.h"
#include"../slide puzzle/SceneChange.h"
#include"../slide puzzle/Player.h"
#include"../slide puzzle/Ball.h"
#include"../slide puzzle/Stage.h"
extern const int window_width;
extern const int window_height;

/// <summary>
/// �Q�[���V�[��
/// </summary>
class GameScene:public BaseScene
{
public:
	struct LoadStatus
	{
		Vec3 position = {};
		Vec3 rotation = {};
	};
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	GameScene();
	/// <summary>
	/// �f�X�R���X�g���N�^
	/// </summary>
	~GameScene();
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

	void BallHave();

	void CameraMove();
private:
	//�X�|�[���G�l�~�[
	void SpawnEnemy();
private:
	std::unique_ptr<LightGroup>lightGroup;
	std::unique_ptr<Audio> audio;
private://��`
	// �V�[���`�F���W
	std::unique_ptr<SceneChange> sceneChange_;

	std::unique_ptr<Player>player;

	std::unique_ptr<Ball> ball;
	std::unique_ptr<Ball> ball2;

	std::unique_ptr<Stage>stage;
	
	SpriteData back;
	ObjectData object;
};