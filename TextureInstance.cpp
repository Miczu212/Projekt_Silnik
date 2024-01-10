#include "TextureInstance.h"

std::filesystem::path TextureInstance::GetPath() const
{
	return Path;
}

void TextureInstance::SetPath(std::filesystem::path path)
{
	this->Path = path;
}



