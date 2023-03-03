#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include <vector>
#include <string>
using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp
class Actor;
class Peach;
class StudentWorld : public GameWorld
{
public:
	StudentWorld(std::string assetPath);
	~StudentWorld();
	virtual int init();
	virtual int move();
	virtual void cleanUp();
	bool isOverlapping(int x1, int y1, int x2, int y2);
	bool overlapingP(int x, int y);
	bool isBlocked(int x, int y);
	bool damage(int x, int y);
	bool isBlockBelow(int px, int py);
	bool fullyOnBlocks(int x, int y);
	bool sameLevel(int y);
	int getDistance(int x);
	void callBonked(int px, int py);
	void bonkPeach();
	void releaseG(int type, int x, int y);
	void shoot(int type, int x, int y, int dir);
	void setPower(int type);
	void setGameStatus(int status);
	string statusText();
private:
	vector<Actor*> container;
	Peach* p;
	int gameStatus;
};

#endif // STUDENTWORLD_H_
