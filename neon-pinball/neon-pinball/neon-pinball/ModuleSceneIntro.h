#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"
#include "ModulePhysics.h"

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


	//tunnels_sensors
	PhysBody* tunnel_upper_sensor;
	PhysBody* tunnel_lower_sensor;



	p2List<PhysBody*> background_;

	//tunnels
	PhysBody* longchain;
	PhysBody* longchain_2;
	PhysBody* uppercurve;
	PhysBody* uppercurve_2;
	PhysBody* lowercurve;
	PhysBody* lowercurve_2;

	bool sensed;

	SDL_Texture* background;
	SDL_Texture* circle;

	p2Point<int> ray;
	bool ray_on;

	//Bodies
	PhysBody* ball;

	//Flippers
	b2RevoluteJoint*  leftFlipper;
	float flipperSpeed = 100.00f;
};
