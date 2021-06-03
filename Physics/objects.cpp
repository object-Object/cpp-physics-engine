#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <memory>
#include "objects.h"

const float POINT_RADIUS = 0.2f;


Object::Object(sf::Vector2f velocity, float mass) {
	m_velocity = velocity;
	m_mass = mass;
	m_angle = 0;
	m_shape = nullptr;
}

const sf::Vector2f Object::getVelocity() const {
	return m_velocity;
}

void Object::setVelocity(sf::Vector2f velocity) {
	m_velocity = velocity;
}

void Object::addVelocity(sf::Vector2f delta) {
	m_velocity += delta;
}

float Object::getMass() const {
	return m_mass;
}

void Object::setMass(float mass) {
	m_mass = mass;
}

const sf::Vector2f Object::getPosition() const {
	return m_position;
}

void Object::setPosition(sf::Vector2f position) {
	m_position = position;
}

void Object::addPosition(sf::Vector2f delta) {
	m_position += delta;
}

float Object::getAngle() const {
	return m_angle;
}

void Object::setAngle(float angle) {
	m_angle = angle;
	
}

void Object::addAngle(float delta) {
	m_angle += delta;
}

auto Object::getShape() const {
	return m_shape;
}

void Object::draw(sf::RenderWindow& window) {
	m_shape->setPosition(m_position.x, m_position.y);
	m_shape->setRotation(m_angle);
	window.draw(*m_shape);
}

void Object::addForce(float x, float y) {
	m_forces.push_back(sf::Vector2f(x, y));
}

void Object::addForce(sf::Vector2f force) {
	m_forces.push_back(force);
}

const sf::Vector2f Object::getNetForce() const {
	sf::Vector2f netForce;
	for (const sf::Vector2f force : m_forces) {
		netForce += force;
	}
	return netForce;
}

const std::vector<sf::Vector2f> Object::getForces() const {
	return m_forces;
}

void Object::clearForces() {
	m_forces.clear();
}


Point::Point(sf::Vector2f position, sf::Vector2f velocity, float mass) : Object(velocity, mass) {
	m_shape = std::make_shared<sf::CircleShape>(sf::CircleShape(POINT_RADIUS));
	m_shape->setOrigin(POINT_RADIUS, POINT_RADIUS);
	m_shape->setFillColor(sf::Color::Black);
	this->setPosition(position);
	this->setAngle(0);
}

Point::Point(sf::Vector2f position, sf::Vector2f velocity, float mass, sf::Color color) : Point(position, velocity, mass) {
	m_shape->setFillColor(color);
}


Circle::Circle(sf::Vector2f position, sf::Vector2f velocity, float mass, float radius) : Object(velocity, mass) {
	m_shape = std::make_shared<sf::CircleShape>(sf::CircleShape(radius));
	m_shape->setOrigin(radius, radius);
	m_shape->setFillColor(sf::Color::Black);
	this->setPosition(position);
	this->setAngle(0);
}

Circle::Circle(sf::Vector2f position, sf::Vector2f velocity, float mass, float radius, sf::Color color)
	: Circle(position, velocity, mass, radius) {
	m_shape->setFillColor(color);
}