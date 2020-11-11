#pragma once
#include "Rendering.h"
#include "Components/PhysBody.h"
#include "BLM.h"
#include "Input.h"

class Scene
{
public:
	Scene(const std::string& name, const glm::vec3& gravity = glm::vec3(0.f), bool physics = true);
	~Scene();

	entt::registry* GetRegistry();
	std::string GetName();

	static void Init(bool causeOverload = true);
	static void Unload(bool causeOverload = true);

	virtual void Init(int windowWidth, int windowHeight);

	virtual void Update() {}
	void BackEndUpdate();

protected:
	std::string m_name;
	float m_dt = 0;
	float m_lastClock = 0;
	short m_camCount = 1;

	entt::registry m_reg;
	btDiscreteDynamicsWorld *m_world = nullptr;

	static btDbvtBroadphase *_broadphase;
	static btCollisionConfiguration *_collisionConfiguration;
	static btCollisionDispatcher *_dispatcher;
	static btSequentialImpulseConstraintSolver *_solver;
};

