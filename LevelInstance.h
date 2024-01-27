#pragma once
#include<fstream>
#include<string>
#include"TextureInstance.h"
#include "Player.h"
class LevelInstance
{
public:
	void SaveLevel(const std::vector<TextureInstance>& ToSave, const std::wstring& Filename, const Player PlayerInstance);
	void LoadLevel(std::vector<TextureInstance>& ToLoad, const std::wstring& Filename, Player& PlayerInstance);


};