#pragma once
#include"BaseScene.h"
#include"../slide puzzle/SceneChange.h"
#include"../slide puzzle/Player.h"
#include"../slide puzzle/Ball.h"
#include"../slide puzzle/Stage.h"
#include"../slide puzzle/BaseEnemy.h"
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

private:
	//出現位置のロード
	void LoadSpawnStatus();
	//スポーンエネミー
	void SpawnEnemy();
private:
	std::unique_ptr<LightGroup>lightGroup;
	std::unique_ptr<Audio> audio;
private://定義
	// シーンチェンジ
	std::unique_ptr<SceneChange> sceneChange_;

	std::unique_ptr<Player>player;

	std::unique_ptr<Ball> ball;
	std::unique_ptr<Ball> ball2;

	std::unique_ptr<Stage>stage;

	
	ObjectData object;

	std::vector<Enemy*>enemys;         //敵
	std::vector<LoadStatus*>loadStatus;//敵の出現位置
	int enemyMax = 3;                  //敵最大数
	int spwnCoolTime = 0;              //現在のクールタイム
	int spwnCoolTimeMax = 500;         //クールタイム最大

	int forcusEnemyNum = 0;
	bool forcusChangeFlag = false;
};