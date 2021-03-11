#include "Rendering.h"

namespace Rendering {

    void Update(entt::registry* reg)
    {
        glClearColor(BackColour.x, BackColour.y, BackColour.z, BackColour.w);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        auto objView = reg->view<ObjLoader, Transform>();
        auto cameraView = reg->view<Camera, Transform>();

        for (auto cam : cameraView)
        {
            cameraView.get<Camera>(cam).SetPosition(cameraView.get<Transform>(cam).GetPosition());
            cameraView.get<Camera>(cam).SetRotation(cameraView.get<Transform>(cam).GetRotation());

            Camera& camCam = cameraView.get<Camera>(cam);

            LightPos = cameraView.get<Transform>(cam).GetPosition();

            //draw all the objs
            for (auto entity : objView)
            {
                Transform& trans = objView.get<Transform>(entity);

                objView.get<ObjLoader>(entity).Draw(camCam, trans.GetModel(), trans.GetRotationM3(),
                    DefaultColour, LightPos, LightColour, 100.f, 4.0f, 0.23f);
            }
        }


		//draw all the objs
		/*for (auto entity : objView2)
		{
			Transform& trans = objView2.get<Transform>(entity);

			objView2.get<ObjLoader>(entity).Draw(
				cameraView.get<Camera>(cam), trans.GetModel(), trans.GetRotationM3(),
				glm::vec3(1.f), cameraView.get<Transform>(cam).GetPosition(), glm::vec3(3.f));
		}*/
    }

    glm::vec4 BackColour = { 0.2f, 0.2f, 0.2f, 1.f };
    glm::vec3 LightColour = glm::vec3(3.f);
    glm::vec3 LightPos = glm::vec3(0.f);
    glm::vec3 DefaultColour = glm::vec3(1.f);

}