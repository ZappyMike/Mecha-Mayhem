#pragma once
#include "Player.h"
#include "Utilities/Time.h"

class Spawner {
public:
	Spawner() {}

	Spawner& Init(float radius, float delay) {
		m_spawnerModel.LoadMeshs("spawner/spawner", true);
		m_radius = radius;
		m_timer = m_delay = delay;
		m_spawnerModel.SetDirection(false);

		return *this;
	}

	void SetBounds(int lower, int upper) {
		if ((lower > upper) || (lower < 0))	return;
		lowerBound = lower;
		upperBound = upper - lower + 1;
	}

	void Render(glm::mat4 model) {
		
		if (m_timer == 0) {
			if (m_currWeapon == Player::WEAPON::FIST)
				Player::GetOffhandModel(Player::OFFHAND::HEALPACK2).Draw(model * m_gunOffset);
			else			Player::GetWeaponModel(m_currWeapon).Draw(model * m_gunOffset);
		}

		m_spawnerModel.Draw(model);
	}

	//return true if colelcted
	void Update(entt::registry* reg, glm::vec3 pos) {
		m_spawnerModel.Update(Time::dt);
		if (m_timer > 0) {
			m_timer -= Time::dt;
			if (m_timer <= 0) {
				m_timer = 0;
				m_spawnerModel.SetDirection(true);
				//decide on what spawns
				m_currWeapon = Player::WEAPON(lowerBound + rand() % upperBound);
			}
			return;
		}

		Player* touched = nullptr;
		float distance = 100;
		reg->view<Player, PhysBody>().each([&](Player& p, PhysBody& body) {
			if (p.IsPlayer() && p.IsAlive()) {
				if (body.TestAABB(pos + BLM::GLMup, m_radius)) {
					float temp = glm::length(pos - BLM::BTtoGLM(body.GetTransform().getOrigin()));
					if (temp <= distance) {
						distance = temp;
						touched = &p;
					}
				}
			}
		});

		if (touched != nullptr) {
			if (m_currWeapon == Player::WEAPON::FIST) {
				if (touched->PickUpOffhand(Player::OFFHAND::HEALPACK2)) {
					m_timer = m_delay;
					m_spawnerModel.SetDirection(false);
				}
			}
			else if (m_currWeapon == Player::WEAPON::SWORD) {
				if (touched->PickUpSword()) {
					m_timer = m_delay;
					m_spawnerModel.SetDirection(false);
				}
			}
			else if (touched->PickUpWeapon(m_currWeapon)) {
				m_timer = m_delay;
				m_spawnerModel.SetDirection(false);
			}
		}
	}

private:
	static const glm::mat4 m_gunOffset;

	ObjMorphLoader m_spawnerModel = {};

	//change to int later
	Player::WEAPON m_currWeapon = Player::WEAPON::FIST;

	int lowerBound = 0;
	int upperBound = 6;

	float m_radius = 0.5f;
	float m_timer = 0;
	float m_delay = 5;
};

inline const glm::mat4 Spawner::m_gunOffset = glm::mat4(
	1, 0, 0, 0,
	0, 1, 0, 0,
	0, 0, 1, 0,
	0, 1, 0, 1
);
