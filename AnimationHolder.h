#pragma once
#include"Animation.h"
class AnimationHolder
{
private: 
public:
	std::vector<std::vector<TextureInstance>> AnimationFrames;
	static AnimationHolder& Get() noexcept
	{
		static AnimationHolder instance;
		return instance;
	}
};