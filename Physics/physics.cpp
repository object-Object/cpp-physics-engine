#include <memory>
#include <SFML/Graphics.hpp>
#include "physics.h"
#include "objects.h"

phys::World::World() {
	m_gravity = sf::Vector2f(); // default: zero gravity
}

phys::World::World(sf::Vector2f gravity) {
	m_gravity = gravity;
}

void phys::World::clearObjectForces() {
	for (std::shared_ptr<Object> object : m_objects) {
		object->clearForces();
	}
}

// assumes forces have been cleared since the last time step was called
// it might be a good idea to add a maximum number of steps eventually
float phys::World::step(float frameTime, float stepTime) {
	for (std::shared_ptr<Object> object : m_objects) {
		object->addForce(m_gravity * object->getMass());
	}
	while (frameTime >= stepTime) {
		// apply forces, update velocities and positions
		for (std::shared_ptr<Object> object : m_objects) {
			sf::Vector2f force = object->getNetForce();
			sf::Vector2f acceleration(force.x / object->getMass(), force.y / object->getMass());
			object->addVelocity(acceleration * stepTime);
			object->addPosition(object->getVelocity() * stepTime);
		}

		// detect collisions
		// broad phase


		// narrow phase


		// solve constraints


		frameTime -= stepTime;
	}
	return stepTime; // return leftover time
}

void phys::World::addObject(std::shared_ptr<Object> object) {
	m_objects.push_back(object);
}

const std::vector<std::shared_ptr<Object>>& phys::World::getObjects() const {
	return m_objects;
}

void phys::World::setGravity(sf::Vector2f gravity) {
	m_gravity = gravity;
}

void phys::World::draw(sf::RenderWindow& window) {
	for (std::shared_ptr<Object> object : m_objects) {
		object->draw(window);
	}
}