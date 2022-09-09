#pragma once

#include <stack>
#include "Ship.h"
#include "Base.h"

class Bot {
public:
	static void initClass();
	void init(Ship* ship, Team botTeam, Base* allyPlanet, Base* enemyPlanet, Base* allyStations, Base* enemyStations, Ship* allyShips, Ship* enemyShips);
	void decide(int deltaTime);
private:
	void setNewObjective();
	void calculateShortestPath();
	void calculateDestinationStack();
	glm::vec2 getPosition();
	Ship* getShip();

	struct Node {
		glm::vec2 position;
		int adjacentNodes[4];
	};
	static Node nodes[23];
	float distances[23];

	Ship* ship;
	Team team;
	Base* allyPlanet;
	Base* enemyPlanet;
	Base* allyStations;
	Base* enemyStations;
	Ship* allyShips;
	Ship* enemyShips;

	Base* objective;
	int currentNodeId;
	stack<int> destinations;

	int coins;
};