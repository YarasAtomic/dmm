#include "utils.h"

bool CheckCollisionSphereTri(Vector3 spherePos, float radius, Vector3 v0, Vector3 v1, Vector3 v2, Vector3 *shiftDelta,float * edgeDistanceSquared);

bool CheckCollisionSphereMesh(Vector3 spherePos, float radius,Mesh mesh, Vector3 *shiftDelta);