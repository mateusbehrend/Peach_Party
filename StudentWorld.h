#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_
#include "Actor.h"
#include "GameWorld.h"
#include "Board.h"
#include <string>
#include <vector>
#include <iostream>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetPath);
	virtual int init();
	virtual int move();
	virtual void cleanUp();
	Board getBoard() {
		return m_studentBoard;
	}
	virtual ~StudentWorld() {
		cleanUp();
	}
	Player* getAvatar(int index) {
		return *(avatars + index);
	}
	void setBankCoins(int coins) {
		m_bankCoins = coins;
	}
	int getBankCoins() {
		return m_bankCoins;
	}
	std::vector<Actor*> getActors() {
		return actors;
	}
	void addDroppingSquare(int x, int y);
	void addVortexSquare(int x, int y, int SpriteDirection, int direction);
	void deleteSpecificSquare(int x, int y);
	bool checkSquareForImpact(int x, int y);


private:
	Player* avatars[2];
	std::vector<Actor*> actors;
	Board m_studentBoard;
	bool cleanedUp;
	int m_bankCoins;
	bool initted;
};

#endif // STUDENTWORLD_H_