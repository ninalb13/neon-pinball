#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"
#include "ModulePlayer.h"
#include "ModuleWindow.h"
#include "string.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	circle = NULL;
	background = NULL;
	ray_on = false;
	tunnel_on = false;

}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()

{

	LOG("Loading Intro assets");
	bool ret = true;

	App->renderer->camera.x = App->renderer->camera.y = 0;

	background = App->textures->Load("pinball/Pinball.png");
	circle = App->textures->Load("pinball/wheel.png");
	flipper = App->textures->Load("pinball/flipper.png");

	App->audio->PlayMusic("pinball/pinball_song.ogg");

	Create_Limits();
	Create_Bouncers();
	CreateSensors();

	//Rectangle bouncers
	float rect_bouncer_angle = 1.01173048f;
	left_rectangle_bouncer = App->physics->CreateRectangleBouncer(145, 743, 180, 1, rect_bouncer_angle);
	right_rectangle_bouncer = App->physics->CreateRectangleBouncer(343, 743, 180, 1, -rect_bouncer_angle);

	//death sensor
	death_sensor = App->physics->CreateRectangleSensor(260, 940, 285, 15, BOUNCER, BALL);

	//FLippers
	leftFlipper = App->physics->CreateFlipper(187, 855, LEFT); //HARDCODING
	rightFlipper = App->physics->CreateFlipper(300, 855, RIGHT);

	flippers.add(leftFlipper);
	flippers.add(rightFlipper);

	//sensors for the tunnels
	int tunnel_sensot_radius = 3;

	//left
	tl1 = App->physics->CreateCircularSensor(100, 360, tunnel_sensot_radius);
	tl2 = App->physics->CreateCircularSensor(289, 44, tunnel_sensot_radius);

	//right interior
	tu1 = App->physics->CreateCircularSensor(405, 120, tunnel_sensot_radius);
	tu2 = App->physics->CreateCircularSensor(422, 168, tunnel_sensot_radius);

	//right exterior
	te1 = App->physics->CreateCircularSensor(399, 150, tunnel_sensot_radius);
	te2 = App->physics->CreateCircularSensor(440, 145, tunnel_sensot_radius);


	//tunnel_upper_sensor = App->physics->CreateRectangleSensor(100, 210, 33, 6, BOUNCER, BALL);
	//tunnel_lower_sensor = App->physics->CreateRectangleSensor(355, 345, 28, 9, BOUNCER, BALL);

	//First desactivate all tunnels
	p2List_item<PhysBody*>* tunnels_iterator = tunnels_list.getFirst();
	while (tunnels_iterator) {
		tunnels_iterator->data->body->SetActive(false);
		tunnels_iterator = tunnels_iterator->next;
	}



	return ret;
}

// Load assetsa
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");

	return true;
}

// Update: draw background
update_status ModuleSceneIntro::Update()
{

	//set title

	p2SString title(" -- SCORE: %d - NEON Pinball - BALLS: %d --", App->player->score, App->player->balls);

	App->window->SetTitle(title.GetString());
	ControlTunnels();

	App->renderer->Blit(background, 0, 0);

	DrawEverything();

	CheckForInput();

	DoRicksCode();

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::DrawEverything()
{
	// All draw functions ------------------------------------------------------
	int x = 0;
	int y = 0;

	//Ball
	if (game_state == PLAYING) {
		ball->GetPosition(x, y);
		App->renderer->Blit(circle, x, y, NULL, 1.0f);
	}

	//p2List_item<b2RevoluteJoint*>* flipper_iterator = flippers.getFirst();

	x = 190;
	y = 855;
	float angle = -leftFlipper->GetJointAngle() * RADTODEG;

	App->renderer->Blit(flipper, x, y, NULL, 1.0f,angle,0,0);

	angle = -(rightFlipper->GetJointAngle()-M_PI) * RADTODEG;
	x = 300;
	y = 855;
	App->renderer->Blit(flipper, x, y, NULL, 1.0f, angle, 0, 6);

}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	if (bodyB == death_sensor) {
		game_state = WAITING_KEY_UP;
		delete_ball = true;

		if (App->player->balls != 0)
		{
			App->player->balls--;

			if (App->player->balls == 0 || App->player->balls < 0)
			{
				App->player->balls = 0;
				game_state = FINISHED;
			}

		}
	}

	p2List_item<PhysBody*>* sensor_iterator = sensors_list.getFirst();
	while (sensor_iterator) {
		if (bodyB == sensor_iterator->data)
		{
			App->player->score += 300;
		}
		sensor_iterator = sensor_iterator->next;
	}


	if (bodyB == tl1 || bodyB == tl2) {
		insideTunnel = !insideTunnel;
	}
	if (bodyB == bouncer_1 || bodyB == bouncer_2 || bodyB == bouncer_3 || bodyB == bouncer_4 || bodyB == bouncer_5)
	{
		App->player->score += 300;
	}

	if (bodyB == left_rectangle_bouncer || bodyB == right_rectangle_bouncer)
	{
		App->player->score += 100;
	}


}

void ModuleSceneIntro::Create_Bouncers()
{

	//Upper left corner
	bouncer_1 = App->physics->CreateCircleBouncer(74, 217);
	bouncer_2 = App->physics->CreateCircleBouncer(100, 157);
	bouncer_3 = App->physics->CreateCircleBouncer(148, 110);
	bouncer_4 = App->physics->CreateCircleBouncer(205, 85);

	//Inside tunnel
	bouncer_5 = App->physics->CreateCircleBouncer(100, 567);
	bouncer_6 = App->physics->CreateCircleBouncer(143, 537);
	bouncer_7 = App->physics->CreateCircleBouncer(143, 587);


	//In the middle
	bouncer_8 = App->physics->CreateCircleBouncer(253, 368);
	bouncer_9 = App->physics->CreateCircleBouncer(269, 527);
	bouncer_10 = App->physics->CreateCircleBouncer(309, 457);

	bouncers.add(bouncer_1);
	bouncers.add(bouncer_2);
	bouncers.add(bouncer_3);
	bouncers.add(bouncer_4);
	bouncers.add(bouncer_5);
	bouncers.add(bouncer_6);
	bouncers.add(bouncer_7);
	bouncers.add(bouncer_8);
	bouncers.add(bouncer_9);
	bouncers.add(bouncer_10);
}

void ModuleSceneIntro::CreateSensors()
{
	int radius = 13;
	//Upper left corner
	s1 = App->physics->CreateCircularSensor(130, 216, radius);
	s2 = App->physics->CreateCircularSensor(209, 150, radius);

	//inside tunnel
	s3 = App->physics->CreateCircularSensor(100, 522, radius);

	//right
	s4 = App->physics->CreateCircularSensor(316, 400, radius);
	s5 = App->physics->CreateCircularSensor(400, 400, radius);
	s6 = App->physics->CreateCircularSensor(413, 487, radius);

	sensors_list.add(s1);

}

void ModuleSceneIntro::ControlTunnels()
{
	p2List_item<PhysBody*>* tunnels_iterator = tunnels_list.getFirst();
	p2List_item<PhysBody*>* walls_iterator = walls_list.getFirst();

	if (insideTunnel) {
		while (tunnels_iterator) {
			tunnels_iterator->data->body->SetActive(true);
			tunnels_iterator = tunnels_iterator->next;
		}
	}
	else {
		while (tunnels_iterator) {
			tunnels_iterator->data->body->SetActive(false);
			tunnels_iterator = tunnels_iterator->next;
		}
	}
}

void ModuleSceneIntro::CheckForInput()
{
	//Reset
	if (game_state == FINISHED) {
		if (App->input->GetKey(SDL_SCANCODE_R) == KEY_DOWN) {
			App->player->Reset();
			game_state = WAITING;
		}
	}

	//Flippers
	if (App->input->GetKey(SDL_SCANCODE_Z) == KEY_REPEAT) {
		if (game_state == WAITING) {
			SpawnBall(LEFT);
			game_state = PLAYING;
		}
		leftFlipper->EnableMotor(true);
	}
	else {
		leftFlipper->EnableMotor(false);
	}


	if (App->input->GetKey(SDL_SCANCODE_M) == KEY_REPEAT) {
		if (game_state == WAITING) {
			SpawnBall(RIGHT);
			game_state = PLAYING;
		}
		rightFlipper->EnableMotor(true);
	}
	else {
		rightFlipper->EnableMotor(false);
	}

	if (game_state == WAITING_KEY_UP) {
		if (App->input->GetKey(SDL_SCANCODE_M) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_Z)) {
			if (App->input->GetKey(SDL_SCANCODE_M) == KEY_UP)
			{
				if (game_state == WAITING_KEY_UP)
					game_state = WAITING;
			}
			if (App->input->GetKey(SDL_SCANCODE_Z) == KEY_UP)
			{
				if (game_state == WAITING_KEY_UP)
					game_state = WAITING;
			}
		}
		else {
			game_state = WAITING;
		}
	}


	//Switch
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		SwitchColliders();
	}
}

void ModuleSceneIntro::DoRicksCode()
{
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	fVector normal(0.0f, 0.0f);

	//delete ball
	if (delete_ball == true)
	{
		App->physics->DeleteBody(ball->body);
		ball = nullptr;
		delete_ball = false;
	}

	// ray -----------------
	if (ray_on == true)
	{
		fVector destination(mouse.x - ray.x, mouse.y - ray.y);
		destination.Normalize();
		destination *= ray_hit;

		App->renderer->DrawLine(ray.x, ray.y, ray.x + destination.x, ray.y + destination.y, 255, 255, 255);

		if (normal.x != 0.0f)
			App->renderer->DrawLine(ray.x + destination.x, ray.y + destination.y, ray.x + destination.x + normal.x * 25.0f, ray.y + destination.y + normal.y * 25.0f, 100, 255, 100);
	}
}


void ModuleSceneIntro::AddTunnels()
{

}

void ModuleSceneIntro::EmitTrail(PhysBody * body)
{

}


void ModuleSceneIntro::SpawnBall(DIRECTION direction)
{
	int radius = 6;
	int spawn_x = 250;
	int spawn_y = 580;
	bool wake = true;
	ball = App->physics->CreateBall(spawn_x, spawn_y, radius, 0.0f, this);
	if (direction == RIGHT)
	{
		b2Vec2 impulse_right(0.25f, 0.0f);
		b2Vec2 impulse_right_y(0.0f, 0.1f);
		ball->body->ApplyLinearImpulse(impulse_right, impulse_right_y, wake);
	}
	else if (direction == LEFT)
	{
		b2Vec2 impulse_left(-0.25f, 0.0f);
		b2Vec2 impulse_left_y(0.0f, 0.1f);
		ball->body->ApplyLinearImpulse(impulse_left, impulse_left_y, wake);
	}

}

void ModuleSceneIntro::Create_Limits()
{


	// Pivot 0, 0
	int lefttunnel2[56] = {
		114, 366,
		99, 374,
		86, 386,
		85, 400,
		85, 425,
		84, 451,
		83, 470,
		92, 487,
		106, 498,
		127, 502,
		147, 505,
		160, 511,
		172, 521,
		184, 544,
		190, 542,
		179, 526,
		167, 512,
		154, 505,
		138, 499,
		117, 497,
		98, 490,
		90, 477,
		87, 460,
		87, 440,
		88, 413,
		89, 391,
		95, 382,
		116, 371
	};

	left_tunnel_2 = App->physics->CreateChain(0, 0, lefttunnel2, 56, "static", BOUNCER, BALL);
	// Pivot 0, 0
	// Pivot 0, 0
	int lefttunnelup[84] = {
		284, 34,
		268, 39,
		256, 45,
		249, 52,
		244, 63,
		243, 78,
		246, 95,
		250, 117,
		254, 130,
		256, 145,
		254, 162,
		249, 185,
		236, 207,
		213, 230,
		186, 248,
		155, 257,
		138, 264,
		125, 277,
		117, 292,
		117, 313,
		116, 330,
		107, 342,
		99, 344,
		108, 334,
		112, 321,
		113, 301,
		117, 283,
		129, 268,
		145, 256,
		162, 251,
		177, 246,
		192, 238,
		204, 230,
		219, 221,
		227, 209,
		238, 196,
		249, 174,
		253, 151,
		244, 105,
		241, 85,
		241, 53,
		258, 35
	};

	left_tunnel_up = App->physics->CreateChain(0, 0, lefttunnelup, 84, "static", BOUNCER, BALL);

	// Pivot 0, 0
	int lefttunneldown[82] = {
		116, 366,
		130, 360,
		138, 349,
		142, 335,
		142, 320,
		143, 298,
		152, 283,
		166, 273,
		184, 267,
		202, 260,
		222, 250,
		242, 234,
		258, 216,
		273, 189,
		280, 155,
		278, 133,
		274, 110,
		266, 85,
		265, 70,
		269, 62,
		284, 56,
		286, 60,
		277, 63,
		272, 70,
		274, 84,
		277, 100,
		281, 119,
		286, 144,
		284, 169,
		272, 202,
		256, 227,
		234, 249,
		212, 260,
		190, 272,
		173, 277,
		157, 286,
		150, 307,
		148, 330,
		145, 352,
		129, 368,
		117, 372
	};
	left_tunnel_down = App->physics->CreateChain(0, 0, lefttunneldown, 82, "static", BOUNCER, BALL);
	// Pivot 0, 0


	//mini lines
	// Pivot 0, 0
	int line1[12] = {
		286, 218,
		286, 264,
		240, 318,
		246, 323,
		291, 266,
		291, 216
	};

	line_1 = App->physics->CreateChain(0, 0, line1, 12, "static", BOUNCER, BALL);


	// Pivot 0, 0
	int righttunnel[48] = {
		349, 407,
		345, 401,
		375, 380,
		389, 362,
		399, 350,
		407, 333,
		412, 299,
		414, 260,
		413, 229,
		413, 198,
		410, 183,
		405, 162,
		409, 162,
		413, 175,
		414, 187,
		417, 199,
		417, 219,
		418, 248,
		417, 281,
		416, 310,
		412, 328,
		407, 341,
		395, 359,
		381, 377
	};
	//LINES
	right_tunnel = App->physics->CreateChain(0, 0, righttunnel, 48, "static", BOUNCER, BALL);

	// Pivot 0, 0
	int righttunnel2[40] = {
		277, 58,
		279, 53,
		295, 52,
		311, 55,
		347, 70,
		380, 97,
		402, 133,
		417, 178,
		420, 211,
		421, 239,
		417, 240,
		416, 220,
		416, 197,
		408, 159,
		388, 116,
		366, 88,
		329, 64,
		303, 57,
		289, 56,
		282, 57
	};

	right_tunnel_2 = App->physics->CreateChain(0, 0, righttunnel2,40, "static", BOUNCER, BALL);





	// Pivot 0, 0
	int line2[8] = {
		306, 216,
		306, 259,
		311, 259,
		311, 215
	};

	line_2 = App->physics->CreateChain(0, 0, line2, 8, "static", BOUNCER, BALL);

	// Pivot 0, 0
	int line3[12] = {
		328, 216,
		328, 264,
		285, 320,
		289, 323,
		334, 266,
		334, 216
	};

	line_3 = App->physics->CreateChain(0, 0, line3, 12, "static", BOUNCER, BALL);

	// Pivot 0, 0
	int line4[8] = {
		349, 216,
		350, 258,
		354, 258,
		354, 216
	};

	line_4 = App->physics->CreateChain(0, 0, line4, 8, "static", BOUNCER, BALL);

	// Pivot 0, 0
	int line5[12] = {
		370, 215,
		370, 264,
		327, 320,
		332, 323,
		375, 268,
		375, 214
	};
	line_5 = App->physics->CreateChain(0, 0, line5, 12, "static", BOUNCER, BALL);
	//right almost death abr
	// Pivot 0, 0
	int rightbar[10] = {
		418, 637,
		418, 797,
		301, 855,
		299, 850,
		413, 794
	};

	right_bar = App->physics->CreateChain(0, 0, rightbar, 10, "static", BOUNCER, BALL);

	// Pivot 0, 0
	int leftbar[10] = {
		68, 637,
		68, 797,
		185, 855,
		188, 851,
		71, 794
	};
	left_bar = App->physics->CreateChain(0, 0, leftbar, 10, "static", BOUNCER, BALL);

	// Pivot 0, 0
	// Pivot 0, 0
	int Pinball[44] = {
		185, 950,
		189, 887,
		46, 818,
		46, 210,
		52, 175,
		63, 141,
		81, 108,
		110, 76,
		144, 52,
		187, 36,
		237, 26,
		282, 30,
		331, 43,
		364, 62,
		395, 89,
		416, 116,
		432, 156,
		438, 190,
		440, 208,
		440, 819,
		299, 888,
		303, 944
	};


	background_.add(App->physics->CreateChain(0, 0, Pinball, 44, "static", BOUNCER, BALL));

	// Pivot 0, 0
	int leftpart[46] = {
		46, 276,
		100, 345,
		87, 350,
		69, 362,
		59, 374,
		58, 391,
		58, 477,
		60, 495,
		67, 514,
		73, 531,
		74, 545,
		73, 558,
		75, 576,
		80, 592,
		90, 601,
		102, 609,
		101, 614,
		83, 600,
		74, 586,
		69, 565,
		68, 523,
		59, 504,
		39, 483
	};
	left_part = App->physics->CreateChain(0, 0, leftpart, 46, "static", BOUNCER, BALL);
	//triangles
	// Pivot 0, 0
	// Pivot 0, 0
	int Triangleleft[8] = {
		97, 668,
		98, 773,
		192, 822,
		196, 817
	};

	left_triangle = App->physics->CreateChain(0, 0, Triangleleft, 8, "static", BOUNCER, BALL);

	// Pivot 0, 0
	int Triangleright[8] = {
		389, 671,
		390, 772,
		293, 823,
		292, 818
	};
	right_triangle = App->physics->CreateChain(0, 0, Triangleright, 6, "static", BOUNCER, BALL);


	// Pivot 0, 0
	int rightmovevertical[8] = {
		395, 137,
		406, 162,
		410, 163,
		400, 136
	};

	right_move_vertical = App->physics->CreateChain(0, 0, rightmovevertical, 8, "static", BOUNCER, BALL);

	tunnels_list.add(left_tunnel_up);
	tunnels_list.add(left_tunnel_down);



}

void ModuleSceneIntro::SwitchColliders()
{
	p2List_item<PhysBody*>* bouncers_iterator = nullptr;


	for (bouncers_iterator = bouncers.getFirst(); bouncers_iterator; bouncers_iterator = bouncers_iterator->next) {
		bouncers_iterator->data->body->SetActive(!bouncers_iterator->data->body->IsActive());
	}
}
