
#include "ActionScript.h"

class RotateScript : public ActionScript
{
	TransformComponent *transform;
	float				rotateSpeed;

public:
	RotateScript(GameObject *attachTo);
	~RotateScript();

	// Inherited via ActionScript
	void init() override;
	void update(float deltaTime) override;
};
