#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"
#include <time.h>

//*************************************************
//               ACTOR DECLARATION
//*************************************************
class StudentWorld;
class Actor : public GraphObject {
public:
	Actor(StudentWorld* sw, int imageID, int startX, int startY, bool isA, bool canB, bool canD, int startDirection = 0, int depth = 0, double size = (1.0));
	virtual void doSomething() = 0;
	virtual void bonked() = 0;
	bool getAlive() const;
	void setAlive(bool isA);
	StudentWorld* getWorld() const;
	bool getBlock() const;
	bool getDamageable() const;
private:
	StudentWorld* m_world;
	bool isAlive;
	bool canBlock;
	bool damageable;
};

//*************************************************
//               PEACH DECLARATION
//*************************************************
class Peach : public Actor {
public:
	Peach(StudentWorld* sw, int startX, int startY);
	virtual void doSomething();
	virtual void bonked();
	void setPower(int type);
	void getPower(bool& sp, bool& fp, bool& mp);
private:
	int hitPoint;
	bool starPow;
	bool flowPow;
	bool mushPow;
	int starTime;
	bool tempInv;
	int invTime;
	int recTime;
	int remaining_jump_distance;
};
//*************************************************
//               OBSTACLE DECLARATION
//*************************************************
class Obstacle : public Actor {
public:
	Obstacle(StudentWorld* sw, int imageID, int startX, int startY);
	void doSomething();
	virtual void bonked();
};
//*************************************************
//               BLOCK DECLARATION
//*************************************************
class Block : public Obstacle {
public:
	Block(StudentWorld* sw, int startX, int startY, bool sg, bool fg, bool mg);
	virtual void bonked();
private:
	bool starG;
	bool flowG;
	bool mushG;
};
//*************************************************
//                PIPE DECLARATION
//*************************************************
class Pipe : public Obstacle {
public:
	Pipe(StudentWorld* sw, int startX, int startY);
private:
};
//*************************************************
//               PROPS DECLARATION
//*************************************************
class Props : public Actor {
public:
	Props(StudentWorld* sw, int imageID, int startX, int startY);
	virtual void doSomething() = 0;
	void move();
	void bonked();
};
//*************************************************
//              MUSHROOM DECLARATION
//*************************************************
class Mushroom : public Props {
public:
	Mushroom(StudentWorld* sw, int startX, int startY);
	virtual void doSomething();
};
//*************************************************
//               FLOWER DECLARATION
//*************************************************
class Flower : public Props {
public:
	Flower(StudentWorld* sw, int startX, int startY);
	virtual void doSomething();
};
//*************************************************
//                STAR DECLARATION
//*************************************************
class Star : public Props {
public:
	Star(StudentWorld* sw, int startX, int startY);
	virtual void doSomething();
};
//*************************************************
//                GOAL DECLARATION
//*************************************************
class Goal : public Actor {
public:
	Goal(StudentWorld* sw, int imageID, int startX, int startY);
	virtual void doSomething() = 0;
	void bonked();
};
//*************************************************
//                FLAG DECLARATION
//*************************************************
class Flag : public Goal {
public:
	Flag(StudentWorld* sw, int startX, int startY);
	virtual void doSomething();
};
//*************************************************
//               MARIO DECLARATION
//*************************************************
class Mario : public Goal {
public:
	Mario(StudentWorld* sw, int startX, int startY);
	virtual void doSomething();
};
//*************************************************
//               ENEMY DECLARATION
//*************************************************
class Enemy : public Actor {
public:
	Enemy(StudentWorld* sw, int imageID, int startX, int startY);
	virtual void doSomething() = 0;
	void move();
	virtual void bonked();
};
//*************************************************
//               GOOMBA DECLARATION
//*************************************************
class Goomba : public Enemy {
public:
	Goomba(StudentWorld* sw, int startX, int startY);
	virtual void doSomething();
};
//*************************************************
//               KOOPA DECLARATION
//*************************************************
class Koopa : public Enemy {
public:
	Koopa(StudentWorld* sw, int startX, int startY);
	virtual void doSomething();
	virtual void bonked();
};
//*************************************************
//               PIRANHA DECLARATION
//*************************************************
class Piranha : public Enemy {
public:
	Piranha(StudentWorld* sw, int startX, int startY);
	virtual void doSomething();
private:
	int fireDelay;
};
//*************************************************
//               WEAPON DECLARATION
//*************************************************
class Weapon : public Actor {
public:
	Weapon(StudentWorld* sw, int imageID, int startX, int startY, int dir);
	virtual void doSomething() = 0;
	void move();
	void bonked();
};
//*************************************************
//           PEACH FIREBALL DECLARATION
//*************************************************
class PeachFireBall : public Weapon {
public:
	PeachFireBall(StudentWorld* sw, int startX, int startY, int dir);
	virtual void doSomething();
};
//*************************************************
//               SHELL DECLARATION
//*************************************************
class Shell : public Weapon {
public:
	Shell(StudentWorld* sw, int startX, int startY, int dir);
	virtual void doSomething();
private:
	bool disable;
};
//*************************************************
//          PIRANHA FIREBALL DECLARATION
//*************************************************
class PiranhaFireBall : public Weapon {
public:
	PiranhaFireBall(StudentWorld* sw, int startX, int startY, int dir);
	virtual void doSomething();
};
//*************************************************
//        FACILITATE FUNCTIONS DECLARATION
//*************************************************
int randomDir();
#endif // ACTOR_H_

