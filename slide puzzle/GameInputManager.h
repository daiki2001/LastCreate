#pragma once
#include"Singleton.h"
#include "./BallController/BallController.h"

// 各種入力デバイスの操作をゲーム用にまとめたクラス
// ※仕様から消えた操作も有ります
class GameInputManager : public Singleton<GameInputManager>
{
	friend Singleton<GameInputManager>;
private:
	BallController* ball = nullptr;

public: //メンバ関数
	GameInputManager() = default;
	GameInputManager(const GameInputManager&) = delete;
	~GameInputManager();
	GameInputManager& operator=(const GameInputManager&) = delete;

	void Init();
	void Update();
	void DebugDraw();

	// 決定
	bool IsDecide() const;
	// 前進
	bool IsForward() const;
	// 後進
	bool IsBack() const;
	// 左
	bool IsLeft() const;
	// 右
	bool IsRight() const;
	// ダッシュ
	bool IsDash() const;
	// ジャンプ
	bool IsJamp() const;
	// 投げ
	bool IsThrow() const;
	// キャッチ（中身はIsThrowと同じ）
	bool IsCatch() const;
	// チャージ
	bool IsCharge() const;
};
