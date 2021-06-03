#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <vector>
#include <memory>

struct AABB {
	sf::Vector2f min;
	sf::Vector2f max;
};

class Object {
protected:
	Object(sf::Vector2f velocity, float mass);

	sf::Vector2f m_position;
	float m_angle;
	std::shared_ptr<sf::Shape> m_shape;
	sf::Vector2f m_velocity;
	float m_mass;
	std::vector<sf::Vector2f> m_forces;

public:
	const sf::Vector2f getVelocity() const;
	void setVelocity(sf::Vector2f velocity);
	void addVelocity(sf::Vector2f delta);

	float getMass() const;
	void setMass(float mass);

	const sf::Vector2f getPosition() const;
	void setPosition(sf::Vector2f position);
	void addPosition(sf::Vector2f delta);

	float getAngle() const;
	void setAngle(float angle);
	void addAngle(float delta);

	auto getShape() const;

	void draw(sf::RenderWindow& window);

	void addForce(float x, float y);
	void addForce(sf::Vector2f force);
	const sf::Vector2f getNetForce() const;
	const std::vector<sf::Vector2f> getForces() const;
	void clearForces();
};

class Point : public Object {
public:
	Point(sf::Vector2f position, sf::Vector2f velocity, float mass);
	Point(sf::Vector2f position, sf::Vector2f velocity, float mass, sf::Color color);
};

class Circle : public Object {
public:
	Circle(sf::Vector2f position, sf::Vector2f velocity, float mass, float radius);
	Circle(sf::Vector2f position, sf::Vector2f velocity, float mass, float radius, sf::Color color);
};