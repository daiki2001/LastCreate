#pragma once
#include"Sprite.h"
#include<time.h>
#include<Audio.h>
time_t time(time_t*);
class GameTime
{
public:

	GameTime();

	~GameTime();

	void Init();

	void Update();

	void Draw();

	void Start();

	bool GetChangeFlag() { return sceneChangeFlag; }
private:
	SpriteData number[10];
	SpriteData ten;
	const int gameTimeMax = 180;
	int nowTime = 0;

	time_t start;
	time_t now;

	int differenceTime = 0;

	bool sceneChangeFlag = false;
	SoundData finishSound;
};

