#pragma once
#include "Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "Globals.h"
#include "ModulePhysics.h"
enum Game_states { PLAYING, WAITING, WAITING_KEY_UP, FINISHED };

struct Particle {
	iPoint position;
	int max_life;
	int current_life;
};

class PhysBody;
class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();

	bool Start();
	update_status Update();
	void DrawEverything();

	bool CleanUp();
	void OnCollision(PhysBody* bodyA, PhysBody* bodyB);
	void Create_Limits();

	void SwitchColliders();
	void Create_Bouncers();
	void CreateSensors();
	void ControlTunnels();
	void CheckForInput();
	void DoRicksCode();

	void EmitTrail(PhysBody* body);
	
	void SpawnBall(DIRECTION direction);

public:

	bool delete_ball;
	PhysBody* death_sensor;

	//Sensors
	p2List<PhysBody*> sensors_list;
	PhysBody* s1;
	PhysBody* s2;
	PhysBody* s3;
	PhysBody* s4;
	PhysBody* s5;
	PhysBody* s6;

	//tunnel sensors
	PhysBody* tl1;
	PhysBody* tl2;

	PhysBody* tu1;
	PhysBody* tu2;

	PhysBody* te1;
	PhysBody* te2;

	p2List<Particle*> particles;

	Game_states game_state = WAITING;

	PhysBody* left_rectangle_bouncer;
	PhysBody* right_rectangle_bouncer;
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

	//Bouncers
	p2List<PhysBody*> bouncers;
	PhysBody* bouncer_1;
	PhysBody* bouncer_2;
	PhysBody* bouncer_3;
	PhysBody* bouncer_4;
	PhysBody* bouncer_5;
	PhysBody* bouncer_6;
	PhysBody* bouncer_7;
	PhysBody* bouncer_8;
	PhysBody* bouncer_9;
	PhysBody* bouncer_10;

//float flipperSpeed = 10.00f;
	
	//triangles
	PhysBody* left_triangle;
	PhysBody* right_triangle;
};
