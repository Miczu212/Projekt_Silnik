#pragma once
#include<fstream>
#include<string>
#include"TextureInstance.h"
#include "Player.h"
class LevelInstance
{
public:
	void SaveLevel(const std::vector<TextureInstance>& ToSave, std::vector<std::wstring>& ToSaveA, const std::wstring& Filename, const Player PlayerInstance);
	void LoadLevel(std::vector<TextureInstance>& ToLoadT, std::vector<std::wstring>& ToLoadA, const std::wstring& Filename, Player& PlayerInstance);


};