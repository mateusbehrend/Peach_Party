#include "Actor.h"
#include "StudentWorld.h"
#include "Board.h"
#include "GameWorld.h"
#include "GameConstants.h"
#include <string>
#include <vector>
#include <iostream>

//helper functions
// check for exact same position intersection


// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp
void Player::doSomething()
{ //peach inherited from Yoshi, so it has same doSomething() function
	m_directionCounter = 0;
	//checking for the number of valid directions
	// add mod 16 checks so we don't do the checks when we are between squares.
	if (((getX() % 16) == 0) && ((getY() % 16) == 0)) {
		if ((getY() != 0) && getWorld()->getBoard().getContentsOf((getX() / 16), (getY() / 16) - 1) != 0)
			m_directionCounter++;
		if ((getX() != 0) && getWorld()->getBoard().getContentsOf(((getX() / 16) - 1), (getY() / 16)) != 0)
			m_directionCounter++;
		if ((getY() != 240) && getWorld()->getBoard().getContentsOf((getX() / 16), (getY() / 16) + 1) != 0)
			m_directionCounter++;
		if ((getX() != 240) && getWorld()->getBoard().getContentsOf(((getX() / 16) + 1), (getY() / 16)) != 0)
			m_directionCounter++;
	}

	//condition for when there is more than two possibilities of direction to move
	//changes status to waiting 
	// and Fork conditon to true
	// if it is a direction square then it is not a fork.  Also need to make sure
	// we are in the "walking" state before we got to waiting.
	if ((m_directionCounter > 2) && (m_atFork == false) && at_fork_waiting_for_roll == false) {
		if ((getWorld()->getBoard().getContentsOf((getX() / 16), (getY() / 16)) !=
			getWorld()->getBoard().right_dir_square) &&
			(getWorld()->getBoard().getContentsOf((getX() / 16), (getY() / 16)) !=
				getWorld()->getBoard().left_dir_square) &&
			(getWorld()->getBoard().getContentsOf((getX() / 16), (getY() / 16)) !=
				getWorld()->getBoard().up_dir_square) &&
			(getWorld()->getBoard().getContentsOf((getX() / 16), (getY() / 16)) !=
				getWorld()->getBoard().down_dir_square)) {
			status = "waiting";
			m_atFork = true;
			if (!ticks_to_move)
				at_fork_waiting_for_roll = true;
		}
	}
	//if status is waiting
	if (status == "waiting") {
		//get player's action
		int command = getWorld()->getAction(getPlayerNum());
		switch (command)
		{
		case ACTION_ROLL:
			//if person not at a fork
			if ((m_atFork == false) || (at_fork_waiting_for_roll == true)) {
				//Person rolls the die to change the position
				die_roll = randInt(1, 10);
				ticks_to_move = die_roll * 8;
				// Don't start walking if don't know the direction yet.
				if (m_atFork == false)
					status = "walking";
				if (at_fork_waiting_for_roll == true)
					at_fork_waiting_for_roll = false;
			}
			break;
		case ACTION_LEFT:
			if (spriteDirection != right && m_atFork == true) {
				if (ticks_to_move)
					status = "walking";
				m_atFork = false;
				spriteDirection = left;
				m_directionCounter = 0;
				setDirection(left);
			}
			break;
		case ACTION_RIGHT:
			if (spriteDirection != left && m_atFork == true) {
				if (ticks_to_move)
					status = "walking";
				m_atFork = false;
				spriteDirection = right;
				m_directionCounter = 0;
				setDirection(right);
			}
			break;
		case ACTION_UP:
			if (spriteDirection != down && m_atFork == true) {
				if (ticks_to_move)
					status = "walking";
				m_atFork = false;
				spriteDirection = up;
				m_directionCounter = 0;
				setDirection(right);
			}
			break;
		case ACTION_DOWN:
			if (spriteDirection != up && m_atFork == true) {
				if (ticks_to_move) {
					status = "walking";
					m_atFork = false;
				}
				//change from up to down
				spriteDirection = down;
				m_directionCounter = 0;
				setDirection(right);
			}
			break;
			// add Vortex fire.
		case ACTION_FIRE:
			if (hasVortex()) {
				//call secondary function in Student World to add a vortex
				getWorld()->addVortexSquare(getX(), getY(), spriteDirection, getDirection());
				//clear vortex, becuase it has been fired
				clearVortex();
				getWorld()->playSound(SOUND_PLAYER_FIRE);
			}
			break;
		default:
			return;
		}
	}
	if (status == "walking") {
		m_buffer = 0;
		m_directionCounter = 0;
		setFirstAttempt(false);


		//right is not valid
		if (spriteDirection == right) {
			if (getX() == 240 || getWorld()->getBoard().getContentsOf(((getX()) / 16) + 1, (getY()) / 16) == 0) {
				if (getWorld()->getBoard().getContentsOf(((getX()) / 16), ((getY()) / 16) + 1) == 0) {
					spriteDirection = down;
					setDirection(right);
				}
				else {
					spriteDirection = up;
					setDirection(right);
				}
			}

		}

		//left is not valid
		if (spriteDirection == left) {
			if (getX() == 0 || getWorld()->getBoard().getContentsOf(((getX() + 15) / 16) - 1, (getY() + 15) / 16) == 0) {
				//checking for only empty squares so it should theoretically work on all kinds of squares
				if (getWorld()->getBoard().getContentsOf(((getX() + 15) / 16), ((getY()) / 16) + 1) == 0) {
					spriteDirection = down;
					setDirection(right);

				}
				else {
					spriteDirection = up;
					setDirection(right);
				}
			}

		}

		//up is not valid 
		if (spriteDirection == up) {
			if (getY() == 240 || getWorld()->getBoard().getContentsOf(((getX() + 15) / 16), ((getY()) / 16) + 1) == 0) {
				if (getWorld()->getBoard().getContentsOf(((getX()) / 16) + 1, (getY() + 15) / 16) == 0) {
					spriteDirection = left;
					setDirection(left);

				}
				else {
					spriteDirection = right;
					setDirection(right);
				}
			}

		}

		//down is not valid 
		if (spriteDirection == down) {
			if (getY() == 0 || getWorld()->getBoard().getContentsOf(((getX() + 15) / 16), ((getY() + 15) / 16) - 1) == 0) {
				if (getWorld()->getBoard().getContentsOf(((getX()) / 16) + 1, (getY() + 15) / 16) == 0) {
					spriteDirection = left;
					setDirection(left);

				}
				else {
					spriteDirection = right;
					setDirection(right);
				}
			}
		}

		if (status == "walking") {
			moveAtAngle(spriteDirection, 2);
			ticks_to_move--;
			if (ticks_to_move == 0) {
				status = "waiting";
				setFirstAttempt(true);
			}
		}
	}
}

void Actor::teleport()
{
	int random = randInt(0, getWorld()->getActors().size() - 1);
	//assumes when teleported it does cause action on the square it lands on 
	moveTo(getWorld()->getActors()[random]->getX(), getWorld()->getActors()[random]->getY());
}

void coinSquare::doSomething()
{
	for (int i = 0; i < 2; i++) {
		if (getAliveStatus() == true) {
			if (getWorld()->getAvatar(i)->getX() == getX() && getWorld()->getAvatar(i)->getY() == getY()) {
				//checks for if the redsqaure would make the count count negative which can't happen
				if (getWorld()->getAvatar(i)->getFirstAttempt() == true) {
					//check to see if the position of this particular sqaure is equal to yoshi's coords
					if (m_coinIncrement == -3 && getWorld()->getAvatar(i)->getCoins() < 3) {
						getWorld()->getAvatar(i)->setCoins(0);
						getWorld()->getAvatar(i)->setFirstAttempt(false);
					}
					else {
						getWorld()->getAvatar(i)->setCoins(getWorld()->getAvatar(i)->getCoins() + m_coinIncrement); // increment players's coins by 3
						getWorld()->getAvatar(i)->setFirstAttempt(false);
					}
					if (m_coinIncrement == 3)
						getWorld()->playSound(SOUND_GIVE_COIN);
					else
						getWorld()->playSound(SOUND_TAKE_COIN);
				}
			}
		}
		else {
			return;
		}
	}
}

void starSquare::doSomething()
{
	for (int i = 0; i < 2; i++) {
		//if player moves over a star square
		if (getWorld()->getAvatar(i)->getX() == getX() && getWorld()->getAvatar(i)->getY() == getY() && getWorld()->getAvatar(i)->getCoins() >= 20) {
			if (getWorld()->getAvatar(i)->getStatus() == "waiting" && getWorld()->getAvatar(i)->getFirstAttempt() == true) {
				getWorld()->getAvatar(i)->setStars(getWorld()->getAvatar(i)->getStars() + 1);
				getWorld()->getAvatar(i)->setCoins(getWorld()->getAvatar(i)->getCoins() - 20);
				getWorld()->getAvatar(i)->setFirstAttempt(false);
				getWorld()->playSound(SOUND_GIVE_STAR);
			}
			else if (getWorld()->getAvatar(i)->getStatus() == "walking" && getWorld()->getAvatar(i)->getBuffer() == 0) {
				getWorld()->getAvatar(i)->setStars(getWorld()->getAvatar(i)->getStars() + 1);
				getWorld()->getAvatar(i)->setCoins(getWorld()->getAvatar(i)->getCoins() - 20);
				getWorld()->playSound(SOUND_GIVE_STAR);
			}
		}

	}
}

void directionalSquare::doSomething()
{
	for (int i = 0; i < 2; i++) {
		if (getWorld()->getAvatar(i)->getX() == getX() && getWorld()->getAvatar(i)->getY() == getY()) {
			getWorld()->getAvatar(i)->setPlayerDirection(m_direction);
			if (m_direction == left)
				getWorld()->getAvatar(i)->setDirection(left);
			else
				getWorld()->getAvatar(i)->setDirection(right);
			setDirection(m_direction);
		}
		
	}
}

void bankSquare::doSomething()
{
	for (int i = 0; i < 2; i++) {
		//check if player is on the square
		if (getWorld()->getAvatar(i)->getX() == getX() && getWorld()->getAvatar(i)->getY() == getY()) {
			//if player lands on the square
			//add that player's coins by whatever is in the bank
			//set central coins to 0
			//And setFirstAttempt to false
			if (getWorld()->getAvatar(i)->getStatus() == "waiting" && getWorld()->getAvatar(i)->getFirstAttempt() == true) {
				getWorld()->getAvatar(i)->setCoins(getWorld()->getAvatar(i)->getCoins() + getWorld()->getBankCoins());
				getWorld()->setBankCoins(0);
				getWorld()->getAvatar(i)->setFirstAttempt(false);
				getWorld()->playSound(SOUND_WITHDRAW_BANK);
			}
			//if player passes over the bank Square
			// add 5 to the bank 
			// subtract 5 from the player
			else if (getWorld()->getAvatar(i)->getStatus() == "walking") {
				// check to see if subtracting puts player in the negatives 
				//if it does make player coins = 0 and increment the Central Bank coins by that amount
				if (getWorld()->getAvatar(i)->getCoins() - 5 < 0) {
					getWorld()->setBankCoins(getWorld()->getBankCoins() + getWorld()->getAvatar(i)->getCoins());
					getWorld()->getAvatar(i)->setCoins(0);
					getWorld()->playSound(SOUND_DEPOSIT_BANK);
				}//else increment central Bank by 5 and subtract from player's value
				else {
					getWorld()->getAvatar(i)->setCoins(getWorld()->getAvatar(i)->getCoins() - 5);
					getWorld()->setBankCoins(getWorld()->getBankCoins() + 5);
					getWorld()->playSound(SOUND_DEPOSIT_BANK);
				}
			}
		}
	}
}

void eventSquare::doSomething()
{
	for (int i = 0; i < 2;i++) {
		if (getWorld()->getAvatar(i)->getX() == getX() && getWorld()->getAvatar(i)->getY() == getY() && getWorld()->getAvatar(i)->getStatus() == "waiting" && getWorld()->getAvatar(i)->getFirstAttempt() == true) {
			int random = randInt(1, 3);
			if (random == 1) {
				//teleport player to random location
				//play teleport sound
				getWorld()->getAvatar(i)->teleport();
				getWorld()->playSound(SOUND_PLAYER_TELEPORT);
			}
			else if (random == 2) {
				int other = -1; //so cause crash in case that it doesn't get set, but it should always get set
				if (i == 1)
					other = 0;
				else
					other = 1;
				//swap positions
				int tempXPos = getWorld()->getAvatar(i)->getX();
				int tempYPos = getWorld()->getAvatar(i)->getY();
				getWorld()->getAvatar(i)->moveTo(getWorld()->getAvatar(other)->getX(), getWorld()->getAvatar(other)->getY());
				getWorld()->getAvatar(other)->moveTo(tempXPos, tempYPos);
				//swap ticks left
				int tempTicks = getWorld()->getAvatar(i)->getTicks();
				getWorld()->getAvatar(i)->setTicks(getWorld()->getAvatar(other)->getTicks());
				getWorld()->getAvatar(other)->setTicks(tempTicks);
				//Swap Player's walk direction
				int tempPlayerWalkDir = getWorld()->getAvatar(i)->getDirection();
				getWorld()->getAvatar(i)->setDirection(getWorld()->getAvatar(other)->getDirection());
				getWorld()->getAvatar(other)->setDirection(tempPlayerWalkDir);
				//Swap Sprite Direction
				int tempSpriteDir = getWorld()->getAvatar(i)->getPlayerDirection();
				getWorld()->getAvatar(i)->setPlayerDirection(getWorld()->getAvatar(other)->getPlayerDirection());
				getWorld()->getAvatar(other)->setPlayerDirection(tempSpriteDir);
				//swap player's roll/walk state
				std::string currentState = getWorld()->getAvatar(i)->getStatus();
				getWorld()->getAvatar(i)->setStatus(getWorld()->getAvatar(other)->getStatus());
				getWorld()->getAvatar(other)->setStatus(currentState);
				//set FirstAttempt to false so that other player when teleported onto an event square doesn't trigger it
				getWorld()->getAvatar(other)->setFirstAttempt(false);
				getWorld()->getAvatar(i)->setFirstAttempt(false);
				getWorld()->playSound(SOUND_PLAYER_TELEPORT);
			}
			else {
				//last option :3 
				//Give player a vortex
				getWorld()->getAvatar(i)->setVortex();
				getWorld()->getAvatar(i)->setFirstAttempt(false);
				getWorld()->playSound(SOUND_GIVE_VORTEX);
			}
		}
	}
}

void baddies::doSomething()
{
	if (getState() == "dropped")
		return;
	for (int i = 0; i < 2;i++) {
		int other = -1;
		if (getIsImpacted()) {
			teleport();
			setImpacted(false);
			//assuming that teleport doesn't play when a baddie is hit by the vortex
		}
		//boo is in paused state
		if (getState() == "paused") {
			if (m_type == "bowser") {
				//firstAttempt and player and BOWSER are on the same square
				if (getWorld()->getAvatar(i)->getStatus() == "waiting" && getWorld()->getAvatar(i)->getX() == getX() && getWorld()->getAvatar(i)->getY() == getY() && getBaddieFirst() == true) {
					int random = randInt(1, 2);
					if (random == 1) {
						getWorld()->getAvatar(i)->setCoins(0);
						getWorld()->playSound(SOUND_BOWSER_ACTIVATE);
					}
				}
			}
			//firstAttempt and player and boo are on the same square
			if (m_type == "boo") {
				if (getWorld()->getAvatar(i)->getStatus() == "waiting" && getWorld()->getAvatar(i)->getX() == getX() && getWorld()->getAvatar(i)->getY() == getY() && getBaddieFirst() == true) {
					int random = randInt(1, 2);
					if (i == 0) {
						other = 1;
					}
					else
						other = 0;
					//swap coins
					if (random == 1) {
						int tempCoins = getWorld()->getAvatar(i)->getCoins();
						getWorld()->getAvatar(i)->setCoins(getWorld()->getAvatar(other)->getCoins());
						getWorld()->getAvatar(other)->setCoins(tempCoins);
						setBaddieFirst(false);
					}
					//swap stars
					if (random == 2) {
						int tempStars = getWorld()->getAvatar(i)->getStars();
						getWorld()->getAvatar(i)->setStars(getWorld()->getAvatar(other)->getStars());
						getWorld()->getAvatar(other)->setStars(tempStars);
						setBaddieFirst(false);
					}
					getWorld()->playSound(SOUND_BOO_ACTIVATE);
				}
			}
			//decrement the pause counter
			setPauseCounter(getPauseCounter() - 1);

			//if pause counter gets to 0
			if (getPauseCounter() == 0) {
				if (m_type == "bowser")
					setSquaresToMove(randInt(1, 10));
				else
					setSquaresToMove(randInt(1, 3));
				setTicksToMove(getSquaresToMove() * 8);
				int random;
				//set new direction using a random, and repeat until valid direction is found
				for (;;) {
					random = randInt(1, 4);
					//right
					if (random == 1) {
						setSpriteDirection(right);
						setDirection(0);
						if (getX() < 240 && getWorld()->getBoard().getContentsOf(((getX() + 15) / 16) + 1, (getY() + 15) / 16) != 0) {
							break;
						}
					}
					//left
					if (random == 2) {
						setSpriteDirection(left);
						setDirection(180);
						if (getX() > 0 && getWorld()->getBoard().getContentsOf(((getX() + 15) / 16) - 1, (getY() + 15) / 16) != 0) {
							break;
						}
					}
					//down
					if (random == 3) {
						setSpriteDirection(down);
						setDirection(0);
						if (getY() > 0 && getWorld()->getBoard().getContentsOf(((getX() + 15) / 16), ((getY() + 15) / 16) - 1) != 0) {
							break;
						}
					}
					//up
					if (random == 4) {
						setSpriteDirection(up);
						setDirection(0);
						if (getY() < 240 && getWorld()->getBoard().getContentsOf(((getX() + 15) / 16), ((getY() + 15) / 16) + 1) != 0) {
							break;
						}
					}
				}
				setState("walking");
			}
		}
		if (getState() == "walking") {

			//CASE 1: if baddie is at a fork

			setDirectionCounter(0);
			//checking for the number of valid directions
			// add mod 16 checks so we don't do the checks when we are between squares.
			if (((getX() % 16) == 0) && ((getY() % 16) == 0)) {
				if ((getY() != 0) && getWorld()->getBoard().getContentsOf((getX() / 16), (getY() / 16) - 1) != 0)
					setDirectionCounter(getDirectionCounter() + 1);
				if ((getX() != 0) && getWorld()->getBoard().getContentsOf(((getX() / 16) - 1), (getY() / 16)) != 0)
					setDirectionCounter(getDirectionCounter() + 1);
				if ((getY() != 240) && getWorld()->getBoard().getContentsOf((getX() / 16), (getY() / 16) + 1) != 0)
					setDirectionCounter(getDirectionCounter() + 1);
				if ((getX() != 240) && getWorld()->getBoard().getContentsOf(((getX() / 16) + 1), (getY() / 16)) != 0)
					setDirectionCounter(getDirectionCounter() + 1);
			}
			//set random new direction
			if ((getDirectionCounter() > 2)) {
				int random;
				//set new direction using a random, and repeat until valid direction is found
				for (;;) {
					random = randInt(1, 4);
					//right
					//makes sure it's not just going back the way that it came(don't think its needed)? -M
					if (random == 1) {
						setSpriteDirection(right);
						setDirection(0);
						if (getX() < 240 && getWorld()->getBoard().getContentsOf(((getX() + 15) / 16) + 1, (getY() + 15) / 16) != 0) {
							break;
						}
					}
					//left
					if (random == 2) {
						setSpriteDirection(left);
						setDirection(180);
						if (getX() > 0 && getWorld()->getBoard().getContentsOf(((getX() + 15) / 16) - 1, (getY() + 15) / 16) != 0) {
							break;
						}
					}
					//down
					if (random == 3) {
						setSpriteDirection(down);
						setDirection(0);
						if (getY() > 0 && getWorld()->getBoard().getContentsOf(((getX() + 15) / 16), ((getY() + 15) / 16) - 1) != 0) {
							break;
						}
					}
					//up
					if (random == 4) {
						setSpriteDirection(up);
						setDirection(0);
						if (getY() < 240 && getWorld()->getBoard().getContentsOf(((getX() + 15) / 16), ((getY() + 15) / 16) + 1) != 0) {
							break;
						}
					}
				}
			}

			//CASE 2:
			//right is not valid
			if (getSpriteDirection() == right) {
				if (getX() == 240 || getWorld()->getBoard().getContentsOf(((getX()) / 16) + 1, (getY()) / 16) == 0) {
					if (getWorld()->getBoard().getContentsOf(((getX()) / 16), ((getY()) / 16) + 1) == 0) {
						setSpriteDirection(down);

					}
					else {
						setSpriteDirection(up);
					}
				}
			}

			//left is not valid
			if (getSpriteDirection() == left) {
				if (getX() == 0 || getWorld()->getBoard().getContentsOf(((getX() + 15) / 16) - 1, (getY() + 15) / 16) == 0) {
					//checking for only empty squares so it should theoretically work on all kinds of squares
					if (getWorld()->getBoard().getContentsOf(((getX() + 15) / 16), ((getY()) / 16) + 1) == 0) {
						setSpriteDirection(down);
						setDirection(right);

					}
					else {
						setSpriteDirection(up);
					}
				}
			}

			//up is not valid 
			if (getSpriteDirection() == up) {
				if (getY() == 240 || getWorld()->getBoard().getContentsOf(((getX() + 15) / 16), ((getY()) / 16) + 1) == 0) {
					if (getWorld()->getBoard().getContentsOf(((getX()) / 16) + 1, (getY() + 15) / 16) == 0) {
						setSpriteDirection(left);
						setDirection(left);
					}
					else {
						setSpriteDirection(right);
					}
				}
			}

			//down is not valid 
			if (getSpriteDirection() == down) {
				if (getY() == 0 || getWorld()->getBoard().getContentsOf(((getX() + 15) / 16), ((getY() + 15) / 16) - 1) == 0) {
					if (getWorld()->getBoard().getContentsOf(((getX()) / 16) + 1, (getY() + 15) / 16) == 0) {
						setSpriteDirection(left);
						setDirection(left);
					}
					else {
						setSpriteDirection(right);
					}
				}
			}
			//now move Boo
			if (getState() == "walking" && m_type == "boo") {
				moveAtAngle(getSpriteDirection(), 2);
				setTicksToMove(getTicksToMove() - 1);
				//when ticks is 0
				//reset state to paused and set pause counter to 180
				if (getTicksToMove() == 0) {
					setState("paused");
					setPauseCounter(180);
					setBaddieFirst(true);
				}
			}
			//now move Bowser FOR BOWSER ONLY
			if (getState() == "walking" && m_type == "bowser") {
				moveAtAngle(getSpriteDirection(), 2);
				setTicksToMove(getTicksToMove() - 1);
				//when ticks is 0
				//reset state to paused and set pause counter to 180
				if (getTicksToMove() == 0) {
					setState("paused");
					setPauseCounter(180);
					setBaddieFirst(true);
					int random = randInt(1, 4);
					if (random == 1) {
						//insert a new dropping square
						// delete or set alive status to false
						// Ask the Student World object to remove the square underneath
						getWorld()->addDroppingSquare(getX(), getY());
						getWorld()->playSound(SOUND_DROPPING_SQUARE_CREATED);
						getWorld()->deleteSpecificSquare(getX(), getY());
					}
				}
			}
		}
	}
}

void vortexSquare::doSomething()
{
	if (getAliveStatus() == true) {
		//right is not valid
		if (spriteDirection == right) {
			if (getX() == 254)
				setAliveStatus(false);
		}

		//left is not valid
		if (spriteDirection == left) {
			if (getX() == 0)
				setAliveStatus(false);
		}

		//up is not valid
		if (spriteDirection == up) {
			if (getY() == 254)
				setAliveStatus(false);
		}

		//down is not valid
		if (spriteDirection == down) {
			if (getY() == 0)
				setAliveStatus(false);
		}

		if (getAliveStatus()) {
			moveAtAngle(spriteDirection, 2);
			bool impact = getWorld()->checkSquareForImpact(getX(), getY());
			if (impact) {
				getWorld()->playSound(SOUND_HIT_BY_VORTEX);
				setAliveStatus(false);
			}
		}
	}
	else
		return;
}

void droppingSquare::doSomething() {
	for (int i = 0; i < 2; i++) {
		if (getWorld()->getAvatar(i)->getX() == getX() && getWorld()->getAvatar(i)->getY() == getY() && getWorld()->getAvatar(i)->getStatus() == "waiting" && getWorld()->getAvatar(i)->getFirstAttempt() == true) {
			int random = randInt(1, 2);
			//option 1 deduct 10 coins or up to 10 coins if the player has less than 10 coins
			if (random == 1) {
				if (getWorld()->getAvatar(i)->getCoins() < 10)
					getWorld()->getAvatar(i)->setCoins(0);
				else
					getWorld()->getAvatar(i)->setCoins(getWorld()->getAvatar(i)->getCoins() - 10);
			}
			else {
				if (getWorld()->getAvatar(i)->getStars() > 0) {
					getWorld()->getAvatar(i)->setStars(getWorld()->getAvatar(i)->getStars() - 1);
				}
			}
			getWorld()->getAvatar(i)->setFirstAttempt(false);
			getWorld()->playSound(SOUND_DROPPING_SQUARE_ACTIVATE);
		}
	}
}