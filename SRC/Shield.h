#ifndef _SHIELD_H_
#define _SHIELD_H_
#include "GameObject.h"

class Shield : public GameObject {

public:
	Shield(void);
	~Shield(void);
	void Render(void);

	bool CollisionTest(shared_ptr<GameObject> o);
	void OnCollision(const GameObjectList& objects);
};


#endif // !_SHIELD_H_