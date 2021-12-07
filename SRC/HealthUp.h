#ifndef _POWERUP_H_
#define _POWERUP_H_
#include "GameObject.h"

class HealthUp : public GameObject {
public:
	HealthUp(void);
	~HealthUp(void);
	void Render(void);

	bool CollisionTest(shared_ptr<GameObject> o);
	void OnCollision(const GameObjectList& objects);
};
#endif // !_POWERUP_H_
