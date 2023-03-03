#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
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
    vector<Actor*>container;
    p = nullptr;
    gameStatus = 1;
}

StudentWorld::~StudentWorld() {
    delete p;
    vector<Actor*>::iterator i;
    for (i = container.begin(); i != container.end();) {
        delete* i;
        i = container.erase(i);
    }
}
int StudentWorld::init()
{
    Level lev(assetPath());
    int curLevel = getLevel();
    ostringstream fileName;
    if (curLevel < 10) {
        fileName << "level0" << curLevel << ".txt";
    }
    else if (curLevel >= 10) {
        fileName << "level" << curLevel << ".txt";
    }
    string level_file = fileName.str();

    Level::LoadResult result = lev.loadLevel(level_file);
    if (result == Level::load_fail_file_not_found)
        cerr << "Could not find level01.txt data file" << endl;
    else if (result == Level::load_fail_bad_format)
        cerr << "level01.txt is improperly formatted" << endl;
    else if (result == Level::load_success) {
        cerr << "Successfully loaded level" << endl;

        Level::GridEntry ge;
        for (int i = 0; i < GRID_WIDTH; i++) {
            for (int j = 0; j < GRID_HEIGHT; j++) {
                ge = lev.getContentsOf(i, j);
                int startX = i * SPRITE_WIDTH;
                int startY = j * SPRITE_HEIGHT;
                switch (ge) {
                case Level::empty:
                    // Do nothing
                    break;
                case Level::block:
                    container.push_back(new Block(this, startX, startY, false, false, false));
                    break;
                case Level::mushroom_goodie_block:
                    container.push_back(new Block(this, startX, startY, false, false, true));
                    break;
                case Level::flower_goodie_block:
                    container.push_back(new Block(this, startX, startY, false, true, false));
                    break;
                case Level::star_goodie_block:
                    container.push_back(new Block(this, startX, startY, true, false, false));
                    break;
                case Level::peach:
                    p = new Peach(this, startX, startY);
                    break;
                case Level::pipe:
                    container.push_back(new Pipe(this, startX, startY));
                    break;
                case Level::flag:
                    container.push_back(new Flag(this, startX, startY));
                    break;
                case Level::mario:
                    container.push_back(new Mario(this, startX, startY));
                    break;
                case Level::goomba:
                    container.push_back(new Goomba(this, startX, startY));
                    break;
                case Level::koopa:
                    container.push_back(new Koopa(this, startX, startY));
                    break;
                case Level::piranha:
                    container.push_back(new Piranha(this, startX, startY));
                    break;
                }
            }
        }
    }
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    setGameStatText(statusText());
    p->doSomething();
    vector<Actor*>::iterator i;
    for (i = container.begin(); i != container.end(); i++) {
        (*i)->doSomething();
        if (gameStatus != 1) {
            break;
        }
    }
    for (i = container.begin(); i != container.end(); i++) {
        if (!(*i)->getAlive()) {
            delete* i;
            i = container.erase(i);
            i--;
        }
    }
    if (gameStatus == 1) {
        return GWSTATUS_CONTINUE_GAME;
    }
    else if (gameStatus == 2) {
        playSound(SOUND_FINISHED_LEVEL);
        return GWSTATUS_FINISHED_LEVEL;
    }
    else if (gameStatus == 3) {
        playSound(SOUND_GAME_OVER);
        return GWSTATUS_PLAYER_WON;
    }
    else if (gameStatus == 4) {
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }
    return GWSTATUS_LEVEL_ERROR;
}

void StudentWorld::cleanUp()
{
    delete p;
    vector<Actor*>::iterator i;
    for (i = container.begin(); i != container.end();) {
        delete* i;
        i = container.erase(i);
    }
    gameStatus = 1;
}
bool StudentWorld::isOverlapping(int x1, int y1, int x2, int y2) {
    int xDif = x1 - x2;
    int yDif = y1 - y2;
    if (xDif < SPRITE_WIDTH && xDif > -SPRITE_WIDTH
        && yDif < SPRITE_HEIGHT && yDif > -SPRITE_HEIGHT) {
        return true;
    }
    return false;
}
bool StudentWorld::overlapingP(int x, int y) {
    int xDif = x - p->getX();
    int yDif = y - p->getY();
    if (xDif < SPRITE_WIDTH && xDif > -SPRITE_WIDTH
        && yDif < SPRITE_HEIGHT && yDif > -SPRITE_HEIGHT) {
        return true;
    }
    return false;
}
bool StudentWorld::isBlocked(int x, int y) {
    vector<Actor*>::iterator i;
    for (i = container.begin(); i != container.end(); i++) {
        if ((*i)->getBlock()) {
            if (isOverlapping(x, y, (*i)->getX(), (*i)->getY())) {
                return true;
            }
        }
    }
    return false;
}
bool StudentWorld::damage(int x, int y) {
    vector<Actor*>::iterator i;
    for (i = container.begin(); i != container.end(); i++) {
        if ((*i)->getDamageable()) {
            if (isOverlapping(x, y, (*i)->getX(), (*i)->getY())) {
                (*i)->bonked();
                return true;
            }
        }
    }
    return false;
}
bool StudentWorld::isBlockBelow(int x, int y) {
    vector<Actor*>::iterator i;
    for (i = container.begin(); i != container.end(); i++) {
        if ((*i)->getBlock()) {
            int bx = (*i)->getX();
            int by = (*i)->getY();
            if (x >= bx && x < bx + SPRITE_WIDTH - 1 && y == by + SPRITE_HEIGHT) {
                return true;
            }
            else if (x + SPRITE_WIDTH - 1 > bx && x + SPRITE_WIDTH  - 1 <= bx + SPRITE_WIDTH - 1 && y == by + SPRITE_HEIGHT) {
                return true;
            }
        }
    }
    return false;
}
bool StudentWorld::fullyOnBlocks(int x, int y) {
    vector<Actor*>::iterator i;
    bool left = false;
    bool right = false;
    for (i = container.begin(); i != container.end(); i++) {
        if ((*i)->getBlock()) {
            int bx = (*i)->getX();
            int by = (*i)->getY();
            if (x >= bx && x <= bx + SPRITE_WIDTH - 1 && y == by + SPRITE_HEIGHT) {
                left = true;
            }
            if (x + SPRITE_WIDTH - 1 >= bx && x + SPRITE_WIDTH - 1 <= bx + SPRITE_WIDTH - 1 && y == by + SPRITE_HEIGHT) {
                right = true;
            }
        }
    }
    return (left && right);
}
bool StudentWorld::sameLevel(int y) {
    if (p->getY() >= y - 1.5 * SPRITE_HEIGHT && p->getY() <= y + 1.5 * SPRITE_HEIGHT) {
        return true;
    }
    return false;
}
int StudentWorld::getDistance(int x) {
    return x - (p->getX());
}
void StudentWorld::callBonked(int px, int py) {
    vector<Actor*>::iterator i;
    for (i = container.begin(); i != container.end(); i++) {
        if (isOverlapping(px, py, (*i)->getX(), (*i)->getY())) {
            (*i)->bonked();
        }
    }
}
void StudentWorld::bonkPeach() {
    p->bonked();
}
void StudentWorld::releaseG(int type, int x, int y) {
    int startX = x;
    int startY = y + 8;
    if (type == 1) {
        container.push_back(new Flower(this, startX, startY));
    }
    else if (type == 2) {
        container.push_back(new Mushroom(this, startX, startY));
    }
    else if (type == 3) {
        container.push_back(new Star(this, startX, startY));
    }
}
void StudentWorld::shoot(int type, int startX, int startY, int dir) {
    if (type == 1) {
        container.push_back(new PeachFireBall(this, startX, startY, dir));
        playSound(SOUND_PLAYER_FIRE);
    }
    else if (type == 2) {
        container.push_back(new Shell(this, startX, startY, dir));
    }
    else if (type == 3) {
        container.push_back(new PiranhaFireBall(this, startX, startY, dir));
        playSound(SOUND_PIRANHA_FIRE);
    }
}
void StudentWorld::setPower(int type) {
    p->setPower(type);
}
void StudentWorld::setGameStatus(int status) {
    gameStatus = status;
}
string StudentWorld::statusText() {
    ostringstream text;
    text << "Lives: " << getLives() << "  Level: " << getLevel() << "  Points: " << getScore();
    bool sp = false;
    bool fp = false;
    bool mp = false;
    p->getPower(sp, fp, mp);
    if (sp) {
        text << " StarPower!";
    }
    if (fp) {
        text << " ShootPower!";
    }
    if (mp) {
        text << " JumpPower!";
    }
    string str = text.str();
    return str;
}
