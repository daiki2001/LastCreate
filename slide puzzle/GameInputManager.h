#pragma once
#include"Singleton.h"

// 各種入力デバイスの操作をゲーム用にまとめたクラス
// ※仕様から消えた操作も有ります
class GameInputManager : public Singleton<GameInputManager>
{
	friend Singleton<GameInputManager>;
public: //メンバ関数
	GameInputManager() = default;
	GameInputManager(const GameInputManager&) = delete;
	~GameInputManager() = default;
	GameInputManager& operator=(const GameInputManager&) = delete;

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
};
