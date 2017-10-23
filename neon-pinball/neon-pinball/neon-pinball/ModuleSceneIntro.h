#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"

class PhysBody;

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);
	void Create_Limits();

public:
	
	p2List<PhysBody*> background_;

	PhysBody* tunnel_1;
	PhysBody* tunnel_1_2;

	bool sensed;

	SDL_Texture* background;
	SDL_Texture* circle;

	p2Point<int> ray;
	bool ray_on;

	//Bodies
	PhysBody* ball;
};
