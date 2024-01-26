#pragma once
#include"BaseScene.h"
#include"../slide puzzle/SceneChange.h"
#include<Audio.h>
/// <summary>
/// リザルトシーン
/// </summary>
class ResultScene:public BaseScene
{
private:

public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	ResultScene();
	/// <summary>
	/// デスコンストラクタ
	/// </summary>
	~ResultScene();
	/// <summary>
	/// ループ初期化
	/// </summary>
	void Init()override;
	/// <summary>
	/// 更新
	/// </summary>
	void Update()override;
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="stageNum">ステージ</param>
	void Draw()override;
	/// <summary>
	/// 影描画
	/// </summary>
	void ShadowDraw()override;

	void Finalize()override;
private:
	std::unique_ptr<LightGroup> lightGroup;
private:
	SpriteData back;
	SpriteData manipulate;
	ObjectData object;
	ObjectData stageObj;
	// シーンチェンジ
	std::unique_ptr<SceneChange> sceneChange_;

	SoundData decideSound;
	bool soundFlag = false;
};