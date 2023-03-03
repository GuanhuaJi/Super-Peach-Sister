#include "Actor.h"
#include "StudentWorld.h"
using namespace std;
//*************************************************
//              ACTOR IMPLEMENTATION
//*************************************************
Actor::Actor(StudentWorld* sw, int imageID, int startX, int startY, bool isA, bool canB, bool canD, int startDirection, int depth, double size)
	: GraphObject(imageID, startX, startY, startDirection, depth, size), isAlive(isA), m_world(sw), canBlock(canB) ,damageable(canD){
}
bool Actor::getAlive() const {
	return isAlive;
}
void Actor::setAlive(bool isA) {
	isAlive = isA;
}
StudentWorld* Actor::getWorld() const{
	return m_world;
}
bool Actor::getBlock() const{
	return canBlock;
}
bool Actor::getDamageable() const{
	return damageable;
}
//*************************************************
//              PEACH IMPLEMENTATION
//*************************************************
Peach::Peach(StudentWorld* sw, int startX, int startY) :
	Actor(sw, IID_PEACH, startX, startY, true, false, true), hitPoint(1), tempInv(false), invTime(0), starPow(false), flowPow(false), mushPow(false), starTime(0), recTime(0), remaining_jump_distance(0) {
}
void Peach::bonked() {
	if (starPow) {
		// StarPower: bonk back and do nothing
		getWorld()->playSound(SOUND_PLAYER_KICK);
		getWorld()->callBonked(getX(), getY());
	}
	else if (tempInv) {
		// Temperate Invincibility: do nothing
	}
	else {
		// Decrease hitpoint and set FlowerPower and MushroomPower off
		hitPoint--;
		if (flowPow || mushPow) {
			tempInv = true;
			invTime = 10;
			flowPow = false;
			mushPow = false;
			getWorld()->playSound(SOUND_PLAYER_HURT);
		}
		// Check if Peach died
		if (hitPoint == 0) {
			getWorld()->playSound(SOUND_PLAYER_DIE);
			setAlive(false);
			getWorld()->setGameStatus(4);
		}
	}
}
void Peach::doSomething() {
	// Count down starPower and temperate invincibility
	if (starTime > 0) {
		starTime--;
	}
	else if (starTime == 0) {
		starPow = false;
	}
	if (invTime > 0) {
		invTime--;
	}
	else if (invTime == 0) {
		tempInv = false;
	}

	if (recTime > 0) {
		recTime--;
	}

	if (remaining_jump_distance > 0) {
		if (getWorld()->isBlocked(getX(), getY() + 4)) {
			remaining_jump_distance = 0;
			getWorld()->callBonked(getX(), getY() + 4);
		}
		else {
			moveTo(getX(), getY() + 4);
			remaining_jump_distance--;
		}
	}
	else if (!getWorld()->isBlockBelow(getX(), getY())) {
		moveTo(getX(), getY() - 4);
	}

	int key = -999;
	getWorld()->getKey(key);
	if (key == KEY_PRESS_LEFT) {
		if (!getWorld()->isBlocked(getX() - 4, getY())) {
			setDirection(180);
			moveTo(getX() - 4, getY());
		}
		else {
			getWorld()->callBonked(getX() - 4, getY());
		}
	}
	else if (key == KEY_PRESS_RIGHT) {
		if (!getWorld()->isBlocked(getX() + 4, getY())) {
			setDirection(0);
			moveTo(getX() + 4, getY());
		}
		else {
			getWorld()->callBonked(getX() + 4, getY());
		}
	}
	else if (key == KEY_PRESS_UP) {
		if (getWorld()->isBlockBelow(getX(), getY())) {
			if (mushPow) {
				remaining_jump_distance = 12;
			}
			else {
				remaining_jump_distance = 8;
			}
			getWorld()->playSound(SOUND_PLAYER_JUMP);
		}
	}
	else if (key == KEY_PRESS_SPACE && recTime == 0 && flowPow) {
		if (getDirection() == 0) {
			getWorld()->shoot(1, getX() + 4, getY(), 0);
		}
		else if (getDirection() == 180) {
			getWorld()->shoot(1, getX() - 4, getY(), 180);
		}
		recTime = 8;
	}
}
void Peach::setPower(int type) {
	if (type == 1) {
		flowPow = true;
		hitPoint = 2;
	}
	else if (type == 2) {
		mushPow = true;
		hitPoint = 2;
	}
	else if (type == 3) {
		starPow = true;
		starTime = 150;
		hitPoint = 2;
	}
}
void Peach::getPower(bool& sp, bool& fp, bool& mp) {
	sp = starPow;
	fp = flowPow;
	mp = mushPow;
}
//*************************************************
//            OBSTACLE IMPLEMENTATION
//*************************************************
Obstacle::Obstacle(StudentWorld* sw, int imageID, int startX, int startY) :
	Actor(sw, imageID, startX, startY, true, true, false, 0, 2, 1.0) {
}
void Obstacle::doSomething() {
	// Do nothing
}
void Obstacle::bonked() {
	// Do nothing
}
//*************************************************
//              BLOCK IMPLEMENTATION
//*************************************************
Block::Block(StudentWorld* sw, int startX, int startY, bool sg, bool fg, bool mg) :
	Obstacle(sw, IID_BLOCK, startX, startY), starG(sg), flowG(fg), mushG(mg) {
}
void Block::bonked() {
	if (flowG) {
		getWorld()->playSound(SOUND_POWERUP_APPEARS);
		getWorld()->releaseG(1, getX(), getY());
		flowG = false;
	}
	else if (mushG) {
		getWorld()->playSound(SOUND_POWERUP_APPEARS);
		getWorld()->releaseG(2, getX(), getY());
		mushG = false;
	}
	else if (starG) {
		getWorld()->playSound(SOUND_POWERUP_APPEARS);
		getWorld()->releaseG(3, getX(), getY());
		starG = false;
	}
	else{
		getWorld()->playSound(SOUND_PLAYER_BONK);
	}
}
//*************************************************
//               PIPE IMPLEMENTATION
//*************************************************
Pipe::Pipe(StudentWorld* sw, int startX, int startY) :
	Obstacle(sw, IID_PIPE, startX, startY) {
}
//*************************************************
//              PROPS IMPLEMENTATION
//*************************************************
Props::Props(StudentWorld* sw, int imageID, int startX, int startY) : 
	Actor(sw, imageID, startX, startY, true, false, false, 0, 1, 1){
}
void Props::bonked() {
	// Do nothing
}
void Props::move() {
	if (getWorld()->isBlockBelow(getX(), getY())) {
		if (getDirection() == 0) {
			if (!getWorld()->isBlocked(getX() + 2, getY())) {
				moveTo(getX() + 2, getY());
			}
			else {
				setDirection(180);
			}
		}
		else if (getDirection() == 180) {
			if (!getWorld()->isBlocked(getX() - 2, getY())) {
				moveTo(getX() - 2, getY());
			}
			else {
				setDirection(0);
			}
		}
	}
	else {
		moveTo(getX(), getY() - 2);
	}
}
//*************************************************
//             MUSHROOM IMPLEMENTATION
//*************************************************
Mushroom::Mushroom(StudentWorld* sw, int startX, int startY) : 
	Props(sw, IID_MUSHROOM, startX, startY){
}
void Mushroom::doSomething() {
	if (getWorld()->overlapingP(getX(), getY())) {
		getWorld()->increaseScore(75);
		getWorld()->setPower(2);
		getWorld()->playSound(SOUND_PLAYER_POWERUP);
		setAlive(false);
	}
	else {
		Props::move();
	}
}
//*************************************************
//              FLOWER IMPLEMENTATION
//*************************************************
Flower::Flower(StudentWorld* sw, int startX, int startY) :
	Props(sw, IID_FLOWER, startX, startY) {
}
void Flower::doSomething() {
	if (getWorld()->overlapingP(getX(), getY())) {
		getWorld()->increaseScore(50);
		getWorld()->setPower(1);
		getWorld()->playSound(SOUND_PLAYER_POWERUP);
		setAlive(false);
	}
	else {
		Props::move();
	}
}
//*************************************************
//               STAR IMPLEMENTATION
//*************************************************
Star::Star(StudentWorld* sw, int startX, int startY) : 
	Props(sw, IID_STAR, startX, startY){
}
void Star::doSomething() {
	if (getWorld()->overlapingP(getX(), getY())) {
		getWorld()->increaseScore(100);
		getWorld()->setPower(3);
		getWorld()->playSound(SOUND_PLAYER_POWERUP);
		setAlive(false);
	}
	else {
		Props::move();
	}
}
//*************************************************
//               GOAL IMPLEMENTATION
//*************************************************
Goal::Goal(StudentWorld* sw, int imageID, int startX, int startY) : 
	Actor(sw, imageID, startX, startY, true, false, false, 0, 1, 1){
}
void Goal::bonked() {
	// Do nothing
}
//*************************************************
//               FLAG IMPLEMENTATION
//*************************************************
Flag::Flag(StudentWorld* sw, int startX, int startY) : 
	Goal(sw, IID_FLAG, startX, startY){
}
void Flag::doSomething() {
	if (getAlive()) {
		if (getWorld()->overlapingP(getX(), getY())) {
			getWorld()->increaseScore(1000);
			getWorld()->setGameStatus(2);
			setAlive(false);
		}
	}
}
//*************************************************
//               MARIO IMPLEMENTATION
//*************************************************
Mario::Mario(StudentWorld* sw, int startX, int startY) : 
	Goal(sw, IID_MARIO, startX, startY){
}
void Mario::doSomething() {
	if (getAlive()) {
		if (getWorld()->overlapingP(getX(), getY())) {
			getWorld()->increaseScore(1000);
			getWorld()->setGameStatus(3);
			setAlive(false);
		}
	}
}
//*************************************************
//               ENEMY IMPLEMENTATION
//*************************************************
Enemy::Enemy(StudentWorld* sw, int imageID, int startX, int startY) : 
	Actor(sw, imageID, startX, startY, true, false, true, randomDir(), 0, 1){
}
void Enemy::move() {
	if (getAlive()) {
		if (getWorld()->overlapingP(getX(), getY())) {
			getWorld()->bonkPeach();
			return;
		}
		else {
			if (getDirection() == 0) {
				if (getWorld()->isBlocked(getX() + 1, getY()) ||
					!getWorld()->fullyOnBlocks(getX() + 1, getY())) {
					setDirection(180);
				}
				else {
					moveTo(getX() + 1, getY());
				}
			}
			else if (getDirection() == 180) {
				if (getWorld()->isBlocked(getX() - 1, getY()) ||
					!getWorld()->fullyOnBlocks(getX() - 1, getY())) {
					setDirection(0);
				}
				else {
					moveTo(getX() - 1, getY());
				}
			}
		}
	}
}
void Enemy::bonked() {
	getWorld()->increaseScore(100);
	setAlive(false);
}
//*************************************************
//              GOOMBA IMPLEMENTATION
//*************************************************
Goomba::Goomba(StudentWorld* sw, int startX, int startY) : 
	Enemy(sw, IID_GOOMBA, startX, startY){
}
void Goomba::doSomething() {
	Enemy::move();
}
//*************************************************
//              KOOPA IMPLEMENTATION
//*************************************************
Koopa::Koopa(StudentWorld* sw, int startX, int startY) :
	Enemy(sw, IID_KOOPA, startX, startY) {
}
void Koopa::doSomething() {
	Enemy::move();
}
void Koopa::bonked() {
	int x = getX();
	int y = getY();
	int dir = getDirection();
	getWorld()->shoot(2, x, y, dir);
	Enemy::bonked();
}
//*************************************************
//             PIRANHA IMPLEMENTATION
//*************************************************
Piranha::Piranha(StudentWorld* sw, int startX, int startY) : 
	Enemy(sw, IID_PIRANHA, startX, startY), fireDelay(0){
}
void Piranha::doSomething() {
	if (getAlive()) {
		increaseAnimationNumber();
		if (getWorld()->overlapingP(getX(), getY())) {
			getWorld()->bonkPeach();
		}
		if (getWorld()->sameLevel(getY())) {
			if (getWorld()->getDistance(getX()) < 0) {
				setDirection(0);
				if (fireDelay == 0) {
					if (getWorld()->getDistance(getX()) > -8 * SPRITE_WIDTH) {
						getWorld()->shoot(3, getX(), getY(), 0);
						fireDelay = 40;
					}
				}
				else {
					fireDelay--;
				}
			}
			else if(getWorld()->getDistance(getX()) > 0){
				setDirection(180);
				if (fireDelay == 0) {
					if (getWorld()->getDistance(getX()) < 8 * SPRITE_WIDTH) {
						getWorld()->shoot(3, getX(), getY(), 180);
						fireDelay = 40;
					}
				}
				else {
					fireDelay--;
				}
			}
		}
	}
}
//*************************************************
//               WEAPON IMPLEMENTATION
//*************************************************
Weapon::Weapon(StudentWorld* sw, int imageID, int startX, int startY, int dir) : 
	Actor(sw, imageID, startX, startY, true, false, false, dir, 1, 1) {
}
void Weapon::move() {
	if (!getWorld()->isBlockBelow(getX(), getY())) {
		moveTo(getX(), getY() - 2);
	}
	if (getDirection() == 0) {
		if (!getWorld()->isBlocked(getX() + 2, getY())) {
			moveTo(getX() + 2, getY());
		}
		else {
			setAlive(false);
			return;
		}
	}
	else if (getDirection() == 180) {
		if (!getWorld()->isBlocked(getX() - 2, getY())) {
			moveTo(getX() - 2, getY());
		}
		else {
			setAlive(false);
			return;
		}
	}
}
void Weapon::bonked() {
	// Do nothing
}
//*************************************************
//         PEACH FIREBALL IMPLEMENTATION
//*************************************************
PeachFireBall::PeachFireBall(StudentWorld* sw, int startX, int startY, int dir) : 
	Weapon(sw, IID_PEACH_FIRE, startX, startY, dir){
}
void PeachFireBall::doSomething() {
	if (getWorld()->damage(getX(), getY())) {
		setAlive(false);
	}
	else {
		Weapon::move();
	}
}
//*************************************************
//             SHELL IMPLEMENTATION
//*************************************************
Shell::Shell(StudentWorld* sw, int startX, int startY, int dir) : 
	Weapon(sw, IID_SHELL, startX, startY, dir), disable(true){
}
void Shell::doSomething() {
	if (disable) {
		disable = false;
	}
	else {
		if (getWorld()->damage(getX(), getY())) {
			setAlive(false);
		}
		else {
			Weapon::move();
		}
	}
}
//*************************************************
//          PIRANHA FIREBALL IMPLEMENTATION
//*************************************************
PiranhaFireBall::PiranhaFireBall(StudentWorld* sw, int startX, int startY, int dir) : 
	Weapon(sw, IID_PIRANHA_FIRE, startX, startY, dir) {
}
void PiranhaFireBall::doSomething() {
	if (getWorld()->overlapingP(getX(), getY())) {
		getWorld()->bonkPeach();
		setAlive(false);
	}
	else {
		Weapon::move();
	}
}
//*************************************************
//        FACILITATE FUNCTIONS IMPLEMENTATION
//*************************************************
int randomDir() {
	int d = rand() % 2;
	if (d == 0) {
		return 0;
	}
	return 180;
}