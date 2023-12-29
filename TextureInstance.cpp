#include "TextureInstance.h"
D2D1_RECT_F TextureInstance::GetRect()
{
	return destinationRect;
}

std::filesystem::path TextureInstance::GetPath()
{
	return Path;
}

void TextureInstance::SetPath(std::filesystem::path path)
{
	this->Path = path;
}

void TextureInstance::SetRect(D2D1_RECT_F rect)
{
	this->destinationRect = rect;
}

