#include "object.h"
#include "utils.h"
#include "component.h"
#include <iostream>

Object::Object(Transform t)
{
    transform = t;
}

Matrix Object::getMatrix()
{
    return matrix;
}

void Object::addComponent(Component * c)
{
    components.push_back(c);
    c->setParent(this);
}

void Object::update()
{
    // apply the transform
    if(parent!=nullptr)
        matrix = parent->getMatrix();
    else
        matrix = {
            1,0,0,0,
            0,1,0,0,
            0,0,1,0,
            0,0,0,1
        };

    matrix = MatrixProduct(matrix,{
        1,0,0,transform.translation.x,
        0,1,0,transform.translation.y,
        0,0,1,transform.translation.z,
        0,0,0,1
    });

    matrix = MatrixProduct(matrix,QuaternionMatrix(QuaternionNormalized(transform.rotation)));

    ScaleMatrix(&matrix,transform.scale);

    // update the components
    for(int l = 0; l < components.size();l++)  components[l]->update();
    
    // update children
    for(int l = 0; l < children.size();l++)  children[l]->update();
}

void Object::addChild(Object * child)
{
    children.push_back(child);
    child->setParent(this);
}

void Object::setParent(Object * p)
{
    parent = p;
}

Vector3 Object::getGlobalPosition()
{
    return {matrix.m12,matrix.m13,matrix.m14};
}