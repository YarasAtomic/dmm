#include <raylib.h>
#include <math.h>
#include <string>

struct Plane
{
    Vector3 point;
    Vector3 normal;
};

float Vector3DotProduct(Vector3 v1, Vector3 v2);

Vector3 Vector3Subtract(Vector3 v1, Vector3 v2);

Vector3 Vector3CrossProduct(Vector3 v1, Vector3 v2);

Vector3 Vector3Normalized(Vector3 vector);

float Vector3Length(Vector3 vector);

Vector2 Vector3Project(Vector3 point, Vector3 planeX, Vector3 planeY);

Vector3 Vector3ScalarProduct(Vector3 v, float n);

bool isPointInside(Vector2 * poly,int pCount, Vector2 point);

bool isPointLeftOf(Vector2 a,Vector2 b,Vector2 p);

bool simpleRaycastSphere(Vector3 o, Vector3 d, Vector3 so, float radius2);

std::string Vector3String(Vector3 v);

std::string Vector2String(Vector2 v);

Matrix MatrixProduct(Matrix m0, Matrix m1);

Quaternion QuaternionNormalized(Quaternion q);

std::string MatrixString(Matrix m);

Matrix QuaternionMatrix(Quaternion);

Quaternion VectorAngleQuaternion(Vector3 v, float angle);

void ScaleMatrix(Matrix * m,Vector3 v);

Quaternion QuaternionProduct(Quaternion q0, Quaternion q1);

std::string QuaternionString(Quaternion q);

float Vector3LengthSquared(Vector3 vector);