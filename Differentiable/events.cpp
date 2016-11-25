#include "head.h"

void eventIsConsoleUp(SDL_Event e, bool &isConsoleUp, bool &inEditMode, std::string &consoleString, std::vector<AABB> &aabbs, std::vector<MovingObject> &movingObjects, Player &player, SDL_Renderer* r)
{
	if (e.type == SDL_KEYDOWN)
	{
		switch (e.key.keysym.sym)
		{
		case SDLK_BACKQUOTE:
		{
			isConsoleUp = !isConsoleUp;
			break;
		}
		case SDLK_BACKSPACE:
		{
			consoleString = consoleString.substr(0, consoleString.length() - 1);
			break;
		}
		case SDLK_RETURN:
		{
			//Parse
			std::queue<std::string> tokens = tokenize(consoleString, separators);
			parseCommand(tokens, aabbs, movingObjects, player, r, inEditMode, separators);
			consoleString = "";
			isConsoleUp = false;
			break;
		}
		default:
		{
			consoleString += e.key.keysym.sym;
			break;
		}

		}
	}
}

void eventKeyDown(SDL_Event e, bool &running, bool &isConsoleUp, bool &debug, std::string &consoleString, Player &player)
{
	switch (e.key.keysym.sym)
	{
	case SDLK_ESCAPE:
	{
		running = false;
		break;
	}
	case SDLK_SPACE:
	{
		if (player.canJump())
		{
			player.jump();
		}
		break;
	}
	case SDLK_LEFT:
	{
		player.accelerateLeft();
		player.setFacing(false);
		break;
	}

	case SDLK_RIGHT:
	{
		player.accelerateRight();
		player.setFacing(true);
		break;
	}

#ifdef _DEBUG
	case SDLK_BACKQUOTE:
	{
		isConsoleUp = !isConsoleUp;
		consoleString = "";
		break;
	}

	case SDLK_d:
	{
		debug = !debug;
		break;
	}
#endif
	}
}

void eventKeyUp(SDL_Event e, Player &player)
{
	switch (e.key.keysym.sym)
	{
	case SDLK_LEFT:
	{
		player.stop();
		break;
	}
	case SDLK_RIGHT:
	{
		player.stop();
		break;
	}
	}
}

void eventJoystick(SDL_Event e, Player &player)
{
	//Motion on controller 0
	if (e.jaxis.which == 0)
	{
		//X axis motion
		if (e.jaxis.axis == 0)
		{
			//Left
			if (e.jaxis.value < -JOYSTICK_DEAD_ZONE)
			{
				player.accelerateLeft();
				player.setFacing(false);
			}
			//Right
			else if (e.jaxis.value > JOYSTICK_DEAD_ZONE)
			{
				player.accelerateRight();
				player.setFacing(true);
			}
			else
			{
				player.stop();
			}
		}
	}
}

void eventButton(SDL_Event e, bool &running, bool &debug, Player &player)
{
	if (e.jbutton.button == XBOX_360_A)
	{
		if (player.canJump())
		{
			player.jump();
		}
	}

	if (e.jbutton.button == XBOX_360_START)
	{
		running = false;
	}

	if (e.jbutton.button == XBOX_360_RB)
	{
		player.zipline();
	}

#ifdef _DEBUG
	if (e.jbutton.button == XBOX_360_SELECT)
	{
		debug = !debug;
	}
#endif

}

void eventMisc(SDL_Event e, bool &running)
{
	switch (e.type)
	{
	case SDL_QUIT:
	{
		running = false;
		break;
	}
	}
}