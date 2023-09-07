#include "hud.h"
#include <iostream>

Hud::Hud()
{
    character = LoadTexture("assets/textures/char_face.png");
    bg = LoadTexture("assets/textures/hud_left_alt.png");

    characterPos = {6,84};
    bgPos = {0,0};
    
    characterSize = {(float)character.width,(float)character.height};
    bgSize = {(float)bg.width,(float)bg.height};
}

void Hud::draw(Vector2 scale,Vector2 pos)
{
    drawPart(bg,bgUv,{0,-bgSize.y},bgSize,0,WHITE,scale,pos);
    drawPart(character,characterUv,{characterPos.x,(characterPos.y-bgSize.y)},characterSize,0,WHITE,scale,pos);
}

void Hud::drawPart(Texture2D tex, Rectangle uv, Vector2 origin, Vector2 size, float rotation, Color color,Vector2 scale, Vector2 hudOrigin)
{
    DrawTexturePro(tex,uv,
        Rectangle{
            hudOrigin.x+origin.x*scale.x,
            hudOrigin.y+origin.y*scale.y,
            size.x*scale.x,
            size.y*scale.y},
        {0,0},0,WHITE);
}