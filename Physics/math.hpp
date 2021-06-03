#pragma once

#include <SFML/System.hpp>
#include <cmath>

const double PI = 3.141592653589793;

template <typename T> T degToRad(T deg);
template <typename T> T radToDeg(T rad);
template <class T> auto Vector2_length(T v);
template <class T> T Vector2_normalize(T v);
template <class T> auto Vector2_dot(T v1, T v2);
template <class T> auto Vector2_cross(T v1, T v2);

template <class T>
T degToRad(T deg) {
	return deg * (T)(PI / 180);
}

template <class T>
T radToDeg(T rad) {
	return rad * (T)(180 / PI);
}

template <class T>
auto Vector2_length(T v) {
	return sqrt(v.x * v.x + v.y * v.y);
}

template <class T>
T Vector2_normalize(T v) {
	return v / Vector2_length(v);
}

template <class T>
auto Vector2_dot(T v1, T v2) {
	return v1.x * v2.x + v1.y * v2.y;
}

template <class T>
auto Vector2_cross(T v1, T v2) {
	return v1.x * v2.y - v1.y * v2.x;
}