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


	Create_Limits();
	Create_Bouncers();

	//Rectangle bouncers
	float rect_bouncer_angle = 1.07973048f;
	left_rectangle_bouncer = App->physics->CreateRectangleBouncer(145, 700, 180, 1, rect_bouncer_angle);
	right_rectangle_bouncer = App->physics->CreateRectangleBouncer(357, 700, 180, 1, -rect_bouncer_angle);

	//death sensor
	death_sensor = App->physics->CreateRectangleSensor(260, 910, 285, 15, BOUNCER, BALL);

	leftFlipper = App->physics->CreateFlipper(200, 858, LEFT); //HARDCODING
	rightFlipper = App->physics->CreateFlipper(300, 858, RIGHT);

	//sensors for the tunnels

	tunnel_upper_sensor = App->physics->CreateRectangleSensor(100, 210, 33, 6, BOUNCER, BALL);
	tunnel_lower_sensor = App->physics->CreateRectangleSensor(355, 345, 28, 9, BOUNCER, BALL);

	//circular sensor
	sensor_test = App->physics->CreateCircularSensor(80, 700, 7, BOUNCER, BALL);

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
	p2SString title("NEON Pinball - SCORE: %d ", App->player->score);
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


}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	if (bodyB == death_sensor) {
		game_state = WAITING_KEY_UP;
		delete_ball = true;
	}

	if (bodyB == sensor_test)
	{
		App->player->score += 300;
	}

	if (bodyB == tunnel_lower_sensor || bodyB == tunnel_upper_sensor) {
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
	bouncer_1 = App->physics->CreateCircleBouncer(253, 141);
	bouncer_2 = App->physics->CreateCircleBouncer(326, 176);
	bouncer_3 = App->physics->CreateCircleBouncer(155, 447);
	bouncer_4 = App->physics->CreateCircleBouncer(225, 490);
	bouncer_5 = App->physics->CreateCircleBouncer(303, 500);

	bouncers.add(bouncer_1);
	bouncers.add(bouncer_2);
	bouncers.add(bouncer_3);
	bouncers.add(bouncer_4);
	bouncers.add(bouncer_5);
}

void ModuleSceneIntro::ControlTunnels()
{
	p2List_item<PhysBody*>* tunnels_iterator = tunnels_list.getFirst();

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
	//if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	//{
	//	ray_on = !ray_on;
	//	ray.x = App->input->GetMouseX();
	//	ray.y = App->input->GetMouseY();
	//}


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

void ModuleSceneIntro::EmitTrail(PhysBody * body)
{

}


void ModuleSceneIntro::SpawnBall(DIRECTION direction)
{
	int spawn_x = 250;
	int spawn_y = 580;
	bool wake = true;
	ball = App->physics->CreateBall(spawn_x, spawn_y, 10, 0.0f, this);
	if (direction == RIGHT)
	{
		b2Vec2 impulse_right(1.0f, 0.0f);
		b2Vec2 impulse_right_y(0.0f, 1.5f);
		ball->body->ApplyLinearImpulse(impulse_right, impulse_right_y, wake);
	}
	else if (direction == LEFT)
	{
		b2Vec2 impulse_left(-1.0f, 0.0f);
		b2Vec2 impulse_left_y(0.0f, 1.5f);
		ball->body->ApplyLinearImpulse(impulse_left, impulse_left_y, wake);
	}

}

void ModuleSceneIntro::Create_Limits()
{
	//tunnels
	//tunnel 1
	//long chain
	// Pivot 0, 0
	int long_chain[8] = {
		351, 366,
		93, 237,
		97, 226,
		357, 357
	};

	tunnels_list.add(longchain = App->physics->CreateChain(0, 0, long_chain, 8, "static", BOUNCER, BALL));

	// Pivot 0, 0
	int long_chain_2[8] = {
		112, 204,
		116, 194,
		375, 330,
		371, 340
	};

	tunnels_list.add(longchain_2 = App->physics->CreateChain(0, 0, long_chain_2, 8, "static", BOUNCER, BALL));

	//upper_curve
	// Pivot 0, 0
	int upper_curve_outside[46] = {
		89, 231,
		67, 216,
		58, 191,
		64, 158,
		83, 132,
		114, 119,
		148, 124,
		173, 139,
		184, 153,
		190, 169,
		197, 189,
		200, 199,
		190, 200,
		183, 172,
		169, 148,
		148, 136,
		122, 129,
		104, 131,
		86, 143,
		71, 171,
		69, 202,
		85, 218,
		96, 225
	};

	uppercurve = App->physics->CreateChain(0, 0, upper_curve_outside, 46, "static", BOUNCER, BALL);

	// Pivot 0, 0
	int upper_curve_inside[36] = {
		150, 200,
		160, 200,
		155, 182,
		142, 164,
		129, 160,
		116, 162,
		106, 169,
		102, 181,
		104, 193,
		110, 202,
		121, 208,
		126, 200,
		113, 192,
		112, 183,
		115, 173,
		123, 170,
		135, 172,
		143, 181
	};

	uppercurve_2 = App->physics->CreateChain(0, 0, upper_curve_inside, 36, "static", BOUNCER, BALL);

	// Pivot 0, 0
	int lower_curve_inside[20] = {
		351, 410,
		358, 415,
		367, 398,
		368, 379,
		363, 364,
		354, 356,
		345, 363,
		354, 370,
		358, 382,
		358, 393
	};

	lowercurve = App->physics->CreateChain(0, 0, lower_curve_inside, 20, "static", BOUNCER, BALL);

	// Pivot 0, 0
	int lower_curve_outside[32] = {
		386, 427,
		392, 430,
		398, 414,
		405, 397,
		405, 382,
		405, 365,
		396, 351,
		387, 340,
		377, 334,
		370, 340,
		377, 345,
		385, 352,
		393, 363,
		397, 374,
		396, 389,
		390, 410
	};

	lowercurve_2 = App->physics->CreateChain(0, 0, lower_curve_outside, 32, "static", BOUNCER, BALL);
	// Pivot 0, 0
	// Pivot 0, 0
	int Pinball[48] = {
		198, 855,
		153, 886,
		161, 924,
		347, 924,
		354, 885,
		306, 849,
		434, 750,
		433, 206,
		426, 171,
		416, 137,
		398, 107,
		370, 79,
		340, 61,
		308, 49,
		267, 42,
		223, 44,
		182, 53,
		153, 66,
		125, 86,
		99, 113,
		77, 149,
		66, 177,
		66, 196,
		67, 750
	};


	background_.add(App->physics->CreateChain(0, 0, Pinball, 48, "static", BOUNCER, BALL));

	//triangles
	// Pivot 0, 0
	int Triangleleft[6] = {
		99, 622,
		98, 730,
		189, 787
	};

	left_triangle = App->physics->CreateChain(0, 0, Triangleleft, 6, "static", BOUNCER, BALL);

	// Pivot 0, 0
	int Triangleright[6] = {
		401, 622,
		401, 730,
		314, 785
	};
	right_triangle = App->physics->CreateChain(0, 0, Triangleright, 6, "static", BOUNCER, BALL);
}

void ModuleSceneIntro::SwitchColliders()
{
	p2List_item<PhysBody*>* bouncers_iterator = nullptr;


	for (bouncers_iterator = bouncers.getFirst(); bouncers_iterator; bouncers_iterator = bouncers_iterator->next) {
		bouncers_iterator->data->body->SetActive(!bouncers_iterator->data->body->IsActive());
	}
}
