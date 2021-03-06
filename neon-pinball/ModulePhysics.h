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
enum DIRECTION {RIGHT, LEFT};
enum COLLISION_TYPE{ 
	BALL = 0x0001,
	DEATH_SENSOR = 0x0002,
	TUNNEL_SENSOR = 0x0003,
	BOUNCER = 0x0004};

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

	PhysBody* CreateCircle(int x, int y, int radius, b2BodyType type, float restitution,uint16 category, uint16 mask);
	PhysBody* CreateBall(int x, int y, int radius, float restitution, Module* listener);
	PhysBody* CreateCircleBouncer(int x, int y);
	PhysBody* CreateRectangleBouncer(int x, int y, int width, int height, float angle);
	PhysBody* CreateRectangle(int x, int y, int width, int height, uint16 category, uint16 mask,float restitution, b2BodyType type, float angle = 420, float density = 1.0f);
	PhysBody* CreateRectangleSensor(int x, int y, int width, int height, uint16 category, uint16 mask);
	PhysBody* CreateCircularSensor(int x, int y, int radius);
	PhysBody* CreateChain(int x, int y, int* points, int size, const char* type, uint16 category, uint16 mask);
	void CreateRevoluteJointDef(b2RevoluteJointDef * def, PhysBody* A, PhysBody* B);
	PhysBody* CreatePolygon(int x, int y, int* points, int size, float density, uint16 category, uint16 mask);  //Cal revisar-lo
	b2RevoluteJoint* CreateFlipper(int x, int y, DIRECTION direction);

	// b2ContactListener ---
	void BeginContact(b2Contact* contact);

	//Mouse 
	int mouse_x;
	int mouse_y;
	b2Vec2 mouse_position;

	void getMousePositionInMeters(int* mouse_x, int* mouse_y);


	b2Body *body_found = nullptr;

	bool mouse_cliked = false;

	bool DeleteBody(b2Body* body);
private:

	bool debug;
	b2World* world;
	b2MouseJoint* mouse_joint;
	b2Body* ground;
};