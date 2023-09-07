#include "collision.h"
#include <iostream>

bool CheckCollisionSphereTri(Vector3 spherePos, float radius, Vector3 v0, Vector3 v1, Vector3 v2, Vector3 *shiftDelta, float * edgeDistanceSquared)
{
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

    Vector3 min0, min1, min2;

    *edgeDistanceSquared = INFINITY;

    bool ray0 = simpleRaycastSphere(v0,edge0,spherePos,squaredRadius,&min0);
    bool ray1 = simpleRaycastSphere(v1,edge1,spherePos,squaredRadius,&min1);
    bool ray2 = simpleRaycastSphere(v2,edge2,spherePos,squaredRadius,&min2);

    if(!(!ray0 &&!ray1 &&!ray2)) 
    {
        float d0 = Vector3LengthSquared(min0);
        float d1 = Vector3LengthSquared(min1);
        float d2 = Vector3LengthSquared(min2);

        *edgeDistanceSquared = d0 < d1 ? d0 : d1;
        *edgeDistanceSquared = *edgeDistanceSquared < d2 ? *edgeDistanceSquared : d2;

        std::cout << "MIN edge distance " << *edgeDistanceSquared << std::endl;

        collide = true;
    }

    if(collide)
        *shiftDelta = {shiftDelta->x+(normal.x*(radius-distance)),shiftDelta->y+(normal.y*(radius-distance)),shiftDelta->z+(normal.z*(radius-distance))};

    return collide;
}

bool CheckCollisionSphereMesh(Vector3 spherePos, float radius,Mesh mesh, Vector3 *shiftDelta)
{
    *shiftDelta = {0,0,0};

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
        float minDistanceSquared = INFINITY;

        int j = 0;
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

            float currentMinDistance = INFINITY;

            nCollisions = CheckCollisionSphereTri(spherePos,radius,v0,v1,v2,shiftDelta,&currentMinDistance) ? nCollisions + 1 : nCollisions;
            
            minDistanceSquared = currentMinDistance < minDistanceSquared ? currentMinDistance : minDistanceSquared;

            // std::cout << "current min distance " << minDistanceSquared << std::endl;

            collide = nCollisions > 0 ? true : false;
        }

        if(collide)
        {
            if(minDistanceSquared>=INFINITY) 
                *shiftDelta = {shiftDelta->x/(float)nCollisions,shiftDelta->y/(float)nCollisions,shiftDelta->z/(float)nCollisions};
            else
            {
                float shiftDeltaLengthSquared = Vector3LengthSquared(*shiftDelta);
                if(minDistanceSquared<shiftDeltaLengthSquared)
                {
                    float shiftDeltaLength = sqrt(shiftDeltaLengthSquared);
                    float minDistance = sqrt(minDistanceSquared);
                    *shiftDelta = {shiftDelta->x/shiftDeltaLength*minDistance,shiftDelta->y/shiftDeltaLength*minDistance,shiftDelta->z/shiftDeltaLength*minDistance};
                }
            }
        }
    }
    
    return collide;
}