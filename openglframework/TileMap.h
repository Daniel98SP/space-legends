#pragma once

#include <glm/glm.hpp>
#include "Texture.h"
#include "ShaderProgram.h"

// Class Tilemap is capable of loading a tile map from a text file in a very
// simple format (see level01.txt for an example). With this information
// it builds a single VBO that contains all tiles. As a result the render
// method draws the whole map independently of what is visible.

class TileMap
{

public:
	// Tile maps can only be created inside an OpenGL context
	static TileMap *createTileMap(const string &mapFile, const glm::vec2 &minCoords, ShaderProgram &program);

	TileMap(const string &mapFile, const glm::vec2 &minCoords, ShaderProgram &program);
	~TileMap();

	void render() const;
	void free();
	
	int getTileSize() const { return tileSize; }
	glm::ivec2 getMapSizeInTiles() const { return mapSize; }

	bool collisionMoveLeft(const glm::ivec2 &pos, const glm::ivec2 &size, bool enemy = false) const;
	bool collisionMoveRight(const glm::ivec2 &pos, const glm::ivec2 &size, bool enemy = false) const;
	bool collisionMoveDown(const glm::ivec2 &pos, const glm::ivec2 &size, bool enemy = false) const;
	bool collisionMoveUp(const glm::ivec2 &pos, const glm::ivec2 &size, bool enemy = false) const;
	bool collisionOnFall(const glm::ivec2& pos, const glm::ivec2& size, int* posY, bool enemy = false) const;

	
private:
	bool loadLevel(const string& mapFile);
	void prepareArrays(const glm::vec2 &minCoords, ShaderProgram &program);

private:
	GLuint vao;
	GLuint vbo;
	GLint posLocation, textureCoordinateLocation;
	glm::ivec2 position, mapSize, tilesheetSize;
	int tileSize, blockSize;
	Texture tilesheet;
	glm::vec2 tileTexSize;
	int *map;

};


