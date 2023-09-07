#include <raylib.h>
#include <vector>

class Component;

class Object
{
    Object * parent = nullptr;

    Matrix matrix = {
        1,0,0,0,
        0,1,0,0,
        0,0,1,0,
        0,0,0,1
        };

    std::vector<Component *> components;

    std::vector<Object *> children;

public:

    Transform transform;

    Object(Transform);

    Matrix getMatrix();

    void setParent(Object * parent);

    void addChild(Object * child);

    void update();

    void addComponent(Component * c);

    Vector3 getGlobalPosition();
};