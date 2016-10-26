#include "head.h"

MovingObject::MovingObject()
{
	texture = NULL;
}

MovingObject::MovingObject(char* pathToTexture, Vector2 Position, SDL_Renderer* r, AABB box, bool FacingRight)
{
	texture = loadTexture(pathToTexture, r);
	SDL_QueryTexture(texture, NULL, NULL, &textureWidth, &textureHeight);
	position = Position;
	facingRight = FacingRight;
}

SDL_Texture* MovingObject::getTexture()
{
	return texture;
}

Vector2 MovingObject::getPosition()
{
	return position;
}

int MovingObject::getTextureWidth()
{
	return textureWidth;
}

int MovingObject::getTextureHeight()
{
	return textureHeight;
}

bool MovingObject::getFacing()
{
	return facingRight;
}

void MovingObject::setFacing(bool facing)
{
	facingRight = facing;
}

void MovingObject::UpdatePhysics(double timeDelta)
{
	//Store previous values in respective variables.
	lastPosition = position;
	lastVelocity = velocity;
	pushedRightWall = pushesRightWall;
	pushedLeftWall = pushesLeftWall;
	wasOnGround = onGround;
	wasAtCeiling = atCeiling;

	//Update position
	position = position + (velocity * timeDelta);

	//Ground collision placeholder
	if (position.y > SCREEN_HEIGHT - textureHeight)
	{
		position.y = SCREEN_HEIGHT - textureHeight;
		onGround = true;
	}
	else
	{
		onGround = false;
	}

	//Update bounding boxes position
	boundingBox.setCenter(position.add(aabbOffset));

	//Apply gravity if not on ground
	if (!onGround && velocity.y < TERMINAL_VELOCITY)
	{
		velocity.y += ACCELERATION_DUE_TO_GRAVITY;
	}
}

void MovingObject::draw(SDL_Renderer* r)
{
	SDL_Rect objectRect = { (int)round(position.x) , (int)round(position.y), this->getTextureWidth(), this->getTextureHeight() };
	if (facingRight)
		SDL_RenderCopyEx(r, texture, NULL, &objectRect, 0, NULL, SDL_FLIP_NONE);
	else
		SDL_RenderCopyEx(r, texture, NULL, &objectRect, 0, NULL, SDL_FLIP_HORIZONTAL);
}