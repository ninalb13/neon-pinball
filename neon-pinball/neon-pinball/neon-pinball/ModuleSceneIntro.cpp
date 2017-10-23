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


	return ret;
}

// Load assets
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
	// Pivot 0, 0
	int Tunnel_1[48] = {
		149, 200,
		140, 179,
		125, 169,
		112, 180,
		114, 192,
		125, 199,
		375, 331,
		392, 345,
		402, 360,
		405, 379,
		392, 429,
		384, 425,
		394, 380,
		393, 365,
		383, 351,
		368, 339,
		119, 208,
		104, 197,
		102, 173,
		113, 161,
		128, 159,
		141, 164,
		152, 175,
		160, 199
	};

	tunnel_1 = App->physics->CreateChain(0, 0, Tunnel_1, 48, "static");


	// Pivot 0, 0
	int Tunnel_1_2[56] = {
		350, 408,
		356, 415,
		367, 397,
		368, 380,
		357, 358,
		83, 218,
		69, 197,
		69, 176,
		80, 151,
		98, 133,
		121, 129,
		145, 133,
		167, 145,
		181, 166,
		190, 200,
		200, 200,
		190, 160,
		173, 136,
		146, 124,
		120, 120,
		93, 126,
		71, 147,
		60, 173,
		56, 198,
		76, 224,
		350, 364,
		358, 378,
		358, 393
	};

	tunnel_1_2 = App->physics->CreateChain(0, 0, Tunnel_1_2, 56, "static");
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