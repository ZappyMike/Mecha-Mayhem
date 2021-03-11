#include <string>
#include "BackEndThings/Rendering.h"
#include <stdlib.h> 

// Matthew McPherson (100757379) 
//			&& 
// Ryan Chang (100745877)
GLFWwindow* window;

//Collision detection function
bool AABB(glm::vec3 BotL, glm::vec3 TopR, glm::vec3 ballPosition)
{
	glm::vec3 ballPos = ballPosition;

	return(ballPos.x > BotL.x && ballPos.x < TopR.x && ballPos.y > BotL.y && ballPos.y < TopR.y
		 && ballPos.z > BotL.z && ballPos.z < TopR.z);
}

int main() {
	srand(time(0));

	//logger initialization
	Logger::Init(); 

	if (!(window = BackEnd::Init("Computer Graphics Midterm")))	return 1;

	int width = 720, height = 480;

	glfwSetWindowSize(window, width, height);

	ObjLoader::Init();

	entt::registry reg;

	ECS::AttachRegistry(&reg);
	
	// create entities
	unsigned cameraEnt = ECS::CreateEntity();
	auto& camCam = ECS::AttachComponent<Camera>(cameraEnt);
	camCam.SetFovDegrees(60.f);

	unsigned paddle = ECS::CreateEntity();
	ECS::AttachComponent<ObjLoader>(paddle).LoadMesh("models/paddle2.obj", true);
	ECS::GetComponent<Transform>(paddle).SetPosition(glm::vec3(0, -8, 15));
	ECS::GetComponent<Transform>(paddle).SetRotation(glm::vec3(0, glm::radians(90.f), 0));
	ECS::GetComponent<Transform>(paddle).SetScale(glm::vec3(0.5f, 0.5f, 1.0f));

	unsigned ball = ECS::CreateEntity();
	ECS::AttachComponent<ObjLoader>(ball).LoadMesh("models/ball.obj", true);
	ECS::GetComponent<Transform>(ball).SetPosition(glm::vec3(0, -3, 15));
	ECS::GetComponent<Transform>(ball).SetScale(glm::vec3(0.3f));

	unsigned walltop = ECS::CreateEntity();
	ECS::AttachComponent<ObjLoader>(walltop).LoadMesh("models/walls.obj", true);
	ECS::GetComponent<Transform>(walltop).SetPosition(glm::vec3(11, 5, 15));
	ECS::GetComponent<Transform>(walltop).SetRotation(glm::vec3(0,0,glm::radians(90.f)));
	ECS::GetComponent<Transform>(walltop).SetScale(glm::vec3(0.5f));

	unsigned wallleft = ECS::CreateEntity();
	ECS::AttachComponent<ObjLoader>(wallleft).LoadMesh("models/walls.obj", true);
	ECS::GetComponent<Transform>(wallleft).SetPosition(glm::vec3(11, -10, 15));
	ECS::GetComponent<Transform>(wallleft).SetScale(glm::vec3(0.5f));

	unsigned wallright = ECS::CreateEntity();
	ECS::AttachComponent<ObjLoader>(wallright).LoadMesh("models/walls.obj", true);
	ECS::GetComponent<Transform>(wallright).SetPosition(glm::vec3(-11, -10, 15));
	ECS::GetComponent<Transform>(wallright).SetScale(glm::vec3(0.5f));

	std::vector<unsigned> bricksweak = {};
	std::vector<unsigned> bricksweak2 = {};
	std::vector<unsigned> bricksStrong = {};

	for (int i = 0; i < 20; ++i)
	{
		bricksweak.push_back(ECS::CreateEntity());
		ECS::AttachComponent<ObjLoader>(bricksweak[i]).LoadMesh("models/bricks.obj", true);
		ECS::GetComponent<Transform>(bricksweak[i]).SetScale(glm::vec3(0.5f));
		
		if(i < 10)
			ECS::GetComponent<Transform>(bricksweak[i]).SetPosition(glm::vec3(2*i - 9, 0, 15));
		else
			ECS::GetComponent<Transform>(bricksweak[i]).SetPosition(glm::vec3(2*i -29, 1, 15));
	}
	
	for (int i = 0; i < 20; ++i)
	{
		bricksStrong.push_back(ECS::CreateEntity());
		ECS::AttachComponent<ObjLoader>(bricksStrong[i]).LoadMesh("models/brick2.obj", true);
		ECS::GetComponent<Transform>(bricksStrong[i]).SetScale(glm::vec3(0.5f));

		if (i < 10)
			ECS::GetComponent<Transform>(bricksStrong[i]).SetPosition(glm::vec3(2 * i - 9, 2, 15));
		else
			ECS::GetComponent<Transform>(bricksStrong[i]).SetPosition(glm::vec3(2 * i - 29, 3, 15));
	}

	for (int i = 0; i < 20; ++i)
	{
		bricksweak2.push_back(ECS::CreateEntity());
	}

	unsigned Score = ECS::CreateEntity();
	ECS::AttachComponent<ObjLoader>(Score).LoadMesh("models/score2.obj", true);
	ECS::GetComponent<Transform>(Score).SetPosition(glm::vec3(8, 6, 15));
	ECS::GetComponent<Transform>(Score).SetRotation(glm::vec3(glm::radians(90.f), glm::radians(180.f), 0));
	ECS::GetComponent<Transform>(Score).SetScale(glm::vec3(2.f));

	unsigned Lives = ECS::CreateEntity();
	ECS::AttachComponent<ObjLoader>(Lives).LoadMesh("models/lives.obj", true);
	ECS::GetComponent<Transform>(Lives).SetPosition(glm::vec3(-2, 6, 15));
	ECS::GetComponent<Transform>(Lives).SetRotation(glm::vec3(glm::radians(90.f), glm::radians(180.f), 0));
	ECS::GetComponent<Transform>(Lives).SetScale(glm::vec3(2.f));

	unsigned scorenumber = ECS::CreateEntity();
	ECS::AttachComponent<ObjLoader>(scorenumber).LoadMesh("models/0.obj", true);
	ECS::GetComponent<Transform>(scorenumber).SetPosition(glm::vec3(1.5, 6, 15));
	ECS::GetComponent<Transform>(scorenumber).SetRotation(glm::vec3(glm::radians(90.f), glm::radians(180.f), 0));
	ECS::GetComponent<Transform>(scorenumber).SetScale(glm::vec3(2.f));

	unsigned scorenumber2 = ECS::CreateEntity();
	ECS::AttachComponent<ObjLoader>(scorenumber2).LoadMesh("models/0.obj", true);
	ECS::GetComponent<Transform>(scorenumber2).SetPosition(glm::vec3(2.5, 6, 15));
	ECS::GetComponent<Transform>(scorenumber2).SetRotation(glm::vec3(glm::radians(90.f), glm::radians(180.f), 0));
	ECS::GetComponent<Transform>(scorenumber2).SetScale(glm::vec3(2.f));

	unsigned lifenumber = ECS::CreateEntity();
	ECS::AttachComponent<ObjLoader>(lifenumber).LoadMesh("models/3.obj", true);
	ECS::GetComponent<Transform>(lifenumber).SetPosition(glm::vec3(-7, 6, 15));
	ECS::GetComponent<Transform>(lifenumber).SetRotation(glm::vec3(glm::radians(90.f), glm::radians(180.f), 0));
	ECS::GetComponent<Transform>(lifenumber).SetScale(glm::vec3(2.f));
	
	auto& camTrans = ECS::GetComponent<Transform>(cameraEnt);


	float lastClock = glfwGetTime();

	// rotation stuff (for testing only)
	constexpr float pi = glm::half_pi<float>() - 0.01f;
	glm::quat startQuat = glm::rotation(glm::vec3(0, 0, 1), glm::vec3(0, 1, 0));
	bool change = true;
	glm::vec2 rot = glm::vec2(0.f);

	// variables
	glm::vec3 velocity(-0.04f , 0.04f, 0.0f);
	int lives = 3;
	int score = 0;
	int score2 = 0;
	int scoretotal = 0;

	// game loop
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		float deltaTime = glfwGetTime() - lastClock;
		lastClock = glfwGetTime();

		glfwGetWindowSize(window, &width, &height);
		camCam.SetAspect(float(width) / height);

		//move ball
		ECS::GetComponent<Transform>(ball).SetPosition(ECS::GetComponent<Transform>(ball).GetPosition() + velocity);

		//collision detection
		if (AABB(glm::vec3(-12, -10, 14), glm::vec3(-10, 12.1, 16), ECS::GetComponent<Transform>(ball).GetPosition()))
			velocity.x *= -1.f;

		if (AABB(glm::vec3(10, -10, 14), glm::vec3(12, 12.1, 16), ECS::GetComponent<Transform>(ball).GetPosition()))
			velocity.x *= -1.f;

		if (AABB(glm::vec3(-12, 4, 14), glm::vec3(12, 6, 16), ECS::GetComponent<Transform>(ball).GetPosition()))
			velocity.y *= -1.f;

		if (AABB(glm::vec3(-1.5 + ECS::GetComponent<Transform>(paddle).GetPosition().x, -8.25, 12),
			glm::vec3(0 + ECS::GetComponent<Transform>(paddle).GetPosition().x, -7.75, 18),
			ECS::GetComponent<Transform>(ball).GetPosition()))
		{
			if(velocity.x > 0)
				velocity.x *= -1.f;
			velocity.y *= -1.f;
		
		}

		if (AABB(glm::vec3(0 + ECS::GetComponent<Transform>(paddle).GetPosition().x, -8.25, 12),
			glm::vec3(1.5 + ECS::GetComponent<Transform>(paddle).GetPosition().x, -7.75, 18),
			ECS::GetComponent<Transform>(ball).GetPosition()))
		{
			if (velocity.x < 0)
				velocity.x *= -1.f;
			velocity.y *= -1.f;
		}
		//brick collision and destruction + score update
		for (int i = 0; i < 20; ++i)
		{
			if (bricksweak[i] == 90009)
				continue;
			else if (i < 10)
			{
				if (AABB(glm::vec3((2 * i - 9) - 1,  -0.64, 14), 
					glm::vec3((2 * i - 9) + 1, 0.64, 16), 
					ECS::GetComponent<Transform>(ball).GetPosition()))
				{
						velocity.y *= -1;
						ECS::DestroyEntity(bricksweak[i]);
						bricksweak[i] = 90009;

						//score 
						score += 1;
						scoretotal += 1;
						if (score == 10 || score == 20 || score == 30 || score == 40)
						{
							score2 += 1;
							std::string Sscore2 = std::to_string(score2);
							score = 0;
							ECS::RemoveComponent<ObjLoader>(scorenumber2);
							ECS::AttachComponent<ObjLoader>(scorenumber2).LoadMesh("models/" + Sscore2 + ".obj");
						}
						std::string Sscore = std::to_string(score);
						ECS::RemoveComponent<ObjLoader>(scorenumber);
						ECS::AttachComponent<ObjLoader>(scorenumber).LoadMesh("models/" + Sscore + ".obj");
				}
			}
			else
			{
				if (AABB(glm::vec3((2 * i - 29) - 1, 0.36, 14),
					glm::vec3((2 * i - 29) + 1, 1.64, 16),
					ECS::GetComponent<Transform>(ball).GetPosition()))
				{
					velocity.y *= -1;
					ECS::DestroyEntity(bricksweak[i]);
					bricksweak[i] = 90009;

					//score
					score += 1;
					scoretotal += 1;
					if (score == 10 || score == 20 || score == 30 || score == 40)
					{
						score2 += 1;
						std::string Sscore2 = std::to_string(score2);
						score = 0;
						ECS::RemoveComponent<ObjLoader>(scorenumber2);
						ECS::AttachComponent<ObjLoader>(scorenumber2).LoadMesh("models/" + Sscore2 + ".obj");
					}
					std::string Sscore = std::to_string(score);
					ECS::RemoveComponent<ObjLoader>(scorenumber);
					ECS::AttachComponent<ObjLoader>(scorenumber).LoadMesh("models/" + Sscore + ".obj");
				}
			}
		}

		for (int i = 0; i < 20; ++i)
		{
			if (bricksStrong[i] == 90008)
				continue;
			else if (i < 10)
			{
				if (AABB(glm::vec3((2 * i - 9) - 1, 1.36, 14),
					glm::vec3((2 * i - 9) + 1, 2.64, 16),
					ECS::GetComponent<Transform>(ball).GetPosition()))
				{
					if (bricksStrong[i] != 90009)
					{					
						velocity.y *= -1;						
						ECS::DestroyEntity(bricksStrong[i]);
						ECS::AttachComponent<ObjLoader>(bricksweak2[i]).LoadMesh("models/bricks.obj", true);
						ECS::GetComponent<Transform>(bricksweak2[i]).SetScale(glm::vec3(0.5f));

						if (i < 10)
							ECS::GetComponent<Transform>(bricksweak2[i]).SetPosition(glm::vec3(2 * i - 9, 2, 15));
						else
							ECS::GetComponent<Transform>(bricksweak2[i]).SetPosition(glm::vec3(2 * i - 29, 3, 15));
						bricksStrong[i] = 90009;
					}
					else
					{
						velocity.y *= -1;
						ECS::DestroyEntity(bricksweak2[i]);
						bricksStrong[i] = 90008;
					}
					
					//score
					score += 1;
					scoretotal += 1;
					if (score == 10 || score == 20 || score == 30 || score == 40)
					{
						score2 += 1;
						std::string Sscore2 = std::to_string(score2);
						score = 0;
						ECS::RemoveComponent<ObjLoader>(scorenumber2);
						ECS::AttachComponent<ObjLoader>(scorenumber2).LoadMesh("models/" + Sscore2 + ".obj");
					}
					std::string Sscore = std::to_string(score);
					ECS::RemoveComponent<ObjLoader>(scorenumber);
					ECS::AttachComponent<ObjLoader>(scorenumber).LoadMesh("models/" + Sscore + ".obj");
				}
			}
			else
			{
				if (AABB(glm::vec3((2 * i - 29) - 1, 2.36, 14),
					glm::vec3((2 * i - 29) + 1, 4.64, 16),
					ECS::GetComponent<Transform>(ball).GetPosition()))
				{
					if (bricksStrong[i] != 90009)
						{
							velocity.y *= -1;
							ECS::DestroyEntity(bricksStrong[i]);
							ECS::AttachComponent<ObjLoader>(bricksweak2[i]).LoadMesh("models/bricks.obj", true);
							ECS::GetComponent<Transform>(bricksweak2[i]).SetScale(glm::vec3(0.5f));

							if (i < 10)
								ECS::GetComponent<Transform>(bricksweak2[i]).SetPosition(glm::vec3(2 * i - 9, 2, 15));
							else
								ECS::GetComponent<Transform>(bricksweak2[i]).SetPosition(glm::vec3(2 * i - 29, 3, 15));
							bricksStrong[i] = 90009;
						}
					else
						{
							velocity.y *= -1;
							ECS::DestroyEntity(bricksweak2[i]);
							bricksStrong[i] = 90008;
						}

					//score
					score += 1;
					scoretotal += 1;
					if (score == 10 || score == 20 || score == 30 || score == 40 || score == 50 || score == 60)
					{
						score2 += 1;
						std::string Sscore2 = std::to_string(score2);
						score = 0;
						ECS::RemoveComponent<ObjLoader>(scorenumber2);
						ECS::AttachComponent<ObjLoader>(scorenumber2).LoadMesh("models/" + Sscore2 + ".obj");
					}
					std::string Sscore = std::to_string(score);
					ECS::RemoveComponent<ObjLoader>(scorenumber);
					ECS::AttachComponent<ObjLoader>(scorenumber).LoadMesh("models/" + Sscore + ".obj");
				}
			}
		}

		// reset ball position and velocity
		if (ECS::GetComponent<Transform>(ball).GetPosition().y < -15)
		{
			ECS::GetComponent<Transform>(ball).SetPosition(glm::vec3(0, -3, 15));
			velocity = { -0.04f , -0.04f, 0.0f };
			lives -= 1;
			if (lives < 1 || scoretotal == 60)
			exit(EXIT_FAILURE);

			std::string Slives = std::to_string(lives);
			ECS::RemoveComponent<ObjLoader>(lifenumber);
			ECS::AttachComponent<ObjLoader>(lifenumber).LoadMesh("models/" + Slives + ".obj");
			
		}
		
		// rotation stuff (only for testing)
		if (rot.x > pi) 
		{
			rot.x = pi;
		}

		else if (rot.x < -pi) 
		{
			rot.x = -pi;
		}

		if (change) 
		{
			glm::quat rotf = glm::rotate(startQuat, rot.x, glm::vec3(1, 0, 0));
			rotf = glm::rotate(rotf, rot.y, glm::vec3(0, 1, 0));
			camTrans.SetRotation(rotf);
			change = false;
		}
		
		//move paddle
		glm::vec3 pos2 = glm::vec3(0.f);

		if (glfwGetKey(window, GLFW_KEY_LEFT)) 
		{
			pos2.x += 12 * deltaTime;
		}
		if (glfwGetKey(window, GLFW_KEY_RIGHT)) 
		{
			pos2.x -= 12 * deltaTime;
		}

		ECS::GetComponent<Transform>(paddle).SetPosition(ECS::GetComponent<Transform>(paddle).GetPosition() + pos2);

		if (ECS::GetComponent<Transform>(paddle).GetPosition().x > 9.f)
			ECS::GetComponent<Transform>(paddle).SetPosition(glm::vec3(9.f, -8, 15));

		if (ECS::GetComponent<Transform>(paddle).GetPosition().x < -9.f)
			ECS::GetComponent<Transform>(paddle).SetPosition(glm::vec3(-9.f, -8, 15));

		Rendering::Update(&reg);
		glfwSwapBuffers(window);
	}

	Shader::UnBind();
	VertexArrayObject::UnBind();

	//memory stuff
	Logger::Uninitialize();
	return 0;
} 
