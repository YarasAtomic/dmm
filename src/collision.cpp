#include "collision.h"
#include <iostream>

bool CheckCollisionSphereTri(Vector3 spherePos, float radius, Vector3 v0, Vector3 v1, Vector3 v2, Vector3 *shiftDelta)
{
    // spherePos.x = spherePos.x+shiftDelta->x;
    // spherePos.y = spherePos.y+shiftDelta->y;
    // spherePos.z = spherePos.z+shiftDelta->z;

    bool collide = false;

    float squaredRadius = radius * radius;

    Vector3 edge0 = Vector3Subtract(v1, v0);
    Vector3 edge1 = Vector3Subtract(v2, v1);
    Vector3 edge2 = Vector3Subtract(v0, v2);

    Vector3 h = Vector3Subtract(spherePos, v0);

    Vector3 normal = Vector3Normalized(Vector3CrossProduct(edge0, edge1));

    // std::cout << "a "<< radius << std::endl;
    // std::cout << "Plane " << Vector3String(planeX)  << " " << Vector3String(planeY) << std::endl;
    // std::cout << "Projected point " << Vector2String(planePos2D) << " from " << Vector3String(spherePos) << std::endl;
    // std::cout << "Projected face " << Vector2String(triangle2D[0]) <<  Vector2String(triangle2D[1]) << Vector2String(triangle2D[2]) << std::endl;
    // std::cout << "Vertices " << Vector3String(v0) <<  Vector3String(v1) << Vector3String(v2) << std::endl;

    float distance = Vector3DotProduct(normal,h);

    if(abs(distance)>radius) return false;

    // std::cout << "b" << std::endl;

    Vector3 planeX = Vector3Normalized(edge0);
    Vector3 planeY = Vector3Normalized(Vector3CrossProduct(normal,edge0));

    Vector2 planePos2D = Vector3Project(spherePos,planeX,planeY);
    Vector2 triangle2D[3] = {Vector3Project(v0,planeX,planeY), Vector3Project(v1,planeX,planeY), Vector3Project(v2,planeX,planeY)};

    if(isPointInside(triangle2D,3,planePos2D)) collide = true;

    // std::cout << "c" << std::endl;
    // std::cout << "Plane " << Vector3String(planeX)  << " " << Vector3String(planeY) << std::endl;
    // std::cout << "Projected point " << Vector2String(planePos2D) << " from " << Vector3String(spherePos) << std::endl;
    // std::cout << "Projected face " << Vector2String(triangle2D[0]) <<  Vector2String(triangle2D[1]) << Vector2String(triangle2D[2]) << std::endl;
    // std::cout << "Vertices " << Vector3String(v0) <<  Vector3String(v1) << Vector3String(v2) << std::endl;



    float v0LengthSquared = Vector3Length(Vector3Subtract(v0,spherePos));
    v0LengthSquared *= v0LengthSquared;

    float v1LengthSquared = Vector3Length(Vector3Subtract(v1,spherePos));
    v0LengthSquared *= v0LengthSquared;

    float v2LengthSquared = Vector3Length(Vector3Subtract(v2,spherePos));
    v0LengthSquared *= v0LengthSquared;

    bool outsideV0 = (v0LengthSquared > squaredRadius);
    bool outsideV1 = (v1LengthSquared > squaredRadius);
    bool outsideV2 = (v2LengthSquared > squaredRadius);

    if(!(outsideV0&&outsideV1&&outsideV2)) collide =  true;

    if(!(
        !simpleRaycastSphere(v0,edge0,spherePos,squaredRadius) &&
        !simpleRaycastSphere(v1,edge1,spherePos,squaredRadius) &&
        !simpleRaycastSphere(v2,edge2,spherePos,squaredRadius))) 
    {
        collide = true;
    }

    if(collide)
        *shiftDelta = {shiftDelta->x+(normal.x*(radius-distance)),shiftDelta->y+(normal.y*(radius-distance)),shiftDelta->z+(normal.z*(radius-distance))};

    return collide;
}

bool CheckCollisionSphereMesh(Vector3 spherePos, float radius,Mesh mesh, Vector3 *shiftDelta)
{
    // (*shiftDelta) = {INFINITY,INFINITY,INFINITY};
    (*shiftDelta) = {0,0,0};

    bool hasIndex = false; // TODO

    unsigned int nCollisions = 0;

    bool collide = false;
    // Does the mesh use indexed vertices?
    if(hasIndex)
    {
        for(int i = 0; i < mesh.triangleCount; ++i)
        {
            Vector3 v0;
            Vector3 v1;
            Vector3 v2;

            v0.x = mesh.vertices[mesh.indices[i*3]*3];
            v0.y = mesh.vertices[mesh.indices[i*3]*3+1];
            v0.z = mesh.vertices[mesh.indices[i*3]*3+2];
        
            v1.x = mesh.vertices[mesh.indices[i*3+1]*3];
            v1.y = mesh.vertices[mesh.indices[i*3+1]*3+1];
            v1.z = mesh.vertices[mesh.indices[i*3+1]*3+2];
            
            v2.x = mesh.vertices[mesh.indices[i*3+2]*3];
            v2.y = mesh.vertices[mesh.indices[i*3+2]*3+1];
            v2.z = mesh.vertices[mesh.indices[i*3+2]*3+2];

            // collide = CheckCollisionSphereTri(spherePos,radius,v0,v1,v2);
        }
    }
    else
    {
        Vector3 equidistantLine = {0,0,0};
        Vector3 lastNormal = {0,0,0};

        Vector3 currentDelta = {0,0,0};

        for(int i = 0; i < mesh.vertexCount; i+=3)
        {
            Vector3 v0;
            Vector3 v1;
            Vector3 v2;

            v0.x = mesh.vertices[i*3];
            v0.y = mesh.vertices[i*3+1];
            v0.z = mesh.vertices[i*3+2];
        
            v1.x = mesh.vertices[i*3+3];
            v1.y = mesh.vertices[i*3+4];
            v1.z = mesh.vertices[i*3+5];
            
            v2.x = mesh.vertices[i*3+6];
            v2.y = mesh.vertices[i*3+7];
            v2.z = mesh.vertices[i*3+8];

            if(CheckCollisionSphereTri(spherePos,radius,v0,v1,v2,shiftDelta))
            {
                // if(Vector3LengthSquared(currentDelta) > Vector3LengthSquared(*shiftDelta))
                // {
                //     *shiftDelta = currentDelta;
                // }

                nCollisions++;

                // Vector3 edge0 = Vector3Subtract(v1, v0);
                // Vector3 edge1 = Vector3Subtract(v2, v1);
                // lastNormal = Vector3Normalized(Vector3CrossProduct(edge0, edge1));

                // equidistantLine = {equidistantLine.x + lastNormal.x, equidistantLine.y + lastNormal.y, equidistantLine.z + lastNormal.z};
            }

            collide = nCollisions > 0 ? true : false;
        }

        if(collide)
        {
            // equidistantLine = Vector3Normalized(equidistantLine);

            // float sin =abs(  Vector3DotProduct(equidistantLine,lastNormal));
            // float eLineDistance = radius/sin - radius;

            // *shiftDelta = {
            //     shiftDelta->x/(float)nCollisions - (equidistantLine.x*eLineDistance),
            //     shiftDelta->y/(float)nCollisions - (equidistantLine.y*eLineDistance),
            //     shiftDelta->z/(float)nCollisions - (equidistantLine.z*eLineDistance)};

            *shiftDelta = {
                shiftDelta->x/(float)nCollisions,
                shiftDelta->y/(float)nCollisions,
                shiftDelta->z/(float)nCollisions};

            // *shiftDelta = {
            //     shiftDelta->x,
            //     shiftDelta->y,
            //     shiftDelta->z};

        }
        else *shiftDelta = {0,0,0};
            
    }
    
    return collide;
}

BSPNode* BuildBSPTree(std::vector<Vector3>& vertices) {
    if (vertices.empty()) {
        return nullptr;
    }

    // Seleccionar un plano de división (simplificado: plano mediano).
    std::cout << "Seleccionando plano de division" << std::endl;
    Plane divisionPlane = SelectDivisionPlane(vertices);

    std::cout << "div " << Vector3String(divisionPlane.normal) << std::endl;
    // Crear un nuevo nodo BSP.
    std::cout << "Creando nuevo nodo" << std::endl;
    BSPNode* node = new BSPNode;
    node->divisionPlane = divisionPlane;

    // Dividir los objetos sólidos según el plano de división.
    std::cout << "Dividiendo vertices entre los lados del plano" << std::endl;
    std::vector<Vector3> frontObjects;
    std::vector<Vector3> backObjects;
    SplitSolidObjects(vertices, divisionPlane, frontObjects, backObjects);

    // Construir recursivamente los subárboles.
    std::cout << "Recursion " << frontObjects.size() << " " << backObjects.size() << std::endl;
    node->frontChild = BuildBSPTree(frontObjects);
    node->backChild = BuildBSPTree(backObjects);
    std::cout << "Out" << std::endl;
    return node;
}

Plane SelectDivisionPlane(std::vector<Vector3>& vertices)
{
    int minDifference = (int)INFINITY;
    Plane bestPlane;
    int bestTri = -1;

    // For each triangle, we obtain the plane it defines and test if it insersects with any other triangle
    for(int i = 0 ; i < vertices.size(); i+=3)
    {
        Vector3 v0 = vertices[i];;
        Vector3 v1 = vertices[i+1];;
        Vector3 v2 = vertices[i+2];;

        Vector3 edge0 = Vector3Subtract(v1, v0);
        Vector3 edge1 = Vector3Subtract(v2, v1);

        Vector3 normal = Vector3Normalized(Vector3CrossProduct(edge0, edge1));

        std::cout << "PUNTO " << Vector3String(v0) << " NORMAL " << Vector3String(normal) << std::endl;

        Plane plane;
        plane.normal = normal;
        plane.point = v0;

        int nTrisInFront = 0;
        bool valid = true;

        // We test the intersection with the rest of triangles
        for(int k = 0 ; k < vertices.size()&&valid; k+=3)
        {
            int isTriOP = IsPolyOutsidePlane({vertices[k],vertices[k+1],vertices[k+2]},plane);

            nTrisInFront+=isTriOP;

            if(isTriOP == 0) 
            {
                std::cout << "miscoj" << std::endl;
                valid = false;
            }
        }

        // If the plane doesnt insersect with any tri and has a better vertex balance between both sides
        if(abs(nTrisInFront) < minDifference && valid)
        {
            minDifference = abs(nTrisInFront);
            bestPlane = plane;
            bestTri = i;
        }
    }

    if(bestTri!=-1)
    {
        std::cout << "Eliminando triangulo usado " <<  bestTri << " size " << vertices.size() << " ASFFFFFFFFFFFFFFFFFFFFFFFF" << std::endl;
        for(int k = 0; k < vertices.size();k++) std::cout << "v" << k << ":\t" << Vector3String(vertices[k]) << std::endl;

        // auto start = vertices.begin();
        // for(int i = 0; i < bestTri; i++) start++;
        // auto end = start;
        // end++;
        // end++;
        // end++;

        auto start = vertices.begin()+ bestTri;
        auto end = start+3;

        vertices.erase(start,end);

        std::cout << "AAAAAAASSSSSSS" << std::endl;
        for(int k = 0; k < vertices.size();k++) std::cout << "v" << k << ":\t" << Vector3String(vertices[k]) << std::endl;
    }

    return bestPlane;
}

int IsMeshOutsidePlane(Mesh mesh,Plane plane)
{
    bool isNegative = true;

    for(int i = 0; i < mesh.vertexCount;i+=3)
    {
        Vector3 vertexPos = {mesh.vertices[i],mesh.vertices[i+1],mesh.vertices[i+2]};

        Vector3 h = Vector3Subtract(vertexPos, plane.point);

        float distance = Vector3DotProduct(plane.normal,h);

        if(i==0)
        {
            isNegative = std::signbit(distance);
        }
        else
        {
            if(std::signbit(distance)!=isNegative) return 0;
        }
    }

    return isNegative ? -1 : 1;
}

int IsPolyOutsidePlane(const std::vector<Vector3>& v,Plane plane)
{
    bool isNegative = true;
    bool firstValid = false;

    for(int i = 0; i < v.size(); i++)
    {
        Vector3 h = Vector3Subtract(v[i], plane.point);

        float distance = Vector3DotProduct(plane.normal,h);

        if(!firstValid)
        {
            if(distance!=0)
            {
                isNegative = (distance < 0);
                firstValid = true;
            }
        }
        else
        {
            if((distance < 0)!=isNegative && distance!=0) return 0;
        }

    }

    return isNegative ? -1 : 1; 
}

void SplitSolidObjects(std::vector<Vector3>& vertices,Plane plane,std::vector<Vector3>& frontTris,std::vector<Vector3>& backTris)
{
    int j = 0;
    bool putOnBack = true;
    for(int i = 0; i < vertices.size(); i++)
    {
        Vector3 h = Vector3Subtract(vertices[i], plane.point);

        float distance = Vector3DotProduct(plane.normal,h);

        std::cout << "distance " << distance << " plane normal " << Vector3String(plane.normal) << " plane point " << Vector3String(plane.point) << " vertex " << Vector3String(vertices[i]) << std::endl;

        if(distance < 0)
            putOnBack = true;
        else if(distance > 0)
            putOnBack = false;

        // When we reach the third vertex of a triangle
        if(j == 2)
        {
            if(putOnBack) 
            {
                std::cout << "on back " << std::endl;
                backTris.push_back(vertices[i-2]);
                backTris.push_back(vertices[i-1]);
                backTris.push_back(vertices[i]);
            }
            else
            {
                std::cout << "on front " << std::endl;
                frontTris.push_back(vertices[i-2]);
                frontTris.push_back(vertices[i-1]);
                frontTris.push_back(vertices[i]);
            }
        }

        j = (j + 1) % 3;
    }
}

bool IsPointValidBSP(Vector3 point,BSPNode * node)
{
        Vector3 h = Vector3Subtract(point, node->divisionPlane.point);

        float distance = Vector3DotProduct(node->divisionPlane.normal,h);

        if(distance < 0)
        {
            if(node->backChild != nullptr) 
                return IsPointValidBSP(point, node->backChild);
            else
                return false;
        }
        else
        {
            if(node->frontChild != nullptr) 
                return IsPointValidBSP(point, node->frontChild);
            else
                return true;
        }
}