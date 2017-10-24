#include "Globals.h"
#include "Application.h"
#include "ModuleRender.h"
#include "ModuleSceneIntro.h"
#include "ModuleInput.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePhysics.h"

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	circle  = NULL;
	ray_on = false;
	sensed = false;
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

	Create_Limits();

	App->physics->CreateCircle(100, 200, 10, b2_dynamicBody);



	App->physics->CreateFlipper();

	//sensors for the tunnels

	tunnel_upper_sensor = App->physics->CreateRectangleSensor(100, 210, 33, 6);
	tunnel_lower_sensor = App->physics->CreateRectangleSensor(355, 345, 28, 9);



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
	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		ray_on = !ray_on;
		ray.x = App->input->GetMouseX();
		ray.y = App->input->GetMouseY();
	}
		
	//ball = App->physics->CreateCircle(490, 800, 15, "static", 0x0004, 0x0001);

	App->renderer->Blit(background, 0, 0);


	// Prepare for raycast ------------------------------------------------------
	
	iPoint mouse;
	mouse.x = App->input->GetMouseX();
	mouse.y = App->input->GetMouseY();
	int ray_hit = ray.DistanceTo(mouse);

	fVector normal(0.0f, 0.0f);

	// All draw functions ------------------------------------------------------


	

	// ray -----------------
	if(ray_on == true)
	{
		fVector destination(mouse.x-ray.x, mouse.y-ray.y);
		destination.Normalize();
		destination *= ray_hit;

		App->renderer->DrawLine(ray.x, ray.y, ray.x + destination.x, ray.y + destination.y, 255, 255, 255);

		if(normal.x != 0.0f)
			App->renderer->DrawLine(ray.x + destination.x, ray.y + destination.y, ray.x + destination.x + normal.x * 25.0f, ray.y + destination.y + normal.y * 25.0f, 100, 255, 100);
	}

	return UPDATE_CONTINUE;
}

void ModuleSceneIntro::OnCollision(PhysBody* bodyA, PhysBody* bodyB)
{
	int x, y;


	/*
	if(bodyA)
	{
		bodyA->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}

	if(bodyB)
	{
		bodyB->GetPosition(x, y);
		App->renderer->DrawCircle(x, y, 50, 100, 100, 100);
	}*/
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

	longchain = App->physics->CreateChain(0, 0, long_chain, 8, "static");

	// Pivot 0, 0
	int long_chain_2[8] = {
		112, 204,
		116, 194,
		375, 330,
		371, 340
	};

	longchain_2 = App->physics->CreateChain(0, 0, long_chain_2, 8, "static");

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

	uppercurve = App->physics->CreateChain(0, 0, upper_curve_outside, 46, "static");

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

	uppercurve_2 = App->physics->CreateChain(0, 0, upper_curve_inside, 36, "static");

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

	lowercurve = App->physics->CreateChain(0, 0, lower_curve_inside, 20, "static");

	// Pivot 0, 0
	int lower_curve_outside[32] = {
		385, 427,
		392, 430,
		398, 414,
		405, 397,
		405, 382,
		405, 365,
		396, 351,
		387, 340,
		376, 334,
		368, 340,
		377, 345,
		385, 352,
		393, 363,
		397, 374,
		396, 389,
		390, 410
	};

	lowercurve_2 = App->physics->CreateChain(0, 0, lower_curve_outside, 32, "static");

	// Pivot 0, 0
	int Pinball[54] = {
		198, 912,
		196, 856,
		61, 746,
		60, 195,
		67, 166,
		76, 143,
		89, 120,
		108, 96,
		129, 76,
		153, 60,
		182, 48,
		205, 41,
		229, 36,
		250, 34,
		275, 35,
		308, 42,
		332, 49,
		355, 62,
		375, 75,
		394, 92,
		410, 113,
		421, 136,
		429, 158,
		434, 178,
		438, 769,
		303, 857,
		301, 911
	};

	background_.add(App->physics->CreateChain(0, 0, Pinball, 54, "static"));
}