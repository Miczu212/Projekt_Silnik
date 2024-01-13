#pragma once
#include<fstream>
#include<string>
#include"TextureInstance.h"
class LevelInstance
{
public:
	void SaveLevel(TextureInstance ToSave, std::string Filename);
	void LoadLevel(TextureInstance& ToLoad, std::string Filename);

};