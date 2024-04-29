#pragma once
#include<fstream>
#include<string>
#include"TextureInstance.h"
#include "Player.h"
#include"Animation.h"
class LevelInstance
{
public:
	void SaveLevel(const std::vector<TextureInstance>& ToSave,const AnimationHolder& AnimHolder, std::vector<std::wstring>& ToSaveA, const std::wstring& Filename, const Player PlayerInstance) const ;
	std::wstring GetNameOfFile(const std::wstring& path) const;
	void ReTargetLevel(const std::wstring& Filename);
	void LoadLevel(std::vector<TextureInstance>& ToLoadT, AnimationHolder& AnimHolder, std::vector<std::wstring>& ToLoadA, const std::wstring& Filename, Player& PlayerInstance);


};