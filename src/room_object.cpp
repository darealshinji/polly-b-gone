// -*- C++ -*-

#include "physics/vector.h"
#include "room_object.h"

using namespace mbostock;

RoomObject::~RoomObject() {
}

bool RoomObject::dynamic() const {
  return false;
}

Vector RoomObject::velocity(const Vector& x) const {
  (void)x;
  return Vector::ZERO();
}

float RoomObject::slip() const {
  return 0.f;
}

void RoomObject::resetForces() {
}

void RoomObject::applyForce(UnaryForce& force) {
  (void)force;
}

void RoomObject::applyWeight(float w, const Vector& x) {
  (void)w;
  (void)x;
}

void RoomObject::step(const ParticleSimulator& s) {
  (void)s;
}

void RoomObject::constrainInternal() {
}

void RoomObject::reset() {
}

bool DynamicRoomObject::dynamic() const {
  return true;
}
