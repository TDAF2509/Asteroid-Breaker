#ifndef _BLASTPOWERUP_H_
#define _BLASTPOWERUP_H_
#include "GameObject.h"

class BlastPowerUp : public GameObject {

public:
	BlastPowerUp(void);
	~BlastPowerUp(void);
	void Render(void);

	bool CollisionTest(shared_ptr<GameObject> o);
	void OnCollision(const GameObjectList& objects);
};


#endif // !_SHIELD_H_
