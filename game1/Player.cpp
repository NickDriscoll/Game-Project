#include "head.h"

Player::Player()
{
}

Player::Player(char* path, Vector2 Position, SDL_Renderer* r, bool FacingRight)
{
	texture = loadTexture(path, r);
	SDL_QueryTexture(texture, NULL, NULL, &textureWidth, &textureHeight);
	position = Position;
	facingRight = FacingRight;
}

bool Player::isInAir()
{
	return !onGround;
}

void Player::jump()
{
	velocity.y = -5;
}

void Player::accelerateLeft()
{
	velocity.x = -0.5;
}

void Player::accelerateRight()
{
	velocity.x = 0.5;
}

void Player::stop()
{
	velocity.x = 0;
}