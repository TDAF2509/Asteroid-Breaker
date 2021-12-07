#include "Asteroid.h"
#include "Asteroids.h"
#include "Animation.h"
#include "AnimationManager.h"
#include "GameUtil.h"
#include "GameWindow.h"
#include "GameWorld.h"
#include "GameDisplay.h"
#include "Spaceship.h"
#include "BoundingShape.h"
#include "BoundingSphere.h"
#include "GUILabel.h"
#include "Explosion.h"
#include "HealthUp.h"
#include "Shield.h"
#include "SpaceshipDemo.h"
#include "BlastEx.h"
#include "Blast.h"
#include "BlastPowerUp.h"


// PUBLIC INSTANCE CONSTRUCTORS ///////////////////////////////////////////////

/** Constructor. Takes arguments from command line, just in case. */
Asteroids::Asteroids(int argc, char *argv[])
	: GameSession(argc, argv)
{
	mLevel = 0;
	mAsteroidCount = 0;
	mGameStart = false;
}

/** Destructor. */
Asteroids::~Asteroids(void)
{
}

// PUBLIC INSTANCE METHODS ////////////////////////////////////////////////////

/** Start an asteroids game. */
void Asteroids::Start()
{
	// Create a shared pointer for the Asteroids game object - DO NOT REMOVE
	shared_ptr<Asteroids> thisPtr = shared_ptr<Asteroids>(this);

	// Add this class as a listener of the game world
	mGameWorld->AddListener(thisPtr.get());

	// Add this as a listener to the world and the keyboard
	mGameWindow->AddKeyboardListener(thisPtr);

	// Add a score keeper to the game world
	mGameWorld->AddListener(&mScoreKeeper);

	// Add this class as a listener of the score keeper
	mScoreKeeper.AddListener(thisPtr);

	// Create an ambient light to show sprite textures
	GLfloat ambient_light[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat diffuse_light[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
	glEnable(GL_LIGHT0);

	Animation *explosion_anim = AnimationManager::GetInstance().CreateAnimationFromFile("explosion", 64, 1024, 64, 64, "explosion_fs.png");
	Animation *asteroid1_anim = AnimationManager::GetInstance().CreateAnimationFromFile("asteroid1", 128, 8192, 128, 128, "asteroid1_fs.png");
	Animation *spaceship_anim = AnimationManager::GetInstance().CreateAnimationFromFile("spaceship", 128, 128, 128, 128, "spaceship_fs.png");
	Animation *healthup_anim = AnimationManager::GetInstance().CreateAnimationFromFile("healthup", 256, 4096, 256, 256, "Health.png");
	Animation *shield_anim = AnimationManager::GetInstance().CreateAnimationFromFile("shield", 182, 182, 182, 182, "shield.png");
	Animation* blastEx_anim = AnimationManager::GetInstance().CreateAnimationFromFile("blastEx", 200,3600 ,200, 225, "blast skull.png");
	Animation* blastPowerUp_anim = AnimationManager::GetInstance().CreateAnimationFromFile("blastPU", 406, 31650, 406, 422, "BlastPowerUp.png");

	// Create the demo spaceship and add it to the world
	mGameWorld->AddObject(CreateSpaceshipDemo());
	// Set the timer for starting the demo spaceships AI 
	SetTimer(100, AI_SPACESHIPDEMO);

	// Create some asteroids and add them to the world
	CreateAsteroids(1);

	// Create a Shield power up and add it to the world
	CreateShield();

	// Create a Blater power up and add it to the world
	CreateBlaster();

	//Create the GUI
	CreateGUI();

	
	// Add a player (watcher) to the game world
	mGameWorld->AddListener(&mPlayer);

	// Add this class as a listener of the player
	mPlayer.AddListener(thisPtr);

	// Start the game
	GameSession::Start();
	
}

/** Stop the current game. */
void Asteroids::Stop()
{
	// Stop the game
	GameSession::Stop();
}

// PUBLIC INSTANCE METHODS IMPLEMENTING IKeyboardListener /////////////////////

void Asteroids::OnKeyPressed(uchar key, int x, int y)
{
	if (!mGameStart)
	{
		switch (key)
		{
		case ' ':
			
			// Set the game to start
			mGameStart = true;

			// Create a spaceship and add it to the game world
			mGameWorld->AddObject(CreateSpaceship());

			// Reset the lives and score counter
			mScoreKeeper.mScore = 0;
			mPlayer.mLives = 3;
			mLevel = 0;

			// Set the labels visibility
			mStartLabel->SetVisible(false);
			mInstructionLabel->SetVisible(false);
			mInstruction1Label->SetVisible(false);
			mInstruction2Label->SetVisible(false);
			mInstruction3Label->SetVisible(false);
			mLivesLabel->SetVisible(true);
			mScoreLabel->SetVisible(true);

			// Remove Spaceship from the Demo
			mGameWorld->FlagForRemoval(mSpaceshipDemo);

			//// Create some asteroids and add them to the world upon game start
			SetTimer(100, START_NEXT_LEVEL);

			// Create a health item and add it to the game world
			CreateHealthUp();

			// Create a shield item and add it to the game world
			CreateShield();

			// Create a shield item and add it to the game world
			CreateBlaster();

			break;
		default:
			break;
		}
	}

	
	switch (key)
	{
	case ' ':
		if (mSpaceship->mBlastReady == true)
		{
			mSpaceship->ShootBlast();
		}
		else
		{
			mSpaceship->Shoot();
		}
		
		break;
	default:
		break;
	}
	
	
}

void Asteroids::OnKeyReleased(uchar key, int x, int y) {}

void Asteroids::OnSpecialKeyPressed(int key, int x, int y)
{
	if (mGameStart)
	{
		switch (key)
		{
			// If up arrow key is pressed start applying forward thrust
		case GLUT_KEY_UP: mSpaceship->Thrust(10); break;
			// If down arrow key is pressed start applying break
		case GLUT_KEY_DOWN: mSpaceship->Thrust(-20); break;
			// If left arrow key is pressed start rotating anti-clockwise
		case GLUT_KEY_LEFT: mSpaceship->Rotate(90); break;
			// If right arrow key is pressed start rotating clockwise
		case GLUT_KEY_RIGHT: mSpaceship->Rotate(-90); break;
			// Default case - do nothing
		default: break;
		}
	}
	
}

void Asteroids::OnSpecialKeyReleased(int key, int x, int y)
{
	if (mGameStart)
	{
		switch (key)
		{
			// If up arrow key is released stop applying forward thrust
		case GLUT_KEY_UP: mSpaceship->Thrust(0); break;
			// If down arrow key is released stop applying break
		case GLUT_KEY_DOWN: mSpaceship->Thrust(0); break;
			// If left arrow key is released stop rotating
		case GLUT_KEY_LEFT: mSpaceship->Rotate(0); break;
			// If right arrow key is released stop rotating
		case GLUT_KEY_RIGHT: mSpaceship->Rotate(0); break;
			// Default case - do nothing
		default: break;
		}
	}
	 
}


// PUBLIC INSTANCE METHODS IMPLEMENTING IGameWorldListener ////////////////////

void Asteroids::OnObjectRemoved(GameWorld* world, shared_ptr<GameObject> object)
{
	if (object->GetType() == GameObjectType("Asteroid"))
	{
	
		if (mGameStart)
		{
			if (mSpaceship->mBlastReady == true)
			{
				shared_ptr<GameObject> explosion = CreateBlastExplosion();
				explosion->SetPosition(object->GetPosition());
				explosion->SetRotation(object->GetRotation());
				explosion->SetVelocity(object->GetVelocity());
				explosion->SetAcceleration(object->GetAcceleration());
				mGameWorld->AddObject(explosion);
				mAsteroidCount--;
			}
			else
			{
				shared_ptr<GameObject> explosion = CreateExplosion();
				explosion->SetPosition(object->GetPosition());
				explosion->SetRotation(object->GetRotation());
				explosion->SetVelocity(object->GetVelocity());
				explosion->SetAcceleration(object->GetAcceleration());
				mGameWorld->AddObject(explosion);
				mAsteroidCount--;
				
			}
		}
		else
		{
			if (mSpaceshipDemo->mBlastReadyDemo == true)
			{
				shared_ptr<GameObject> explosion = CreateBlastExplosion();
				explosion->SetPosition(object->GetPosition());
				explosion->SetRotation(object->GetRotation());
				explosion->SetVelocity(object->GetVelocity());
				explosion->SetAcceleration(object->GetAcceleration());
				mGameWorld->AddObject(explosion);
				mAsteroidCount--;

			}
			else
			{
				shared_ptr<GameObject> explosion = CreateExplosion();
				explosion->SetPosition(object->GetPosition());
				explosion->SetRotation(object->GetRotation());
				explosion->SetVelocity(object->GetVelocity());
				explosion->SetAcceleration(object->GetAcceleration());
				mGameWorld->AddObject(explosion);
				mAsteroidCount--;

			}
		}
		
		if (mAsteroidCount <= 0)
		{
			//mStartLabel->SetVisible(true);
			SetTimer(500, START_NEXT_LEVEL);
		}
	}


	if (object->GetType() == GameObjectType("HealthUp"))
	{
		// Increase the Players Life by 1
		mPlayer.mLives++;
		// Update the Players lives
		std::ostringstream msg_stream;
		msg_stream << "Lives: " << mPlayer.mLives;
		std::string lives_msg = msg_stream.str();
		mLivesLabel->SetText(lives_msg);

		SetTimer(10000 + rand()% 10000, CREATE_HEALTH_UP);
	}

	if (object->GetType() == GameObjectType("Shield"))
	{
		if (mSpaceship)
		{
			if (mSpaceship->mShieldOn == false)
			{
				mSpaceship->mShieldOn = true;
				
			}
			else
			{
				mScoreKeeper.returnScore();
			}
		}

		if (mSpaceshipDemo)
		{
			if (mSpaceshipDemo->mShieldOnDemo == false)
			{
				mSpaceshipDemo->mShieldOnDemo = true;
			}
			else
			{
				mScoreKeeper.returnScore();
			}
		}

		SetTimer(8000, CREATE_SHIELD);
	}


	if (object->GetType() == GameObjectType("BlastPowerUp"))
	{
		if (mSpaceship)
		{
			if (mSpaceship->mBlastReady == false)
			{
				mSpaceship->mBlastReady = true;

			}
			else
			{
				mScoreKeeper.returnScore();
			}
		}

		if (mSpaceshipDemo)
		{
			if (mSpaceshipDemo->mBlastReadyDemo == false)
			{
				mSpaceshipDemo->mBlastReadyDemo = true;
			}
			else
			{
				mScoreKeeper.returnScore();
			}
		}

		SetTimer(30000, CREATE_BLASTER);
		SetTimer(10000, CREATE_BLAST_TIMER);
	}
	if (object->GetType() == GameObjectType("SpaceshipDemo"))
	{
		
		shared_ptr<GameObject> explosion = CreateExplosion();
		explosion->SetPosition(mSpaceshipDemo->GetPosition());
		explosion->SetRotation(mSpaceshipDemo->GetRotation());
		explosion->SetVelocity(mSpaceshipDemo->GetVelocity());
		explosion->SetAcceleration(mSpaceshipDemo->GetAcceleration());
		mGameWorld->AddObject(explosion);
		mAsteroidCount--;
		
		if (!mGameStart)
		{
			SetTimer(800, CREATE_SPACESHIPDEMO);
		}
		
	}
}

// PUBLIC INSTANCE METHODS IMPLEMENTING ITimerListener ////////////////////////

void Asteroids::OnTimer(int value)
{
	if (value == CREATE_NEW_PLAYER)
	{
		mSpaceship->Reset();
		mGameWorld->AddObject(mSpaceship);
	}

	if (value == START_NEXT_LEVEL)
	{
		mLevel++;
   		int num_asteroids = 1 + mLevel;
		CreateAsteroids(num_asteroids);
	}

	if (value == SHOW_GAME_OVER)
	{
		mGameOverLabel->SetVisible(true);
		// Initiates the Start screen after a game over
		SetTimer(3000, SHOW_GAME_START);
	}

	if (value == SHOW_GAME_START)
	{
		mGameOverLabel->SetVisible(false);
		mLivesLabel->SetVisible(false);
		mScoreLabel->SetVisible(false);
		mScoreKeeper.mScore = 0;
		mLevel = 0;
		// Create the GUI
		CreateGUI();
		
		// Create a demo spaceship and add it to the game world
		mGameWorld->AddObject(CreateSpaceshipDemo());

		mGameStart = false;
		if (mGameStart == false)
		{
			SetTimer(100, AI_SPACESHIPDEMO);
		
		}

	}

	// Adds another health power up to the game world
	if (value == CREATE_HEALTH_UP)
	{
		CreateHealthUp();
	}

	// Adds another shield power up to the game world
	if (value == CREATE_SHIELD)
	{
		CreateShield();
	}

	// Adds another shield power up to the game world
	if (value == CREATE_BLASTER)
	{
		CreateBlaster();
	}

	// Randomise the AI Spaceships movement and shooting while the mGameStart is false (Demo screen)
	if (value == AI_SPACESHIPDEMO)
	{
		if (!mGameStart)
		{
			shared_ptr<GameObject> asteroid = make_shared<Asteroid>();
			// Set the demo spaceship to rotate in the direction of the nearest asteroid
			mSpaceshipDemo->SetRotation((asteroid->GetAngle() - mSpaceshipDemo->GetAngle()) - rand() %100);
			// Randomise the thrust of the demo spaceship
			mSpaceshipDemo->Thrust(rand() % 10);
			// Setting up the blaster for the demo spaceship
			if (mSpaceshipDemo->mBlastReadyDemo == true)
			{
				mSpaceshipDemo->ShootBlast();
			}
			else
			{
				mSpaceshipDemo->Shoot();
			}
			SetTimer(200, AI_SPACESHIPDEMO);
		}
	}


	// Recreate the Spaceship in the demo screen after its been destroyed
	if (value == CREATE_SPACESHIPDEMO)
	{
		if (!mGameStart)
		{
			mGameWorld->AddObject(CreateSpaceshipDemo());
		}
		
	}

	// Create timer for when the blast is eligible
	if (value == CREATE_BLAST_TIMER)
	{
		if (!mGameStart)
		{
			mSpaceshipDemo->mBlastReadyDemo = false;
		}
		else
		{
			mSpaceship->mBlastReady = false;
		}
		
	}

}

// PROTECTED INSTANCE METHODS /////////////////////////////////////////////////
shared_ptr<GameObject> Asteroids::CreateSpaceship()
{
	// Create a raw pointer to a spaceship that can be converted to
	// shared_ptrs of different types because GameWorld implements IRefCount
	mSpaceship = make_shared<Spaceship>();
	mSpaceship->SetBoundingShape(make_shared<BoundingSphere>(mSpaceship->GetThisPtr(), 4.0f));
	shared_ptr<Shape> bullet_shape = make_shared<Shape>("bullet.shape");
	mSpaceship->SetBulletShape(bullet_shape);
	Animation *anim_ptr = AnimationManager::GetInstance().GetAnimationByName("spaceship");
	shared_ptr<Sprite> spaceship_sprite =
		make_shared<Sprite>(anim_ptr->GetWidth(), anim_ptr->GetHeight(), anim_ptr);
	mSpaceship->SetSprite(spaceship_sprite);
	mSpaceship->SetScale(0.1f);
	// Reset spaceship back to centre of the world
	mSpaceship->Reset();
	// Return the spaceship so it can be added to the world
	return mSpaceship;

}

shared_ptr<GameObject> Asteroids::CreateSpaceshipDemo()
{
	// Create a raw pointer to a spaceship that can be converted to shared_ptrs of different types because GameWorld implements IRefCount
	mSpaceshipDemo = make_shared<SpaceshipDemo>();
	mSpaceshipDemo->SetBoundingShape(make_shared<BoundingSphere>(mSpaceshipDemo->GetThisPtr(), 4.0f));
	shared_ptr<Shape> bullet_shape = make_shared<Shape>("bullet.shape");
	mSpaceshipDemo->SetBulletDemoShape(bullet_shape);
	Animation* anim_ptr = AnimationManager::GetInstance().GetAnimationByName("spaceship");
	shared_ptr<Sprite> spaceship_sprite =
		make_shared<Sprite>(anim_ptr->GetWidth(), anim_ptr->GetHeight(), anim_ptr);
	mSpaceshipDemo->SetSprite(spaceship_sprite);
	mSpaceshipDemo->SetScale(0.1f);
	mSpaceshipDemo->Reset();
	// return SpaceshipDemo so it can be added to the world
	return mSpaceshipDemo;
}

void Asteroids::CreateAsteroids(const uint num_asteroids)
{
	mAsteroidCount = num_asteroids;
	for (uint i = 0; i < num_asteroids; i++)
	{
		Animation *anim_ptr = AnimationManager::GetInstance().GetAnimationByName("asteroid1");
		shared_ptr<Sprite> asteroid_sprite
			= make_shared<Sprite>(anim_ptr->GetWidth(), anim_ptr->GetHeight(), anim_ptr);
		asteroid_sprite->SetLoopAnimation(true);
		shared_ptr<GameObject> asteroid = make_shared<Asteroid>();
		asteroid->SetBoundingShape(make_shared<BoundingSphere>(asteroid->GetThisPtr(), 10.0f));
		asteroid->SetSprite(asteroid_sprite);
		asteroid->SetScale(0.2f);
		mGameWorld->AddObject(asteroid);
	}
}

void Asteroids::CreateMiniAsteroids(const uint num_asteroids)
{
	mAsteroidCount = num_asteroids;
	for (uint i = 0; i < num_asteroids; i++)
	{
		Animation* anim_ptr = AnimationManager::GetInstance().GetAnimationByName("asteroid1");
		shared_ptr<Sprite> asteroid_sprite
			= make_shared<Sprite>(anim_ptr->GetWidth(), anim_ptr->GetHeight(), anim_ptr);
		asteroid_sprite->SetLoopAnimation(true);
		shared_ptr<GameObject> miniasteroid = make_shared<Asteroid>();
		shared_ptr<Asteroid> asteroid = make_shared<Asteroid>();
		miniasteroid->SetBoundingShape(make_shared<BoundingSphere>(miniasteroid->GetThisPtr(), 10.0f));
		miniasteroid->SetSprite(asteroid_sprite);
		miniasteroid->SetScale(0.05f);
		
		mGameWorld->AddObject(miniasteroid);
	}
}

void Asteroids::CreateHealthUp()
{
	Animation* anim_ptr = AnimationManager::GetInstance().GetAnimationByName("healthup");
	shared_ptr<Sprite> health_sprite
		= make_shared<Sprite>(anim_ptr->GetWidth(), anim_ptr->GetHeight(), anim_ptr);
	health_sprite->SetLoopAnimation(true);
	shared_ptr<GameObject> mHealthUp = make_shared<HealthUp>();
	shared_ptr<HealthUp> healthup = make_shared<HealthUp>();
	mHealthUp->SetBoundingShape(make_shared<BoundingSphere>(mHealthUp->GetThisPtr(), 10.0f));
	mHealthUp->SetSprite(health_sprite);
	mHealthUp->SetScale(0.08f);
	mGameWorld->AddObject(mHealthUp);
}

void Asteroids::CreateShield()
{
	Animation* anim_ptr = AnimationManager::GetInstance().GetAnimationByName("shield");
	shared_ptr<Sprite> shield_sprite
		= make_shared<Sprite>(anim_ptr->GetWidth(), anim_ptr->GetHeight(), anim_ptr);
	shield_sprite->SetLoopAnimation(true);
	shared_ptr<GameObject> mShield = make_shared<Shield>();
	shared_ptr<Shield> shield = make_shared<Shield>();
	mShield->SetBoundingShape(make_shared<BoundingSphere>(mShield->GetThisPtr(), 10.0f));
	mShield->SetSprite(shield_sprite);
	mShield->SetScale(0.05f);
	mGameWorld->AddObject(mShield);

}

void Asteroids::CreateBlaster()
{
	Animation* anim_ptr = AnimationManager::GetInstance().GetAnimationByName("blastPU");
	shared_ptr<Sprite> blaster_sprite
		= make_shared<Sprite>(anim_ptr->GetWidth(), anim_ptr->GetHeight(), anim_ptr);
	blaster_sprite->SetLoopAnimation(true);
	shared_ptr<GameObject> mBlastPowerUp = make_shared<BlastPowerUp>();
	shared_ptr<BlastPowerUp> blaster = make_shared<BlastPowerUp>();
	mBlastPowerUp->SetBoundingShape(make_shared<BoundingSphere>(mBlastPowerUp->GetThisPtr(), 10.0f));
	mBlastPowerUp->SetSprite(blaster_sprite);
	mBlastPowerUp->SetScale(0.03f);
	mGameWorld->AddObject(mBlastPowerUp);

}

void Asteroids::CreateGUI()
{
	// Add a (transparent) border around the edge of the game display
	mGameDisplay->GetContainer()->SetBorder(GLVector2i(10, 10));
	// Create a new GUILabel and wrap it up in a shared_ptr
	mScoreLabel = make_shared<GUILabel>("Score: 0");
	// Set the vertical alignment of the label to GUI_VALIGN_TOP
	mScoreLabel->SetVerticalAlignment(GUIComponent::GUI_VALIGN_TOP);
	// Set the visibility of the Label
	mScoreLabel->SetVisible(false);
	// Add the GUILabel to the GUIComponent  
	shared_ptr<GUIComponent> score_component
		= static_pointer_cast<GUIComponent>(mScoreLabel);
	mGameDisplay->GetContainer()->AddComponent(score_component, GLVector2f(0.0f, 1.0f));

	// Create a new GUILabel and wrap it up in a shared_ptr
	mLivesLabel = make_shared<GUILabel>("Lives: 3");
	// Set the vertical alignment of the label to GUI_VALIGN_BOTTOM
	mLivesLabel->SetVerticalAlignment(GUIComponent::GUI_VALIGN_BOTTOM);
	// Set the visibility of the Label
	mLivesLabel->SetVisible(false);
	// Add the GUILabel to the GUIComponent  
	shared_ptr<GUIComponent> lives_component = static_pointer_cast<GUIComponent>(mLivesLabel);
	mGameDisplay->GetContainer()->AddComponent(lives_component, GLVector2f(0.0f, 0.0f));

	// Create a new GUILabel and wrap it up in a shared_ptr
	mStartLabel = shared_ptr<GUILabel>(new GUILabel("PRESS SPACE TO START GAME"));
	// Set the horizontal alignment of the label to GUI_HALIGN_CENTER
	mStartLabel->SetHorizontalAlignment(GUIComponent::GUI_HALIGN_CENTER);
	// Set the vertical alignment of the label to GUI_VALIGN_MIDDLE
	mStartLabel->SetVerticalAlignment(GUIComponent::GUI_VALIGN_MIDDLE);
	// Set the visibility of the label to false (hidden)
	mStartLabel->SetVisible(true);
	// Add the GUILabel to the GUIContainer  
	shared_ptr<GUIComponent> start_component
		= static_pointer_cast<GUIComponent>(mStartLabel);
	mGameDisplay->GetContainer()->AddComponent(start_component, GLVector2f(0.5f, 0.5f));


	// Create a new GUILabel and wrap it up in a shared_ptr
	mInstruction3Label = shared_ptr<GUILabel>(new GUILabel("COLLECT RED ORBS FOR A BLASTER CANNON"));
	// Set the vertical alignment of the label to GUI_VALIGN_MIDDLE
	mInstruction3Label->SetVerticalAlignment(GUIComponent::GUI_VALIGN_TOP);
	// Set the visibility of the label to false (hidden)
	mInstruction3Label->SetVisible(true);
	// Add the GUILabel to the GUIContainer  
	shared_ptr<GUIComponent> instruction3_component
		= static_pointer_cast<GUIComponent>(mInstruction3Label);
	mGameDisplay->GetContainer()->AddComponent(instruction3_component, GLVector2f(0.0f, 0.7f));

	// Create a new GUILabel and wrap it up in a shared_ptr
	mInstruction2Label = shared_ptr<GUILabel>(new GUILabel("COLLECT GREEN ORBS FOR AN EXTRA LIFE"));
	// Set the vertical alignment of the label to GUI_VALIGN_MIDDLE
	mInstruction2Label->SetVerticalAlignment(GUIComponent::GUI_VALIGN_TOP);
	// Set the visibility of the label to false (hidden)
	mInstruction2Label->SetVisible(true);
	// Add the GUILabel to the GUIContainer  
	shared_ptr<GUIComponent> instruction2_component
		= static_pointer_cast<GUIComponent>(mInstruction2Label);
	mGameDisplay->GetContainer()->AddComponent(instruction2_component, GLVector2f(0.0f, 0.8f));


	// Create a new GUILabel and wrap it up in a shared_ptr
	mInstruction1Label = shared_ptr<GUILabel>(new GUILabel("COLLECT BLUE ORBS FOR A SHIELD"));
	// Set the vertical alignment of the label to GUI_VALIGN_MIDDLE
	mInstruction1Label->SetVerticalAlignment(GUIComponent::GUI_VALIGN_TOP);
	// Set the visibility of the label to false (hidden)
	mInstruction1Label->SetVisible(true);
	// Add the GUILabel to the GUIContainer  
	shared_ptr<GUIComponent> instruction1_component
		= static_pointer_cast<GUIComponent>(mInstruction1Label);
	mGameDisplay->GetContainer()->AddComponent(instruction1_component, GLVector2f(0.0f, 0.9f));

	// Create a new GUILabel and wrap it up in a shared_ptr
	mInstructionLabel = shared_ptr<GUILabel>(new GUILabel("PRESS ARROW KEYS TO MOVE"));
	// Set the vertical alignment of the label to GUI_VALIGN_MIDDLE
	mInstructionLabel->SetVerticalAlignment(GUIComponent::GUI_VALIGN_TOP);
	// Set the visibility of the label to false (hidden)
	mInstructionLabel->SetVisible(true);
	// Add the GUILabel to the GUIContainer  
	shared_ptr<GUIComponent> instruction_component
		= static_pointer_cast<GUIComponent>(mInstructionLabel);
	mGameDisplay->GetContainer()->AddComponent(instruction_component, GLVector2f(0.0f, 1.0f));


	// Create a new GUILabel and wrap it up in a shared_ptr
	mGameOverLabel = shared_ptr<GUILabel>(new GUILabel("GAMES OVER"));
	// Set the horizontal alignment of the label to GUI_HALIGN_CENTER
	mGameOverLabel->SetHorizontalAlignment(GUIComponent::GUI_HALIGN_CENTER);
	// Set the vertical alignment of the label to GUI_VALIGN_MIDDLE
	mGameOverLabel->SetVerticalAlignment(GUIComponent::GUI_VALIGN_MIDDLE);
	// Set the visibility of the label to false (hidden)
	mGameOverLabel->SetVisible(false);
	// Add the GUILabel to the GUIContainer  
	shared_ptr<GUIComponent> game_over_component
		= static_pointer_cast<GUIComponent>(mGameOverLabel);
	mGameDisplay->GetContainer()->AddComponent(game_over_component, GLVector2f(0.5f, 0.5f));

}

void Asteroids::OnScoreChanged(int score)
{
	// Format the score message using an string-based stream
	std::ostringstream msg_stream;
	msg_stream << "Score: " << score;
	// Get the score message as a string
	std::string score_msg = msg_stream.str();
	mScoreLabel->SetText(score_msg);
}

void Asteroids::OnPlayerKilled(int lives_left)
{
	shared_ptr<GameObject> explosion = CreateExplosion();
	explosion->SetPosition(mSpaceship->GetPosition());
	explosion->SetRotation(mSpaceship->GetRotation());
	mGameWorld->AddObject(explosion);

	// Format the lives left message using an string-based stream
	std::ostringstream msg_stream;
	msg_stream << "Lives: " << lives_left;
	// Get the lives left message as a string
	std::string lives_msg = msg_stream.str();
	mLivesLabel->SetText(lives_msg);

	if (lives_left > 0) 
	{ 
		SetTimer(1000, CREATE_NEW_PLAYER); 
	}
	else
	{
		SetTimer(500, SHOW_GAME_OVER);
	}
}

shared_ptr<GameObject> Asteroids::CreateExplosion()
{
	Animation *anim_ptr = AnimationManager::GetInstance().GetAnimationByName("explosion");
	shared_ptr<Sprite> explosion_sprite =
		make_shared<Sprite>(anim_ptr->GetWidth(), anim_ptr->GetHeight(), anim_ptr);
	explosion_sprite->SetLoopAnimation(false);
	shared_ptr<GameObject> explosion = make_shared<Explosion>();
	explosion->SetSprite(explosion_sprite);
	explosion->Reset();
	return explosion;
	
}

shared_ptr<GameObject> Asteroids::CreateBlastExplosion()
{
	Animation* anim_ptr = AnimationManager::GetInstance().GetAnimationByName("blastEx");
	shared_ptr<Sprite> explosion_sprite =
		make_shared<Sprite>(anim_ptr->GetWidth(), anim_ptr->GetHeight(), anim_ptr);
	explosion_sprite->SetLoopAnimation(false);
	shared_ptr<GameObject> explosion = make_shared<BlastEx>();
	explosion->SetSprite(explosion_sprite);
	explosion->SetScale(0.25f);
	explosion->Reset();
	return explosion;

}