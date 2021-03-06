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

	void AddTunnels();

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
	p2List<PhysBody*> walls_list;

	bool insideTunnel;

	SDL_Texture* background;
	SDL_Texture* circle;
	SDL_Texture* flipper;

	p2Point<int> ray;
	bool ray_on;

	//Bodies
	PhysBody* ball;

	//Flippers
	p2List<b2RevoluteJoint*> flippers;
	b2RevoluteJoint*  leftFlipper;
	b2RevoluteJoint*  leftFlipper2;
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

	//right bar
	PhysBody* right_bar;
	PhysBody* left_bar;

	PhysBody* line_1;
	PhysBody* line_2;
	PhysBody* line_3;
	PhysBody* line_4;
	PhysBody* line_5;


	PhysBody* left_part;
	PhysBody* right_part;
	PhysBody* left_tunnel_up;//
	PhysBody* left_tunnel_down;
	PhysBody* left_tunnel_2;
	PhysBody* small_rightpart;
	PhysBody* right_tunnel;
	PhysBody* right_tunnel_2;
	PhysBody* right_small_2;

	PhysBody* right_move_1;//
	PhysBody* right_move_2;
	PhysBody* right_move_vertical;
	PhysBody* right_move_vertical_1;
	PhysBody* right_block_down;
	PhysBody* right_block_up;

	
};
