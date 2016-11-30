#include "head.h"

void loadLevel(char* path, std::vector<Tile> &tiles, std::vector<MovingObject> &movingObjects, Player &player, SDL_Renderer* r, const char separators[])
{
	//Clear the incoming vectors, this implicitly un-loads the current level.
	tiles.clear();
	movingObjects.clear();

	//Open file
	std::fstream fs;
	fs.open(path, std::ios::in);

	std::queue<std::string> tokens = tokenize(fs, separators);
	fs.close();

	parselevel(tokens, tiles, movingObjects, player, r);
}

bool contains(char a, const char arr[])
{
	for (int i = 0; arr[i] != '\0'; i++)
	{
		if (a == arr[i])
			return true;
	}
	return false;
}

std::string getNextWord(std::string string, int &position, const char separators[])
{
	int beginning = position;

	//Ensure the index is pointing to a non-separator
	while (contains(string[beginning], separators))
	{
		beginning++;
	}

	int end = beginning;

	//Increment end until we hit another separator or the end of the line
	while (string[end] != '\0' && !contains(string[end], separators))
	{
		end++;
	}

	position = end;
	return string.substr(beginning, end);

}

std::queue<std::string> tokenize(std::fstream &in, const char separators[])
{
	std::queue<std::string> tokens;

	while (!in.eof())
	{
		int currentPosition = 0;
		std::string currentToken;
		in >> currentToken;

		if (currentToken.compare("!") == 0)
		{
			in >> currentToken;
			while (currentToken.compare("!") != 0)
			{
				in >> currentToken;
				if (in.eof())
				{
					printf("Expected matching \"!\"\n\n");
					exit(0);
				}
			}
			in >> currentToken;
		}
		tokens.push(currentToken);
	}

	return tokens;
}

std::queue<std::string> tokenize(std::string &in, const char separators[])
{
	std::queue<std::string> tokens;
	int currentPosition = 0;

	while (currentPosition < (int)in.length())
	{
		tokens.push(getNextWord(in, currentPosition, separators));
	}

	return tokens;

}


void parseCommand(std::queue<std::string> &tokens, std::vector<Tile> &tiles, std::vector<MovingObject> &movingObjects, Player &player, SDL_Renderer *r, bool &inEditMode, std::string &editorString, const char separators[])
{
	//Handle empty string
	if (tokens.size() != 0)
	{

		if (tokens.front().compare("load") == 0 && tokens.size() == 2)
		{
			tokens.pop();

			//Stupid bullshit
			std::string fullPath = ("levels\\" + tokens.front()).c_str();
			char* path = new char[fullPath.length()];
			strcpy(path, fullPath.c_str());

			if (fileExists(path))
			{
				loadLevel(path, tiles, movingObjects, player, r, separators);
			}
		}
		else if (tokens.front().compare("edit") == 0)
		{
			inEditMode = !inEditMode;
			editorString = "Currently selected tile is: ";
		}
		else
		{
			if (tokens.front().compare("load") != 0)
			{
				printf("Unrecognized command: \"%s\"\n\n", tokens.front().c_str());
			}
		}

		tokens.pop();
	}
}

bool fileExists(char* path)
{
	std::fstream fs;
	fs.open(path, std::ios::in);
	return (bool)fs;
}

void parselevel(std::queue<std::string> &tokens, std::vector<Tile> &tiles, std::vector<MovingObject> &movingObjects, Player &player, SDL_Renderer *r)
{
	//Throw away <level>
	tokens.pop();

	while (tokens.front().compare("</level>") != 0)
	{
		if (tokens.front().compare("<Tile>") == 0)
		{
			parseTile(tokens, tiles, r);
		}
		else if (tokens.front().compare("<MovingObject>") == 0)
		{
			parseMovingObject(tokens, movingObjects, r);
		}
		else if (tokens.front().compare("<Player>") == 0)
		{
			parsePlayer(tokens, player, r);
		}
		else
		{
			printf("Error parsing .lvl file\n\nInvalid token: %s\n\n", tokens.front().c_str());
			exit(0);
		}
	}
}

void parseTile(std::queue<std::string> &tokens, std::vector<Tile> &tiles, SDL_Renderer* r)
{
	Vector2 origin;
	int textureIndex;

	//Throw away <AABB>
	tokens.pop();

	//Get xpos
	origin.x = std::stod(tokens.front());
	tokens.pop();

	//Get ypos
	origin.y = std::stod(tokens.front());
	tokens.pop();

	//Get texture index
	textureIndex = std::stod(tokens.front());
	tokens.pop();

	//Throw away </Tile>
	if (tokens.front().compare("</Tile>") != 0)
	{
		printf("Expected \"</Tile>\" but was \"%s\"\n\n", tokens.front().c_str());
		exit(0);
	}
	tokens.pop();

	//Done
	tiles.push_back(Tile(textureIndex, origin, r));
}

void parseMovingObject(std::queue<std::string> &tokens, std::vector<MovingObject> &movingObjects, SDL_Renderer* r)
{
	//Properties
	std::string pathtoTexture;
	Vector2 position;
	bool facingRight;


	//Throw away <MovingObject>
	tokens.pop();

	//Get position
	position.x = std::stod(tokens.front());
	tokens.pop();

	position.y = std::stod(tokens.front());
	tokens.pop();

	//Get texture path
	pathtoTexture = tokens.front();
	tokens.pop();

	//Get facingRight
	if (std::stoi(tokens.front()) == 1)
	{
		facingRight = true;
	}
	else
	{
		facingRight = false;
	}
	tokens.pop();

	//Throw away </MovingObject>
	if (tokens.front().compare("</MovingObject>") != 0)
	{
		printf("Expected \"</MovingObject>\" but was \"%s\"\n\n", tokens.front().c_str());
		exit(0);
	}
	tokens.pop();

	//Stupid bullshit
	char* pathToTextureChar = new char[pathtoTexture.length()];
	strcpy(pathToTextureChar, pathtoTexture.c_str());


	//Done
	movingObjects.push_back(MovingObject(pathToTextureChar, position, r, facingRight));

}

void parsePlayer(std::queue<std::string> &tokens, Player &player, SDL_Renderer *r)
{
	//Properties
	std::string pathtoTexture;
	Vector2 position;
	bool facingRight;


	//Throw away <Player>
	tokens.pop();

	//Get position
	position.x = std::stod(tokens.front());
	tokens.pop();

	position.y = std::stod(tokens.front());
	tokens.pop();

	//Get texture path
	pathtoTexture = tokens.front();
	tokens.pop();

	//Get facingRight
	if (std::stoi(tokens.front()) == 1)
	{
		facingRight = true;
	}
	else
	{
		facingRight = false;
	}
	tokens.pop();

	//Throw away </Player>
	if (tokens.front().compare("</Player>") != 0)
	{
		printf("Expected \"</Player>\" but was \"%s\"\n\n", tokens.front().c_str());
		exit(0);
	}
	tokens.pop();

	//Stupid bullshit
	char* pathToTextureChar = new char[pathtoTexture.length()];
	strcpy(pathToTextureChar, pathtoTexture.c_str());

	//Done
	player = Player(pathToTextureChar, position, r, facingRight);
}
