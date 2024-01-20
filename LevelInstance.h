#pragma once
#include<fstream>
#include<string>
#include"TextureInstance.h"
class LevelInstance
{
public:
	void SaveLevel(TextureInstance ToSave, std::wstring Filename);
	void LoadLevel(TextureInstance& ToLoad, std::wstring Filename);

};