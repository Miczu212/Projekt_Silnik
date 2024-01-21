#pragma once
#include<fstream>
#include<string>
#include"TextureInstance.h"
class LevelInstance
{
public:
	void SaveLevel(const std::vector<TextureInstance>& ToSave, const std::wstring& Filename);
	void LoadLevel(std::vector<TextureInstance>& ToLoad, const std::wstring& Filename);

};