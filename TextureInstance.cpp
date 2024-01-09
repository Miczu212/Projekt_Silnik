#include "TextureInstance.h"

std::filesystem::path TextureInstance::GetPath()
{
	return Path;
}

void TextureInstance::SetPath(std::filesystem::path path)
{
	this->Path = path;
}



