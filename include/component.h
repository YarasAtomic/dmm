#include <raylib.h>

class Object;

class Component
{
    Object * parent;

public:
    virtual void update() = 0;

    Object * getParent();

    void setParent(Object * p);
};

class ModelComponent : public Component
{
    Model model;

public:
    ModelComponent(Model model);

    void update();
};