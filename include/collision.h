#include "utils.h"
#include <vector>

bool CheckCollisionSphereTri(Vector3 spherePos, float radius, Vector3 v0, Vector3 v1, Vector3 v2, Vector3 *shiftDelta);

bool CheckCollisionSphereMesh(Vector3 spherePos, float radius,Mesh mesh, Vector3 *shiftDelta);

struct BSPNode {
    Plane divisionPlane;
    std::vector<Mesh> solidObjects;
    BSPNode* frontChild;
    BSPNode* backChild;
    int type = -1;
};

BSPNode* BuildBSPTree(std::vector<Vector3>& vertices);

void SplitSolidObjects(std::vector<Vector3>& vertices,Plane plane,std::vector<Vector3>& frontTris,std::vector<Vector3>& backTris);

Plane SelectDivisionPlane(std::vector<Vector3>& vertices);

int IsMeshOutsidePlane(Mesh mesh,Plane plane);

int IsPolyOutsidePlane(const std::vector<Vector3>&,Plane plane);

bool PointBSPCollision(Vector3 point,BSPNode * node);

bool PointBSPCollision(Vector3 point,BSPNode * node,Vector3 * shiftDelta,float * distance,int depth);

bool BSPRay(BSPNode * node,Vector3 p0, Vector3 p1, Vector3 * intersection,Vector3 * normal);