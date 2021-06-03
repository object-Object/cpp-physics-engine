#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "objects.h"

namespace phys {
	class World {
		sf::Vector2f m_gravity;
		std::vector<std::shared_ptr<Object>> m_objects;

	public:
		World();
		World(sf::Vector2f gravity);

		float step(float frameTime, float stepTime);

		void addObject(std::shared_ptr<Object> object);
		const std::vector<std::shared_ptr<Object>>& getObjects() const;
		void clearObjectForces();

		void setGravity(sf::Vector2f gravity);

		void draw(sf::RenderWindow& window);
	};
}