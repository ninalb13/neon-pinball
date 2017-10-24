#pragma once
#include "Module.h"
#include "Globals.h"
#include "Box2D/Box2D/Box2D.h"

#define GRAVITY_X 0.0f
#define GRAVITY_Y -7.0f

#define PIXELS_PER_METER 50.0f // if touched change METER_PER_PIXEL too
#define METER_PER_PIXEL 0.02f // this is 1 / PIXELS_PER_METER !

#define METERS_TO_PIXELS(m) ((int) floor(PIXELS_PER_METER * m))
#define PIXEL_TO_METERS(p)  ((float) METER_PER_PIXEL * p)

// Small class to return to other modules to track position and rotation of physics bodies
enum FLIPPER_DIRECTION {FLIPPER_RIGHT, FLIPPER_LEFT};
class PhysBody
{
public:
	PhysBody() : listener(NULL), body(NULL)
	{}

	void GetPosition(int& x, int &y) const;
	float GetRotation() const;
	bool Contains(int x, int y) const;
	int RayCast(int x1, int y1, int x2, int y2, float& normal_x, float& normal_y) const;

public:
	int width, height;
	b2Body* body;
	Module* listener;
};

// Module --------------------------------------
class ModulePhysics : public Module, public b2ContactListener // TODO
{
public:
	ModulePhysics(Application* app, bool start_enabled = true);
	~ModulePhysics();

	bool Start();
	update_status PreUpdate();
	update_status PostUpdate();
	bool CleanUp();

	PhysBody* CreateCircle(int x, int y, int radius, b2BodyType type);
	PhysBody* CreateRectangle(int x, int y, int width, int height);
	PhysBody* CreateRectangleSensor(int x, int y, int width, int height);
	PhysBody* CreateChain(int x, int y, int* points, int size, const char* type);
	void CreateRevoluteJointDef(b2RevoluteJointDef * def, PhysBody* A, PhysBody* B, int reference_angle, int lower_angle, int upper_angle);
	PhysBody* CreatePolygon(int x, int y, int* points, int size, float density, uint16 category, uint16 mask);  //Cal revisar-lo
	b2RevoluteJoint* CreateFlipper(FLIPPER_DIRECTION direction);

	// b2ContactListener ---
	void BeginContact(b2Contact* contact);

	//Mouse 
	int mouse_x;
	int mouse_y;
	b2Vec2 mouse_position;

	void getMousePositionInMeters(int* mouse_x, int* mouse_y);


	b2Body *body_found = nullptr;

	bool mouse_cliked = false;


private:

	bool debug;
	b2World* world;
	b2MouseJoint* mouse_joint;
	b2Body* ground;
};