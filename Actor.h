#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include <string>
#include <vector>

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
class StudentWorld;

class Actor : public GraphObject
{
public:
	Actor(StudentWorld* world, int imageID, int startX, int startY, int startDirection, int depth) :GraphObject(imageID, startX, startY, startDirection, depth), m_world(world) {
		//m_centralBankVal = 0;
		m_aliveStatus = true;
		m_canBeDeleted = true;
		m_isImpactable = false;
		m_isImpacted = false;
	};
	virtual void doSomething() = 0;
	StudentWorld* getWorld() {
		return m_world;
	}
	void setAliveStatus(bool status) {
		m_aliveStatus = status;
	}
	bool getAliveStatus() {
		return m_aliveStatus;
	}
	void setCanBeDeleted(bool canBeDeleted) {
		m_canBeDeleted = canBeDeleted;
	}
	bool getCanBeDeleted() {
		return m_canBeDeleted;
	}
	void setIsImpactable(bool isImpactable) {
		m_isImpactable = isImpactable;
	}
	bool getIsImpactable() {
		return m_isImpactable;
	}
	void setImpacted(bool isImpacted) {
		m_isImpacted = isImpacted;
	}
	bool getIsImpacted() {
		return m_isImpacted;
	}
	void teleport();
private:
	StudentWorld* m_world;
	//int m_centralBankVal;
	bool m_aliveStatus;
	bool m_canBeDeleted;
	bool m_isImpactable;
	bool m_isImpacted;
};

class Player : public Actor {
public:
	Player(StudentWorld* world, int imageID, int xpos, int ypos, int playerNum) : Actor(world, imageID, xpos, ypos, 0, 0) {
		m_coins = 0;
		m_stars = 0;
		vortex = false;
		m_playerNum = 2;
		die_roll = 0;
		spriteDirection = right;
		ticks_to_move = 0; //ticks_to_move set  to 0 
		status = "waiting"; //starts out in the waiting to roll state
		m_firstAttempt = true;
		m_directionCounter = 0;
		setPlayerNum(playerNum);
		m_buffer = 0;
		m_xVal = 0;
		m_atFork = false;
		at_fork_waiting_for_roll = false;
	}
	void setFirstAttempt(bool attempt) {
		m_firstAttempt = attempt;
	}
	bool getFirstAttempt() {
		return m_firstAttempt;
	}
	void doSomething();
	void setStatus(std::string newStatus) {
		status = newStatus;
	}
	std::string getStatus() {
		return status;
	}
	void setPlayerNum(int num) {
		m_playerNum = num;
	}
	int getPlayerNum() {
		return m_playerNum;
	}
	void setCoins(int coins) {
		m_coins = coins;
	}
	int getCoins() {
		return m_coins;
	}
	int getRoll() {
		return die_roll;
	}
	int getStars() {
		return m_stars;
	}
	void setStars(int stars) {
		m_stars = stars;
	}
	void setPlayerDirection(int direction) {
		spriteDirection = direction;
	}
	int getPlayerDirection() {
		return spriteDirection;
	}
	void setBuffer(int val) {
		m_buffer = val;
	}
	int getBuffer() {
		return m_buffer;
	}
	void setTicks(int ticks) {
		ticks_to_move = ticks;
	}
	int getTicks() {
		return ticks_to_move;
	}
	void setAtFork(bool fork) {
		m_atFork = fork;
	}
	void setVortex() {
		vortex = true;
	}
	void clearVortex() {
		vortex = false;
	}
	bool hasVortex() {
		if (vortex)
			return true;
		else
			return false;
	}
private:
	int m_coins;
	int m_stars;
	bool vortex;
	int m_playerNum;
	int die_roll;
	int ticks_to_move;
	std::string status;
	int spriteDirection;
	bool m_firstAttempt;
	int m_directionCounter;
	int m_buffer;
	int m_xVal;
	bool m_atFork;
	bool at_fork_waiting_for_roll;
};

//Yoshi and Peach will be constructed from the player
class Yoshi : public Player {
public:
	Yoshi(StudentWorld* world, int imageID, int xpos, int ypos) :Player(world, imageID, xpos, ypos, 0) {
		setPlayerNum(2);
	}
};

class Peach : public Player {
public:
	Peach(StudentWorld* world, int ID, int xpos, int ypos) :Player(world, ID, xpos, ypos, 0) {
		setPlayerNum(1);
	}
};

//BASE
//base class for Squares
class Square : public Actor {
public:
	Square(StudentWorld* world, int ID, int xpos, int ypos) : Actor(world, ID, xpos, ypos, 0, 1) {}
	virtual void doSomething() = 0; // pure virtual so you can't actually call a creation to square
};

class coinSquare : public Square {
public:
	coinSquare(StudentWorld* world, int ID, int xpos, int ypos, int increment) : Square(world, ID, xpos, ypos) {
		m_coinIncrement = increment;
	}
	void doSomething();
private:
	int m_coinIncrement; //distinctive feature of coin square
};

class starSquare : public Square {
public:
	starSquare(StudentWorld* world, int ID, int xpos, int ypos) : Square(world, ID, xpos, ypos) {}
	void doSomething();
private:
};

//has yet to be tested current implementation cannot test that
class directionalSquare : public Square {
public:
	directionalSquare(StudentWorld* world, int ID, int xpos, int ypos, int direction) : Square(world, ID, xpos, ypos) {
		//init the direction
		setDirection(direction);
		m_direction = direction;
	}
	void doSomething();
private:
	int m_direction;
};

class bankSquare : public Square {
public:
	bankSquare(StudentWorld* world, int ID, int xpos, int ypos) : Square(world, ID, xpos, ypos) {}
	void doSomething();
};

class eventSquare : public Square {
public:
	eventSquare(StudentWorld* world, int ID, int xpos, int ypos) : Square(world, ID, xpos, ypos) {}
	void doSomething();
};

class droppingSquare : public Square {
public:
	droppingSquare(StudentWorld* world, int ID, int xpos, int ypos) : Square(world, ID, xpos, ypos) {}
	virtual void doSomething();
};


class baddies : public Actor {
public:
	baddies(StudentWorld* world, int ID, int xpos, int ypos, std::string type) : Actor(world, ID, xpos, ypos, right, 0) {
		setIsImpactable(true);
		setCanBeDeleted(false);
		m_state = "paused";
		m_pauseCounter = 180;
		m_squares_to_move = 0;
		m_ticks_to_move = 0;
		m_first = true;
		m_type = type;
	}
	void setState(std::string state) {
		m_state = state;
	}
	std::string getState() {
		return m_state;
	}
	void setPauseCounter(int pCounter) {
		m_pauseCounter = pCounter;
	}
	int getPauseCounter() {
		return m_pauseCounter;
	}
	void doSomething();
	void setSquaresToMove(int squares) {
		m_squares_to_move = squares;
	}
	int getSquaresToMove() {
		return m_squares_to_move;
	}
	void setTicksToMove(int ticks) {
		m_ticks_to_move = ticks;
	}
	int getTicksToMove() {
		return m_ticks_to_move;
	}
	void setSpriteDirection(int direction) {
		m_spriteDirection = direction;
	}
	int getSpriteDirection() {
		return m_spriteDirection;
	}
	void setDirectionCounter(int counter) {
		m_directionCounter = counter;
	}
	int getDirectionCounter() {
		return m_directionCounter;
	}
	void setBaddieFirst(bool first) {
		m_first = first;
	}
	bool getBaddieFirst() {
		return m_first;
	}
private:
	std::string m_state;
	int m_pauseCounter;
	int m_squares_to_move;
	int m_ticks_to_move;
	int m_spriteDirection;
	int m_directionCounter;
	bool m_first;
	std::string m_type;
};

class vortexSquare : public Actor {
public:
	vortexSquare(StudentWorld* world, int ID, int xpos, int ypos, int sprite_direction, int direction) : Actor(world, ID, xpos, ypos, direction, 0) {
		setAliveStatus(true);
		setDirection(direction);
		spriteDirection = sprite_direction;
	}
	void doSomething();
private:
	int spriteDirection;
};

#endif
