#pragma once
#include"BaseScene.h"
#include"../slide puzzle/SceneChange.h"
#include"../slide puzzle/Player.h"
#include"../slide puzzle/Ball.h"
#include"../slide puzzle/Stage.h"
#include"../slide puzzle/BaseEnemy.h"
#include"Easing.h"
#include"../slide puzzle/GameTime.h"
extern const int window_width;
extern const int window_height;

/// <summary>
/// �Q�[���V�[��
/// </summary>
class GameScene :public BaseScene
{
public:
	struct LoadStatus
	{
		Vec3 position = {};
		Vec3 rotation = {};
	};
	struct RespawnPos
	{
		Vec3 pos = {};
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

	void NearEnemyCheck();

	void EnemyDeath();

	void TargetAct();

	void TargetReset(Vec3 pos, bool flag);

	void BallDelete();

	void BallCreate(const Vec3& pos);

private:
	//�o���ʒu�̃��[�h
	void LoadSpawnStatus();
	//�X�|�[���G�l�~�[
	void SpawnEnemy();
	//�{�[�����X�|�[��
	void BallRespawn();
	//�{�[���̃X�|�[��������W
	void LoadRespawn();
private:
	std::unique_ptr<LightGroup>lightGroup;
	std::unique_ptr<Audio> audio;
private://��`
	// �V�[���`�F���W
	std::unique_ptr<SceneChange> sceneChange_;

	std::unique_ptr<Player>player;

	std::vector<std::unique_ptr<Ball>> balls;

	std::unique_ptr<Stage>stage;

	std::unique_ptr<EaseData> targetEase_;
	bool targetFlag_ = false;

	ObjectData object;

	std::vector<std::unique_ptr<Enemy>>enemys;         //�G
	std::vector<LoadStatus*>loadStatus;//�G�̏o���ʒu
	int enemyMax = 3;                  //�G�ő吔
	int spwnCoolTime = 0;              //���݂̃N�[���^�C��
	int spwnCoolTimeMax = 500;         //�N�[���^�C���ő�

	int forcusEnemyNum = 0;
	bool forcusChangeFlag = false;

	GameTime gameTime;

	std::vector<RespawnPos*>respawnPos;
	ObjectData respawnObj;
};