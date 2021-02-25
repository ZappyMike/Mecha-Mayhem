#include "Utilities/Gameloop.h"
#include "DemoScene.h"
#include "Tutorial.h"
#include "MainMenu.h"
#include "LeaderBoard.h"
#include "MapEditorScene.h"

int main() {
	int width = 1280, height = 720;
	const bool usingImGui = false;

	GLFWwindow* window = Gameloop::Start("Mecha Mayhem", width, height, usingImGui);
	if (!window)	return 1;

	{
		// Creating demo scenes
		//Scene::m_scenes.push_back(new MapEditor("uh, not for playing"));
		Scene::m_scenes.push_back(new MainMenu("Mecha Mayhem"));
		Scene::m_scenes.push_back(new Tutorial("MM Tutorial", glm::vec3(0, -100, 0)));
		Scene::m_scenes.push_back(new DemoScene("MM Demo", glm::vec3(0, -100, 0)));
		Scene::m_scenes.push_back(new LeaderBoard("The Winner is..."));

		Scene::m_scenes[0]->Init(width, height);
		Scene::m_scenes[1]->Init(width, height);
		Scene::m_scenes[2]->Init(width, height);
		Scene::m_scenes[3]->Init(width, height);

		Scene::m_activeScene = Scene::m_scenes[0]->Reattach();
		glfwSetWindowTitle(window, Scene::m_activeScene->GetName().c_str());

		bool paused = false;

		while (!glfwWindowShouldClose(window)) {
			glfwPollEvents();

			if (Scene::GetExit()) {
				break;
			}

			if (!BackEnd::HasFocus()) {
				if (!paused) {
					AudioEngine::Instance().PauseEverything();
					paused = true;
				}
				//has to update all the time
				AudioEngine::Instance().Update();
				if (usingImGui)
					Gameloop::ImGuiWindow(window, Scene::m_activeScene);
				continue;
			}
			if (paused) {
				AudioEngine::Instance().UnPauseEverything();
				paused = false;
			}

			ControllerInput::ControllerRefresh();
			Scene::doSceneChange(window);

			Scene::m_activeScene->Update();

			//do not touch plz
			Scene::m_activeScene->BackEndUpdate();
			if (usingImGui)
				Gameloop::ImGuiWindow(window, Scene::m_activeScene);
			Gameloop::Update();

			glfwSwapBuffers(window);
		}

		Scene::m_activeScene->Exit();
	}

	Gameloop::Stop(usingImGui);
	return 0;
}