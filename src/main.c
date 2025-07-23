#include "entity.h"
#include "linkedlist.h"
#include "raylib.h"
#include "raymath.h"
#include <stdio.h>

int main(void)
{
    const int screenWidth = 1400;
    const int screenHeight = 800;
    struct entities *entities = entities_create();

    InitWindow(screenWidth, screenHeight, "Cube2");

    Camera camera = {0};
    camera.position = (Vector3){0.0f, 2.0f, 4.0f}; // Camera position
    camera.target = (Vector3){0.0f, 2.0f, 0.0f};   // Camera looking at point
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};        // Camera up vector 
    camera.fovy = 60.0f;                    // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE; // Camera projection type

    int cameraMode = CAMERA_FIRST_PERSON;

    // ShowCursor();

    DisableCursor();

    SetTargetFPS(60);

    entities_add(entities,
                 wall_create((Vector3){0.0, 2.5, 16.0}, 5.0, 32.0, 1.0, GOLD));

    entities_add(entities, wall_create((Vector3){-16.0f, 2.5f, 0.0f}, 5.0f, 1.0f,
                                       32.0f, BLUE));
    entities_add(entities, wall_create((Vector3){16.0f, 2.5f, 0.0f}, 5.0f, 1.0f,
                                       32.0f, LIME));

    while (!WindowShouldClose())
    {
        UpdateCameraPro(&camera,
                        (Vector3){
                            (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)) *
                                    0.1f - // Move forward-backward
                                (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) * 0.1f,
                            (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) *
                                    0.1f - // Move right-left
                                (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)) * 0.1f,
                            0.0f // Move up-down
                        },
                        (Vector3){
                            GetMouseDelta().x * 0.05f, // Rotation: yaw
                            GetMouseDelta().y * 0.05f, // Rotation: pitch
                            0.0f                       // Rotation: roll
                        },
                        GetMouseWheelMove() * 2.0f); // Move to target (zoom)

        if (IsKeyPressed(KEY_SPACE))
        {
            Vector3 direction =
                Vector3Normalize(Vector3Subtract(camera.target, camera.position));
            Vector3 far_target =
                Vector3Add(camera.position, Vector3Scale(direction, 100.0f));
            entities_add(entities, projectile_create(camera.position, far_target));
        }

        entities_process(entities, UPDATE);
        entities_process(entities, COLLID);

        BeginDrawing();
        ClearBackground(BLACK);
        BeginMode3D(camera);
        DrawPlane((Vector3){0.0f, 0.0f, 0.0f}, (Vector2){32.0f, 32.0f},
                  LIGHTGRAY); // Draw ground

        entities_process(entities, RENDER);

        EndMode3D();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
