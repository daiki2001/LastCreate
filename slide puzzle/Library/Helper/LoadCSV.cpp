#include "LoadCSV.h"
#include <stdio.h>

int LoadCSV(int map[MAP_HEIGHT][MAP_WIDTH], const std::string& FilePath, const int stageWidth, const int  stageHeight)
{
	if (map == nullptr)
	{//なかったら戻す
		return -1;
	}

	for (size_t y = 0; y < stageWidth; y++)
	{
		for (size_t x = 0; x < stageHeight; x++)
		{
			// 初期化
			map[y][x] = 0;
		}
	}

	FILE* fileHandle;
	errno_t err;
	char string[256] = { 0 };

	err = fopen_s(&fileHandle, FilePath.c_str(), "r");
	if (err != 0)
	{
		return err;
	}

	bool isMinus = false;

	for (int y = 0; y < stageHeight;)
	{
		bool end = false;
		fgets(string, 256, fileHandle);
		for (int x = 0, i = 0; x < stageWidth; i++)
		{
			if (string[i] == '\0')
			{
				// 読み込み終了
				fgets(string, 256, fileHandle);
				i = 0;
			}
			else if (string[i] == '\n')
			{
				// 改行
				y++;
				break;
			}
			else if (string[i] == ',')
			{
				// 次の数字へ
				x++;
				isMinus = false;
			}
			else if (string[i] == '-')
			{
				isMinus = true;
			}
			else if (string[i] >= '0' && string[i] <= '9')
			{//桁数が2桁以上の処理
				map[y][x] *= 10;

				if (isMinus == true)
				{
					map[y][x] -= string[i] - '0';
				}
				else
				{
					map[y][x] += string[i] - '0';
				}
			}
		}
		if (end)
		{
			break;
		}
	}
	fclose(fileHandle);

	return 0;
}

int LoadCSV1D(int* mapArray, const size_t& mapSize, const std::string& FilePath, int LoadStopNumber)
{
	for (size_t i = 0; i < mapSize; i++)
	{
		mapArray[i] = 0;
	}

	FILE* fileHandle;
	errno_t err;
	char string[256] = { 0 };
	int index = 0;

	err = fopen_s(&fileHandle, FilePath.c_str(), "r");
	if (err != 0)
	{
		return err;
	}

	bool isMinus = false;

	while (fgets(string, 256, fileHandle) != nullptr)
	{
		bool end = false;

		for (int i = 0; string[i] != '\0'; i++)
		{
			if (string[i] == ',' || string[i] == '\n')
			{
				// 次の数字へ
				index++;
				isMinus = false;
			}
			else if (string[i] == '-')
			{
				isMinus = true;
			}
			else if (string[i] >= '0' && string[i] <= '9')
			{
				mapArray[index] *= 10;

				if (isMinus == true)
				{
					mapArray[index] -= string[i] - '0';
				}
				else
				{
					mapArray[index] += string[i] - '0';
				}

				if (mapArray[index] == LoadStopNumber)
				{
					// ヒットしたら、読み込みを強制的に終了する。
					end = true;
					break;
				}
			}
		}
		if (end)
		{
			break;
		}
	}
	fclose(fileHandle);

	return 0;
}

void LoadSize(int& width, int& height, const std::string& FilePath)
{
	FILE* fileHandle;
	errno_t err;
	char string[256] = { 0 };
	char stringMemory[256] = { 0 };

	err = fopen_s(&fileHandle, FilePath.c_str(), "r");
	if (err != 0)
	{
		return;
	}

	bool isMinus = false;

	int widthMemory = 0, heightMemory = 0;



	int i = 1;
	while (1)
	{
		fgets(string, 256, fileHandle);
		int end = 0;
		for (int j = 0; j < i; j++)
		{
			if (string[j] == stringMemory[j])
			{
				end++;
			}
		}
		if (end == i)
		{
			break;
		}

		for (int j = 0; j < i; j++)
		{
			stringMemory[j] = string[j];
		}
		widthMemory = 0;
		i = 0;
		while (1)
		{
			if (string[i] == '\0')
			{
				// 読み込み終了
				fgets(string, 256, fileHandle);
				i = 0;
			}
			else if (string[i] == '\n')
			{
				// 改行
				heightMemory++;
				break;
			}
			else if (string[i] == ',')
			{
				// 次の数字へ
				isMinus = false;
			}
			else if (string[i] == '-')
			{
				isMinus = true;
			}
			else if (string[i] >= '0' && string[i] <= '9')
			{
				widthMemory++;
			}
			i++;
		}
	}

	fclose(fileHandle);

	width = widthMemory;
	height = heightMemory;
}
