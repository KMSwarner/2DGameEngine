#include "Map.h"
#include "Game.h"
#include <fstream>

Map::Map()
{
}


Map::~Map()
{
}

void Map::LoadMap(std::string path, int sizeX, int sizeY)
{
	char tileType;
	std::fstream mapFile;
	mapFile.open(path);

	int srcX, srcY;

	for (int y = 0; y < sizeY; y++)
	{
		for(int x = 0; x < sizeX; x++)
		{
			mapFile.get(tileType);
			srcY = atoi(&tileType) * 32;

			mapFile.get(tileType);
			srcX = atoi(&tileType) * 32;

			Game::AddTile(srcX, srcY, x * 64, y * 64);
			mapFile.ignore(); //Ignores ',' mid-line
		}
		//mapFile.ignore(); //Ignores \n at end of line
	}

	mapFile.close();
}