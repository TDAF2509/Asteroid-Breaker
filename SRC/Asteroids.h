#ifndef __ASTEROIDS_H__
#define __ASTEROIDS_H__

#include "GameUtil.h"
#include "GameSession.h"
#include "IKeyboardListener.h"
#include "IGameWorldListener.h"
#include "IScoreListener.h" 
#include "ScoreKeeper.h"
#include "Player.h"
#include "IPlayerListener.h"

class GameObject;
class Spaceship;
class GUILabel;
class HealthUp;
class BlastPowerUp;
class SpaceshipDemo;

class Asteroids : public GameSession, public IKeyboardListener, public IGameWorldListener, public IScoreListener, public IPlayerListener
{
public:
	Asteroids(int argc, char *argv[]);
	virtual ~Asteroids(void);

	virtual void Start(void);
	virtual void Stop(void);

	// Declaration of IKeyboardListener interface ////////////////////////////////

	void OnKeyPressed(uchar key, int x, int y);
	void OnKeyReleased(uchar key, int x, int y);
	void OnSpecialKeyPressed(int key, int x, int y);
	void OnSpecialKeyReleased(int key, int x, int y);

	// Declaration of IScoreListener interface //////////////////////////////////

	void OnScoreChanged(int score);

	// Declaration of the IPlayerLister interface //////////////////////////////

	void OnPlayerKilled(int lives_left);

	// Declaration of IGameWorldListener interface //////////////////////////////

	void OnWorldUpdated(GameWorld* world) {}
	void OnObjectAdded(GameWorld* world, shared_ptr<GameObject> object) {}
	void OnObjectRemoved(GameWorld* world, shared_ptr<GameObject> object);

	// Override the default implementation of ITimerListener ////////////////////
	void OnTimer(int value);

private:
	shared_ptr<Spaceship> mSpaceship;
	shared_ptr<SpaceshipDemo> mSpaceshipDemo;
	shared_ptr<HealthUp> mHealthUp;
	shared_ptr<BlastPowerUp> mBlastPowerUp;
	shared_ptr<GUILabel> mScoreLabel;
	shared_ptr<GUILabel> mLivesLabel;
	shared_ptr<GUILabel> mStartLabel;
	shared_ptr<GUILabel> mInstructionLabel;
	shared_ptr<GUILabel> mInstruction1Label;
	shared_ptr<GUILabel> mInstruction2Label;
	shared_ptr<GUILabel> mInstruction3Label;
	shared_ptr<GUILabel> mGameOverLabel;


	uint mLevel;
	uint mAsteroidCount;

	void ResetSpaceship();
	shared_ptr<GameObject> CreateSpaceship();
	shared_ptr<GameObject> CreateSpaceshipDemo();
	shared_ptr<GameObject> CreateExplosion();
	shared_ptr<GameObject> CreateBlastExplosion();
	void CreateAsteroids(const uint num_asteroids);
	void CreateMiniAsteroids(const uint num_asteroids);
	void CreateGUI();
	void CreateHealthUp();
	void CreateShield();
	void CreateBlaster();
	
	
	const static uint SHOW_GAME_OVER = 0;
	const static uint START_NEXT_LEVEL = 1;
	const static uint CREATE_NEW_PLAYER = 2;
	const static uint CREATE_HEALTH_UP = 3;
	const static uint CREATE_SHIELD = 4;
	const static uint SHOW_GAME_START = 5;
	const static uint AI_SPACESHIPDEMO = 6;
	const static uint CREATE_SPACESHIPDEMO = 7;
	const static uint CREATE_BLASTER = 8;
	const static uint CREATE_BLAST_TIMER = 9;

	ScoreKeeper mScoreKeeper;
	Player mPlayer;
	bool mGameStart;
};

#endif