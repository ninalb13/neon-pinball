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

	void Create_Bouncers();

	void ControlTunnels();

	void Control_Tunnels();

	bool Deactivate_Tunnel();
public:

	bool win;
	bool gameover;
	bool delete_ball;
	PhysBody* death_sensor;
	//tunnels_sensors
	bool tunnel_on;
	PhysBody* tunnel_upper_sensor;
	PhysBody* tunnel_lower_sensor;

	p2List<PhysBody*> background_;

	//tunnels
	p2List<PhysBody*> tunnels_list;
	PhysBody* longchain;
	PhysBody* longchain_2;
	PhysBody* uppercurve;
	PhysBody* uppercurve_2;
	PhysBody* lowercurve;
	PhysBody* lowercurve_2;

	bool insideTunnel;

	SDL_Texture* background;
	SDL_Texture* circle;

	p2Point<int> ray;
	bool ray_on;

	//Bodies
	PhysBody* ball;

	//Flippers
	b2RevoluteJoint*  leftFlipper;
	b2RevoluteJoint*  rightFlipper;

	float flipperSpeed = 30.00f;

	//Bouncers
	PhysBody* bouncer_1;
	PhysBody* bouncer_2;
	PhysBody* bouncer_3;
	PhysBody* bouncer_4;
	PhysBody* bouncer_5;

//float flipperSpeed = 10.00f;
	
	//triangles
	PhysBody* left_triangle;
	PhysBody* right_triangle;
};
