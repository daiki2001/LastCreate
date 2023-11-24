#pragma once
#include"BaseScene.h"
#include"../slide puzzle/SceneChange.h"
extern const int window_width;
extern const int window_height;
/// <summary>
/// タイトルシーン
/// </summary>
class TitleScene :public BaseScene
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	TitleScene();
	/// <summary>
	/// デスコンストラクタ
	/// </summary>
	~TitleScene();

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
	void TitleDirection();
private:
	std::unique_ptr<LightGroup>lightGroup;
private://定義
	// シーンチェンジ
	std::unique_ptr<SceneChange> sceneChange_;
	
	ObjectData object;
	SpriteData button;
	ObjectData stageObj;
	Vec3 sAngle={};
};