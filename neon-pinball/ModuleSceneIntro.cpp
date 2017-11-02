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

	left_tunnel_up = App->physics->CreateChain(0, 0,lefttunnelup, 84, "static", BOUNCER, BALL);

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

	int rightpart[26] = {
		438, 400,
		438, 246,
		436, 195,
		428, 159,
		446, 157,
		461, 160,
		479, 166,
		494, 175,
		508, 190,
		517, 207,
		524, 227,
		520, 248,
		464, 379
	};

	right_part = App->physics->CreateChain(0, 0, rightpart, 26, "static", BOUNCER, BALL);

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

	// Pivot 0, 0
	int righttunnelsmall[8] = {
		389, 447,
		393, 451,
		440, 405,
		437, 401
	};
	small_rightpart = App->physics->CreateChain(0, 0, righttunnelsmall, 8, "static", BOUNCER, BALL);

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
	int righttunnel2[54] = {
		357, 157,
		360, 160,
		373, 148,
		388, 141,
		399, 137,
		398, 128,
		388, 113,
		380, 102,
		363, 86,
		344, 72,
		319, 61,
		294, 54,
		273, 60,
		264, 71,
		269, 72,
		272, 65,
		280, 60,
		292, 60,
		300, 61,
		324, 67,
		346, 78,
		364, 92,
		379, 108,
		388, 120,
		394, 132,
		378, 141,
		364, 150
	};

	right_tunnel_2 = App->physics->CreateChain(0, 0, righttunnel2, 54, "static", BOUNCER, BALL);

	// Pivot 0, 0
	int rightsmall2[16] = {
		407, 160,
		398, 167,
		387, 177,
		371, 188,
		367, 185,
		381, 173,
		392, 166,
		406, 159
	};

	right_small_2 = App->physics->CreateChain(0, 0, rightsmall2, 16, "static", BOUNCER, BALL);


	
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
	int Pinball[62] = {
		190, 922,
		189, 884,
		45, 817,
		45, 210,
		52, 172,
		67, 133,
		90, 97,
		126, 65,
		166, 44,
		220, 29,
		286, 32,
		324, 43,
		362, 62,
		394, 88,
		420, 131,
		431, 132,
		450, 134,
		468, 137,
		487, 144,
		504, 154,
		521, 167,
		537, 185,
		546, 205,
		551, 229,
		545, 258,
		490, 398,
		472, 443,
		439, 518,
		438, 816,
		298, 886,
		296, 921
	};


	background_.add(App->physics->CreateChain(0, 0, Pinball, 62, "static", BOUNCER, BALL));

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


	//moving chains
	// Pivot 0, 0
	int rightmove1[12] = {
		395, 136,
		409, 131,
		424, 130,
		426, 133,
		411, 134,
		400, 139
	};
	right_move_1 = App->physics->CreateChain(0, 0, rightmove1, 12, "static", BOUNCER, BALL);

	// Pivot 0, 0
	int rightmove2[12] = {
		407, 160,
		409, 165,
		421, 159,
		435, 159,
		435, 154,
		420, 155
	};

	right_move_2 = App->physics->CreateChain(0, 0, rightmove2, 12, "static", BOUNCER, BALL);

	// Pivot 0, 0
	int rightmovevertical[8] = {
		395, 137,
		406, 162,
		410, 163,
		400, 136
	};

	right_move_vertical = App->physics->CreateChain(0, 0, rightmovevertical, 8, "static", BOUNCER, BALL);

	// Pivot 0, 0
	int rightmovevertical1[8] = {
		418, 132,
		428, 158,
		436, 157,
		426, 131
	};
	right_move_vertical_1 = App->physics->CreateChain(0, 0, rightmovevertical1, 8, "static", BOUNCER, BALL);

	//tapa
	// Pivot 0, 0
	int rightblockdown[8] = {
		98, 347,
		114, 369,
		121, 366,
		103, 343
	};
	right_block_down = App->physics->CreateChain(0, 0, rightblockdown, 8, "static", BOUNCER, BALL);

	// Pivot 0, 0
	int rightblockup[8] = {
		276, 31,
		277, 60,
		285, 58,
		284, 32
	};

	right_block_up = App->physics->CreateChain(0, 0, rightblockup, 8, "static", BOUNCER, BALL);
}

void ModuleSceneIntro::SwitchColliders()
{
	p2List_item<PhysBody*>* bouncers_iterator = nullptr;


	for (bouncers_iterator = bouncers.getFirst(); bouncers_iterator; bouncers_iterator = bouncers_iterator->next) {
		bouncers_iterator->data->body->SetActive(!bouncers_iterator->data->body->IsActive());
	}
}
