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
	boundingBox = AABB(Vector2(), Vector2(textureWidth / 2, textureHeight / 2));
	boundingBoxOffset = Vector2(position.x + textureWidth / 2, position.y + textureHeight / 2);
}

bool Player::isInAir()
{
	return !onGround;
}

void Player::jump()
{
	velocity.y = -300;
}

void Player::accelerateLeft()
{
	velocity.x = -200;
}

void Player::accelerateRight()
{
	velocity.x = 200;
}

void Player::stop()
{
	velocity.x = 0;
}