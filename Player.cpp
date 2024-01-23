#include "Player.h"

void Player::PlayerTexture::SetPath(std::filesystem::path path)
{
	this->Path = path;
}

std::filesystem::path Player::PlayerTexture::GetPath() const
{
	return Path;
}

