#include "level.h"


// Draw game level
void DrawLevel(void)
{
    const int floorExtent = 150;
    const float tileSize = 5.0f;
    const Color tileColor1 = (Color){ 150, 200, 200, 255 };

    // Floor tiles
    for (int y = -floorExtent; y < floorExtent; y++)
    {
        for (int x = -floorExtent; x < floorExtent; x++)
        {
            if ((y & 1) && (x & 1))
            {
                DrawPlane((Vector3){ x*tileSize, 0.0f, y*tileSize}, (Vector2){ tileSize, tileSize }, tileColor1);
            }
            else if (!(y & 1) && !(x & 1))
            {
                DrawPlane((Vector3){ x*tileSize, 0.0f, y*tileSize}, (Vector2){ tileSize, tileSize }, LIGHTGRAY);
            }
        }
    }

    // const Vector3 towerSize = (Vector3){ 16.0f, 32.0f, 16.0f };
    // const Color towerColor = (Color){ 150, 200, 200, 255 };

    // Vector3 towerPos = (Vector3){ 16.0f, 16.0f, 16.0f };
    // DrawCubeV(towerPos, towerSize, towerColor);
    // DrawCubeWiresV(towerPos, towerSize, DARKBLUE);

    // towerPos.x *= -1;
    // DrawCubeV(towerPos, towerSize, towerColor);
    // DrawCubeWiresV(towerPos, towerSize, DARKBLUE);

    // towerPos.z *= -1;
    // DrawCubeV(towerPos, towerSize, towerColor);
    // DrawCubeWiresV(towerPos, towerSize, DARKBLUE);

    // towerPos.x *= -1;
    // DrawCubeV(towerPos, towerSize, towerColor);
    // DrawCubeWiresV(towerPos, towerSize, DARKBLUE);

    // Red sun
    DrawSphere((Vector3){ 300.0f, 300.0f, 0.0f }, 100.0f, (Color){ 255, 0, 0, 255 });
}