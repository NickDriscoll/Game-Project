#include "head.h"

MovingObject::MovingObject(char* pathToTexture, Vector2 Position, SDL_Renderer* r)
{
	texture = loadTexture(pathToTexture, r);
	position = Position;
}

MovingObject::MovingObject(char* pathToTexture, Vector2 Position, AABB BoundingBox, SDL_Renderer* r)
{
	texture = loadTexture(pathToTexture, r);
	position = Position;
	boundingBox = BoundingBox;
}

SDL_Texture* MovingObject::getTexture()
{
	return texture;
}

Vector2 MovingObject::getPosition()
{
	return position;
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
	position = position.add(velocity.multiply(timeDelta));

	//Ground collision placeholder
	if (position.y > SCREEN_HEIGHT - 64)
	{
		position.y = SCREEN_HEIGHT - 64;
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
		velocity.y += 0.0001;
	}
}