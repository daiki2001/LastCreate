#pragma once
#include"Sprite.h"

class Score :public Singleton<Score>
{
public:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	friend Singleton<Score>;
public:

	Score();

	~Score();

	void Init();

	void ScoreReset();

	void Update();

	void PlasScore();

	void GameSceneDraw();

	void ResultSceneDraw();

	void RankingLoad();

	void RankingWrite();

private:
	int score = 0;
	SpriteData number[10];
	SpriteData scoreBack;
	int topScore[3] = {0, 0, 0};
	bool loadFlag = true;
};

