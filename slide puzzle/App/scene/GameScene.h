#pragma once
#include"BaseScene.h"
#include"../slide puzzle/SceneChange.h"
extern const int window_width;
extern const int window_height;

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene:public BaseScene
{
public:
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
private:
	std::unique_ptr<LightGroup>lightGroup;
	std::unique_ptr<Audio> audio;
private://定義
	// シーンチェンジ
	std::unique_ptr<SceneChange> sceneChange_;
	
	SpriteData back;
	ObjectData object;

	std::unique_ptr<Model> m_model;		  //ペンギンモデル
	std::unique_ptr<FBXObject3d>  m_fbx;//ペンギンFBX
};