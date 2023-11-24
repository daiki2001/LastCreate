#pragma once
#include"BaseScene.h"
#include"../slide puzzle/SceneChange.h"
#include"../slide puzzle/Player.h"
#include"../slide puzzle/Ball.h"
#include"../slide puzzle/Stage.h"
extern const int window_width;
extern const int window_height;

/// <summary>
/// ゲームシーン
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
private:
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
	
	SpriteData back;
	ObjectData object;
};