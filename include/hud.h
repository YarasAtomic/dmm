#include <raylib.h>
#include <string>

class Hud
{
    int hudRot = 0;

    Vector2 hudOrigin = {0,0};

    Texture2D character;
    Vector2 characterPos = {0,0};
    Rectangle characterUv = {0,0,32,32};
    Vector2 characterSize = {1,1};

    Texture2D bg;
    Vector2 bgPos = {0,0};
    Rectangle bgUv = {0,0,128,128};
    Vector2 bgSize = {1,1};

    void drawPart(Texture2D tex, Rectangle uv, Vector2 origin, Vector2 size, float rotation, Color color,Vector2 scale, Vector2 hudOrigin);

public:
    Hud();

    void draw(Vector2 scale,Vector2 pos);
};