#include "util.h"
#include <cmath>
#include <ios>

void Scene::update(float dt) {}

Vector2& Vector2::operator+=(const Vector2& b) {
    x += b.x;
    y += b.y;
    return *this;
}

Vector2& Vector2::operator+=(float b) {
    x += b;
    y += b;
    return *this;
}

Vector2& Vector2::operator-=(const Vector2& b) {
    x -= b.x;
    y -= b.y;
    return *this;
}

Vector2& Vector2::operator-=(float b) {
    x -= b;
    y -= b;
    return *this;
}

Vector2& Vector2::operator*=(const Vector2& b) {
    x *= b.x;
    y *= b.y;
    return *this;
}

Vector2& Vector2::operator*=(float b) {
    x *= b;
    y *= b;
    return *this;
}

Vector2& Vector2::operator/=(const Vector2& b) {
    x /= b.x;
    y /= b.y;
    return *this;
}

Vector2& Vector2::operator/=(float b) {
    x /= b;
    y /= b;
    return *this;
}

Vector2 Vector2::operator+(const Vector2& b) const {
    Vector2 copy(*this);
    copy += b;
    return copy;
}

Vector2 Vector2::operator+(float b) const {
    Vector2 copy(*this);
    copy += b;
    return copy;
}

Vector2 Vector2::operator-(const Vector2& b) const {
    Vector2 copy(*this);
    copy -= b;
    return copy;
}

Vector2 Vector2::operator-(float b) const {
    Vector2 copy(*this);
    copy -= b;
    return copy;
}

Vector2 Vector2::operator*(const Vector2& b) const {
    Vector2 copy(*this);
    copy *= b;
    return copy;
}

Vector2 Vector2::operator*(float b) const {
    Vector2 copy(*this);
    copy *= b;
    return copy;
}

Vector2 Vector2::operator/(const Vector2& b) const {
    Vector2 copy(*this);
    copy /= b;
    return copy;
}

Vector2 Vector2::operator/(float b) const {
    Vector2 copy(*this);
    copy /= b;
    return copy;
}

float Vector2::dot(const Vector2& a, const Vector2& b) {
    return a.x * b.x + a.y * b.y;
}

float Vector2::dot(const Vector2& b) const {
    return dot(*this, b);
}

float Vector2::distance(const Vector2& a, const Vector2& b) {
    return std::sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
}

float Vector2::distance(const Vector2& b) const {
    return distance(*this, b);
}
