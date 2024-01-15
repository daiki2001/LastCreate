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
/// ゲームシーン
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
	/// コンストラクタ
	/// </summary>
	GameScene();
	/// <summary>
	/// デスコンストラクタ
	/// </summary>
	~GameScene();
	void Init()override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;
	/// <summary>
	/// 描画
	/// </summary>
	void Draw()override;
	/// <summary>
	/// 影描画
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
	//出現位置のロード
	void LoadSpawnStatus();
	//スポーンエネミー
	void SpawnEnemy();
	//ボールリスポーン
	void BallRespawn();
	//ボールのスポーンする座標
	void LoadRespawn();
private:
	std::unique_ptr<LightGroup>lightGroup;
	std::unique_ptr<Audio> audio;
private://定義
	// シーンチェンジ
	std::unique_ptr<SceneChange> sceneChange_;

	std::unique_ptr<Player>player;

	std::vector<std::unique_ptr<Ball>> balls;

	std::unique_ptr<Stage>stage;

	std::unique_ptr<EaseData> targetEase_;
	bool targetFlag_ = false;

	ObjectData object;

	std::vector<std::unique_ptr<Enemy>>enemys;         //敵
	std::vector<LoadStatus*>loadStatus;//敵の出現位置
	int enemyMax = 3;                  //敵最大数
	int spwnCoolTime = 0;              //現在のクールタイム
	int spwnCoolTimeMax = 500;         //クールタイム最大

	int forcusEnemyNum = 0;
	bool forcusChangeFlag = false;

	GameTime gameTime;

	std::vector<RespawnPos*>respawnPos;
	ObjectData respawnObj;
};