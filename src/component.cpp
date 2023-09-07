#include "component.h"
#include "object.h"
#include "utils.h"
#include <iostream>

void Component::setParent(Object * p)
{
    parent = p;
}

Object * Component::getParent()
{
    return parent;
}

ModelComponent::ModelComponent(Model model)
{
    this->model = model;
}

void ModelComponent::update()
{
    DrawMesh(model.meshes[0],model.materials[0],getParent()->getMatrix());
}

