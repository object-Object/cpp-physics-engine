//#define DEBUG

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "objects.h"
#include "physics.h"
#include "math.hpp"
#include <iostream>
#include <vector>
#include <cmath>

const unsigned int DEFAULT_WINDOW_WIDTH = 800;
const unsigned int DEFAULT_WINDOW_HEIGHT = 800;
const float DEFAULT_VIEW_WIDTH = 40.f;
const float DEFAULT_VIEW_HEIGHT = 40.f;
const float VECTOR_SCALE = 0.5f;
const float ARROW_KEY_MOVE = 1.f;

const unsigned int FPS = 60;
const float PHYSICS_TIME_STEP = 1.f / 120.f;
const sf::Vector2f GRAVITY(0, 0);

void drawArrow(sf::RenderWindow& window, sf::Vector2f start, sf::Vector2f end, float lineScale, float viewScale, sf::Color color);
void drawArrow2(sf::RenderWindow& window, sf::Vector2f start, sf::Vector2f direction, float lineScale, float viewScale, sf::Color color);

int main() {
	phys::World world(GRAVITY);
	world.addObject(std::make_shared<Circle>(sf::Vector2f(10.f, 35.f), sf::Vector2f(), 1.f, 0.5f));
	world.addObject(std::make_shared<Circle>(sf::Vector2f(20.f, 35.f), sf::Vector2f(0, 5.f), 10.f, 0.5f));
	world.addObject(std::make_shared<Circle>(sf::Vector2f(5.f, 5.f), sf::Vector2f(10.f, 10.f), 5.f, 0.5f));

	float viewScale = DEFAULT_VIEW_HEIGHT / DEFAULT_WINDOW_HEIGHT;

	bool isPaused = false;

	sf::RenderWindow window(sf::VideoMode(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT), "Physics");
	sf::View view(sf::FloatRect(0, DEFAULT_VIEW_HEIGHT, DEFAULT_VIEW_WIDTH, -DEFAULT_VIEW_HEIGHT));
	window.setView(view);
	window.setFramerateLimit(FPS);
	window.setKeyRepeatEnabled(false);

	sf::Clock clock;
	float frameTimeCarry = 0;
	sf::Vector2i oldMousePos;
	bool movingView = false;
	while (window.isOpen()) {
		// events
		sf::Event event;
		while (window.pollEvent(event)) {
			switch (event.type) {
			case sf::Event::Closed: {
				window.close();
			} break;
			case sf::Event::Resized: {
				view.setSize(sf::Vector2f((float)event.size.width, -(float)event.size.height) * viewScale);
				window.setView(view);
			} break;
			case sf::Event::MouseWheelScrolled: {
				if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
					float zoom = event.mouseWheelScroll.delta > 0 ? 0.9f : 1.1f;
					view.zoom(zoom);
					window.setView(view);
					viewScale *= zoom;
				}
			} break;
			case sf::Event::KeyPressed: { // key presses that should only trigger once per press, even if held
				switch (event.key.code) {
				case sf::Keyboard::Escape: {
					isPaused = !isPaused;
					if (!isPaused) {
						window.setTitle("Physics");
						clock.restart(); // if it unpaused, restart the clock so physics doesn't jump ahead by however long it was paused
					} else {
						window.setTitle("Physics (paused)");
					}
				} break;
				default:
					break;
				}
			} break;
			case sf::Event::MouseButtonPressed: {
				if (event.mouseButton.button == sf::Mouse::Middle) {
					oldMousePos = sf::Mouse::getPosition();
					movingView = true;
				}
			} break;
			case sf::Event::MouseButtonReleased: {
				if (event.mouseButton.button == sf::Mouse::Middle) {
					movingView = false;
				}
			} break;
			case sf::Event::MouseMoved: {
				if (movingView) {
					sf::Vector2i newMousePos = sf::Mouse::getPosition();
					float x = (newMousePos.x - oldMousePos.x) * viewScale;
					float y = (newMousePos.y - oldMousePos.y) * viewScale;
					view.move(-x, y);
					window.setView(view);
					oldMousePos = newMousePos;
				}
			} break;
			default:
				break;
			}
		}

		// key presses that need to support holding down or multiple keys
		if (window.hasFocus()) {
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) { // up
				view.move(0, ARROW_KEY_MOVE);
				window.setView(view);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) { // down
				view.move(0, -ARROW_KEY_MOVE);
				window.setView(view);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) { // left
				view.move(-ARROW_KEY_MOVE, 0);
				window.setView(view);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) { // right
				view.move(ARROW_KEY_MOVE, 0);
				window.setView(view);
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) && sf::Keyboard::isKeyPressed(sf::Keyboard::C)) { // ctrl+c
				window.close();
			}
		}

		// do physics
		// using constant time step, tick as many times as possible within frame, then carry the remaining time to the next frame
		if (!isPaused) {
			world.clearObjectForces();
			frameTimeCarry = world.step(clock.restart().asSeconds() + frameTimeCarry, PHYSICS_TIME_STEP);
		}

		// draw everything
		window.clear(sf::Color::White);
		world.draw(window);
		// draw force and velocity vectors
		for (const std::shared_ptr<Object> object : world.getObjects()) {
			const sf::Vector2f position = object->getPosition();
			for (const sf::Vector2f force : object->getForces()) { // individual forces (blue)
				drawArrow(window, position, force, VECTOR_SCALE, viewScale, sf::Color::Blue);
			}
			drawArrow(window, position, object->getNetForce(), VECTOR_SCALE, viewScale, sf::Color::Red); // net force (red)
			drawArrow(window, position, object->getVelocity(), VECTOR_SCALE, viewScale, sf::Color(33, 156, 65, 255)); // velocity (green)
		}
		window.display();
	}

	return 0;
}

// triangle is drawn at the end, scaled to the view
void drawArrow(sf::RenderWindow& window, sf::Vector2f start, sf::Vector2f direction, float lineScale, float viewScale, sf::Color color) {
	direction *= lineScale;

	sf::Vertex line[] = { start, start + direction };
	line[0].color = color;
	line[1].color = color;

	float triangleRadius = 5.f * viewScale;
	sf::CircleShape triangle(triangleRadius, 3);
	triangle.setOrigin(triangleRadius, triangleRadius);
	triangle.setRotation(radToDeg(std::atan2(direction.y, direction.x))+90);
	triangle.setPosition(start + direction - Vector2_normalize(direction) * (triangleRadius / 2.f)); // put triangle half radius of triangle before the end of the arrow
	triangle.setFillColor(color);

	window.draw(line, 2, sf::Lines);
	window.draw(triangle);
}

void drawArrow2(sf::RenderWindow& window, sf::Vector2f start, sf::Vector2f end, float lineScale, float viewScale, sf::Color color) {
	drawArrow(window, start, end - start, lineScale, viewScale, color);
}