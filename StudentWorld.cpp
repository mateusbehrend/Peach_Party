#include "StudentWorld.h"
#include "GameConstants.h"
#include <string>
#include <sstream>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
    : GameWorld(assetPath)
{

    cleanedUp = false;
    avatars[0] = { nullptr };
    avatars[1] = { nullptr };
    m_bankCoins = 0;
    initted = false;
}

int StudentWorld::init()
{
    // implement code to implement specific board
    int boardNumber;
    boardNumber = getBoardNumber();
    string board = "";
    board = assetPath() + "board0" + std::to_string(boardNumber) + ".txt";
    m_studentBoard.loadBoard(board);
    for (int x = 0; x < BOARD_WIDTH; x++) {
        for (int y = 0; y < BOARD_HEIGHT; y++) {
            //starting spot, blue square, peach and yoshi all init
            if (m_studentBoard.getContentsOf(x, y) == m_studentBoard.player) { //1 is player sqaure enumeration
                actors.push_back(new coinSquare(this, IID_BLUE_COIN_SQUARE, x * 16, y * 16, 3));
                avatars[0] = new Player(this, IID_PEACH, x * 16, y * 16, 1);
                //m_yoshi = new Yoshi(this, IID_PEACH, x * 16, y * 16);
                avatars[1] = new Player(this, IID_YOSHI, x * 16, y * 16, 2);
                //m_peach = new Peach(this, IID_YOSHI, x * 16, y * 16);
            }
            if (m_studentBoard.getContentsOf(x, y) == m_studentBoard.blue_coin_square) //2 is blue sqaure enumeration
                actors.push_back(new coinSquare(this, IID_BLUE_COIN_SQUARE, x * 16, y * 16, 3));
            if (m_studentBoard.getContentsOf(x, y) == m_studentBoard.red_coin_square)
                actors.push_back(new coinSquare(this, IID_RED_COIN_SQUARE, x * 16, y * 16, -3));
            if (m_studentBoard.getContentsOf(x, y) == m_studentBoard.star_square)
                actors.push_back(new starSquare(this, IID_STAR_SQUARE, x * 16, y * 16));
            if (m_studentBoard.getContentsOf(x, y) == m_studentBoard.left_dir_square)
                actors.push_back(new directionalSquare(this, IID_DIR_SQUARE, x * 16, y * 16, 180));
            if (m_studentBoard.getContentsOf(x, y) == m_studentBoard.right_dir_square)
                actors.push_back(new directionalSquare(this, IID_DIR_SQUARE, x * 16, y * 16, 0));
            if (m_studentBoard.getContentsOf(x, y) == m_studentBoard.down_dir_square)
                actors.push_back(new directionalSquare(this, IID_DIR_SQUARE, x * 16, y * 16, 270));
            if (m_studentBoard.getContentsOf(x, y) == m_studentBoard.up_dir_square)
                actors.push_back(new directionalSquare(this, IID_DIR_SQUARE, x * 16, y * 16, 90));
            if (m_studentBoard.getContentsOf(x, y) == m_studentBoard.bank_square)
                actors.push_back(new bankSquare(this, IID_BANK_SQUARE, x * 16, y * 16));
            if (m_studentBoard.getContentsOf(x, y) == m_studentBoard.event_square)
                actors.push_back(new eventSquare(this, IID_EVENT_SQUARE, x * 16, y * 16));
            if (m_studentBoard.getContentsOf(x, y) == m_studentBoard.boo) {
                actors.push_back(new baddies(this, IID_BOO, x * 16, y * 16, "boo"));
                actors.push_back(new coinSquare(this, IID_BLUE_COIN_SQUARE, x * 16, y * 16, 3));
            }
            if (m_studentBoard.getContentsOf(x, y) == m_studentBoard.bowser) {
                actors.push_back(new baddies(this, IID_BOWSER, x * 16, y * 16, "bowser"));
                actors.push_back(new coinSquare(this, IID_BLUE_COIN_SQUARE, x * 16, y * 16, 3));
            }
        }
    }
    int size0 = actors.size();
    initted = true;
    startCountdownTimer(99);  // this placeholder causes timeout after 5 seconds( change to 99)
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // Don't allow moves until the board has been initialized.
    if (!initted)
        return 0;
    // This code is here merely to allow the game to build, run, and terminate after you hit ESC.
    // Notice that the return value GWSTATUS_NOT_IMPLEMENTED will cause our framework to end the game.

    //call all actors to do something 
    std::vector <Actor*> ::iterator it;

    avatars[0]->doSomething();
    avatars[1]->doSomething();

    for (it = actors.begin(); it != actors.end(); it++) {
        if ((*it)->getAliveStatus() == true)
            (*it)->doSomething();
    }
    //deletes all dead actors
    for (it = actors.begin(); it != actors.end(); it++) {
        if ((*it)->getAliveStatus() == false) {
            //int size0 = actors.size();
            delete* it;
            actors.erase(it);
            //int size1 = actors.size();
            it = actors.begin();
        }
    }

    // outputs the scoreboard header
    int SecRemaining = timeRemaining();
    std::ostringstream gameheader;
    gameheader << "P1: Roll: " << (getAvatar(0)->getTicks())/8 << " Stars: " << getAvatar(0)->getStars() << " $$: " << getAvatar(0)->getCoins();
    if (getAvatar(0)->hasVortex())
        gameheader << " VOR ";
    gameheader << " | Time: " << SecRemaining << " | BANK: " << m_bankCoins << " | ";
    gameheader << "P2: Roll: " << (getAvatar(1)->getTicks())/8 << " Stars: " << getAvatar(1)->getStars() << " $$: " << getAvatar(1)->getCoins();
    if (getAvatar(1)->hasVortex())
        gameheader << " VOR ";
    string title = gameheader.str();
    setGameStatText(title);

    //set final score and announce winner
    if (timeRemaining() <= 0) {
        playSound(SOUND_GAME_FINISHED);
        int peach_stars = getAvatar(0)->getStars();
        int peach_coins = getAvatar(0)->getCoins();
        int yoshi_stars = getAvatar(1)->getStars();
        int yoshi_coins = getAvatar(1)->getCoins();
        bool yoshi_won;
        if (yoshi_stars > peach_stars) {
            yoshi_won = true;
        }
        else if (yoshi_stars < peach_stars)
            yoshi_won = false;
        else {
            if (yoshi_coins > peach_coins)
                yoshi_won = true;
            else if (yoshi_coins < peach_coins)
                yoshi_won = false;
            else {
                int random = randInt(1, 2);
                if (random == 1)
                    yoshi_won = true;
                else
                    yoshi_won = false;
            }
        }
        if (yoshi_won)
        {
            setFinalScore(getAvatar(1)->getStars(), getAvatar(1)->getCoins());
            return GWSTATUS_YOSHI_WON;
        }
        else // peach won
        {
            setFinalScore(getAvatar(0)->getStars(), getAvatar(0)->getCoins());
            return GWSTATUS_PEACH_WON;
        }
    }

    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    if (cleanedUp == false) {
        std::vector <Actor*> ::iterator it;
        for (it = actors.begin();it != actors.end();it++) {
            delete* it;
        }
        delete avatars[0];
        delete avatars[1];
        cleanedUp = true;
    }
}

//adds a Dropping Square to the Actors array
void StudentWorld::addDroppingSquare(int x, int y) {
    actors.push_back(new droppingSquare(this, IID_DROPPING_SQUARE, x, y));
}

//adds a Vortex Square to Actors array
void StudentWorld::addVortexSquare(int x, int y, int spriteDirection, int Direction) {
    actors.push_back(new vortexSquare(this, IID_VORTEX, x, y, spriteDirection, Direction));
}
//deletes a specific square
void StudentWorld::deleteSpecificSquare(int x, int y) {
    std::vector <Actor*> ::iterator it;
    for (it = actors.begin();it != actors.end();it++) {
        if ((*it)->getX() == x && (*it)->getY() == y && (*it)->getCanBeDeleted() == true) {
            (*it)->setAliveStatus(false);
            break;
        }
    }
}


// Check for Impact (touching squares).
bool StudentWorld::checkSquareForImpact(int x, int y) {
    std::vector <Actor*> ::iterator it;
    for (it = actors.begin();it != actors.end();it++) {
        if (((*it)->getIsImpactable() == true) &&
            (abs((*it)->getX() - x) < 16 && (abs((*it)->getY() - y) < 16))) {
            (*it)->setImpacted(true);
            return true;
        }
    }
    return false;
}
