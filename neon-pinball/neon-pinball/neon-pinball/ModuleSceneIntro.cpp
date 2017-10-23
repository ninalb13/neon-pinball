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

	App->physics->CreateCircle(100, 100, 50);

	int left_kicker_vertices[8] = {
		0, 7,
		0, -7,
		48, -2,
		48, 2
	};
	
	PhysBody* left_kicker = App->physics->CreatePolygon(180, 768, left_kicker_vertices, 8, 1.0f, 0x0001, 0x0004);
	PhysBody* B = App->physics->CreateCircle(180, 768, 7);
	b2RevoluteJoint* rev_joint_left = App->physics->CreatePaddle(left_kicker, B, 0, 0, -5, -15, 15);

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
	// Pivot 0, 0
	int Pinball[36] = {
		31,751,
		28,232,
		36,184,
		55,144,
		83,108,
		129,70,
		189,41,
		266,30,
		346,41,
		396,67,
		426,89,
		450,138,
		457,200,
		461,765,
		309,875,
		308,875,
		181,948,
		180,875
	};

	background_.add(App->physics->CreateChain(0, 0, Pinball, 36, "static"));
}