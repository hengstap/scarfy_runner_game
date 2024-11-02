#include "raylib.h"
#include <unistd.h>

// Custom Data Type for Animations
struct AnimData {
    Rectangle rec; // Rectangle to define the position and size of the sprite in the texture
    Vector2 pos; // Vector2 to store the position of the sprite
    int frame; // Current Frame
    float updateTime; // Time between each frame
    float runningTime; // Time that the current frame has been running
};

// Function to check if the sprite is on the ground
bool isOnGround(AnimData data, int windowHeight) {
    return data.pos.y >= windowHeight - data.rec.height;
}
// Fuction to update animation data
AnimData updateAnimData(AnimData data, float deltaTime, int maxFrames) {
    data.runningTime += deltaTime; // Get the running time
    if (data.runningTime >= data.updateTime) { // Check if the running time is greater than the update time
        data.runningTime = 0.0f; // Reset the running time
        data.frame++; // Move to the next frame
        if (data.frame > maxFrames) { // Check if the frame is the last frame
            data.frame = 0; // Reset the frame to the first frame
        }
        data.rec.x = data.frame * data.rec.width; // Update the x position of the rectangle to draw the next frame
    }
    return data;
}
int main() {
    // Window Dimensions Array
    const int windowDimensions[2] = { 512, 380 }; // Array to store the window dimensions
    
    // Screen Initialization Height and Width
    InitWindow(windowDimensions[0], windowDimensions[1], "Dasher");

    // Acceleration due to Gravity 1000 pixels per second/second
    const int gravity = 1'000;

    // Nebula Variables
    Texture2D nebula = LoadTexture("textures\\12_nebula_spritesheet.png"); // Texture2D variable to store the sprite sheet

    
    // Set number of nebulae
    const int numNebulae = 12; // Number of nebulae
    
    // Nebula Animation Data Array
    AnimData nebulae[numNebulae] = { }; // Array to store the animation data for the nebulae


    // For loop to fill the nebulae array
    for (int i = 0; i < numNebulae; i++) { // Loop through the nebulae array
        nebulae[i].rec.width = nebula.width / 8; // Set the width of the rectangle
        nebulae[i].rec.height = nebula.height / 8; // Set the height of the rectangle
        nebulae[i].rec.x = 0; // Set the x position of the rectangle
        nebulae[i].rec.y = 0; // Set the y position of the rectangle

        nebulae[i].pos.x = windowDimensions[0] + (windowDimensions[0] + (i*600)); // Set the x position of nebula outside screen
        nebulae[i].pos.y = windowDimensions[1] - nebulae[i].rec.height; // Set the y position of the sprite on ground level

        nebulae[i].frame = 0; // Starting Frame
        nebulae[i].updateTime = 1.0f / (12.0f + (i*2)); // Time between each frame
        nebulae[i].runningTime = 0.0f; // Time that the current frame has been running
    }

    // Nebula Velocity
    int nebVel[numNebulae] = { }; // Array to store the velocity of the nebulae

    // For loop to fill the nebulae velocity array
    for (int i = 0; i < numNebulae; i++) { // Loop through the nebulae array
        nebVel[i] = -200 + (i * -10); // Set the velocity of the nebula
    }

    // Scarfy Sprite Sheet
    Texture2D scarfy = LoadTexture("textures\\scarfy.png"); // Texture2D variable to store the sprite sheet

    // Animation Data for Scarfy
    AnimData scarfyData; // Variable to store the animation data
    scarfyData.rec.width = scarfy.width / 6; // Set the width of the rectangle
    scarfyData.rec.height = scarfy.height; // Set the height of the rectangle
    scarfyData.rec.x = 0; // Set the x position of the rectangle
    scarfyData.rec.y = 0; // Set the y position of the rectangle

    scarfyData.pos.x = windowDimensions[0] / 2 - scarfyData.rec.width / 2; // Set the x position of the sprite (center of the screen)
    scarfyData.pos.y = windowDimensions[1] - scarfyData.rec.height; // Set the y position of the sprite on ground level

    scarfyData.frame = 0; // Starting Frame
    scarfyData.updateTime = 1.0f / 8.0f; // Time between each frame
    scarfyData.runningTime = 0.0f; // Time that the current frame has been running

    // Set the velocity of the sprite
    int velocity = 0;

    // Jump Velocity
    const int jumpVelocity = -600; // Set the jump velocity 600 pixels per second


    SetTargetFPS(60); // Set the frames per second

    //Making the Background
    Texture2D background = LoadTexture("textures\\far-buildings.png"); // Texture2D variable to store the background
    Texture2D midground = LoadTexture("textures\\back-buildings.png"); // Texture2D variable to store the background
    Texture2D foreground = LoadTexture("textures\\foreground.png"); // Texture2D variable to store the background

    //Background Movement Variable
    float bg1Scroll = 0.0f; // Set the scroll value of the background
    float mg1Scroll = 0.0f; // Set the scroll value of the background
    float fg1Scroll = 0.0f; // Set the scroll value of the background

    // Make a Finish Line
    float finishLine = nebulae[numNebulae - 1].pos.x; // Set the finish line to the last nebula

    // Collision Variable
    bool collision = false; // Set the collision variable to false

    // Main game loop
    bool gameOver = false; // Add a flag to indicate when the game is over
    bool win = false; // Add a flag to indicate when the player wins
    while (!WindowShouldClose()) {
        // Basic Screen Drawing
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Setting delta time
        float dT = GetFrameTime(); // Get the time that passed between the last frame and the current frame

        if (!gameOver && !win) {
            // Update and Draw Background
            bg1Scroll -= 20 * dT; // Update the scroll value of the background
            if (bg1Scroll <= -background.width * 2) { // Check if the background has scrolled all the way
                bg1Scroll = 0.0; // Reset the scroll value of the background
            }
            mg1Scroll -= 40 * dT; // Update the scroll value of the background
            if (mg1Scroll <= -midground.width * 2) { // Check if the background has scrolled all the way
                mg1Scroll = 0.0; // Reset the scroll value of the background
            }
            fg1Scroll -= 80 * dT; // Update the scroll value of the background
            if (fg1Scroll <= -foreground.width * 2) { // Check if the background has scrolled all the way
                fg1Scroll = 0.0; // Reset the scroll value of the background
            }

            // Draw Background
            Vector2 bg1Pos = {bg1Scroll, 0.0}; // Set the position of the background
            DrawTextureEx(background, bg1Pos, 0, 2.0, WHITE); // Draw the background
            Vector2 bg2Pos = {bg1Scroll + background.width * 2, 0.0}; // Set the position of the background
            DrawTextureEx(background, bg2Pos, 0, 2.0, WHITE); // Draw the background

            // Draw Midground
            Vector2 mg1Pos = {mg1Scroll, 0.0}; // Set the position of the background
            DrawTextureEx(midground, mg1Pos, 0, 2.0, WHITE); // Draw the background
            Vector2 mg2Pos = {mg1Scroll + midground.width * 2, 0.0}; // Set the position of the background
            DrawTextureEx(midground, mg2Pos, 0, 2.0, WHITE); // Draw the background

            // Draw Foreground
            Vector2 fg1Pos = {fg1Scroll, 0.0}; // Set the position of the background
            DrawTextureEx(foreground, fg1Pos, 0, 2.0, WHITE); // Draw the background
            Vector2 fg2Pos = {fg1Scroll + foreground.width * 2, 0.0}; // Set the position of the background
            DrawTextureEx(foreground, fg2Pos, 0, 2.0, WHITE); // Draw the background

            // Apply gravity to velocity but only when on the ground
            if (isOnGround(scarfyData, windowDimensions[1])) { // Check if rectangle is on the ground
                // scarfy is on the ground
                velocity = 0; // Reset velocity to 0
            } else {
                // scarfy is in the air
                velocity += gravity * dT; // Apply gravity to velocity
            }

            // When scarfy is in Air
            bool isInAir = scarfyData.pos.y < windowDimensions[1] - scarfyData.rec.height; // Check if rectangle is in the air

            // scarfy Movement
            if (IsKeyPressed(KEY_SPACE) && !isInAir) { // Check if space is pressed and rectangle is on the ground
                velocity += jumpVelocity; // Move upwards when space is pressed
            }

            // Update nebula position Array
            for (int i = 0; i < numNebulae; i++) { // Loop through the nebulae array
                nebulae[i].pos.x += nebVel[i] * dT; // Update the x position of the nebula
            }

            // Update Finish Line
            finishLine += nebVel[numNebulae - 2] * dT; // Update the finish line

            // Update scarfy position
            scarfyData.pos.y += velocity * dT;

            scarfyData.runningTime += dT; // Get the running time
            // No animation when scarfy is in the air
            if (!isInAir) { // Check if rectangle is in the air
                // Update the frame
                scarfyData = updateAnimData(scarfyData, dT, 5); // Update the frame
            } else {
                // No animation when scarfy is in the air
                scarfyData.rec.x = 0; // Set the x position of the rectangle to 0
            }
            // Update nebula frame Array
            for (int i = 0; i < numNebulae; i++) { // Loop through the nebulae array
                nebulae[i] = updateAnimData(nebulae[i], dT, 7); // Update the frame
            }

            // Collision detection
            collision = false;
            for (AnimData nebula : nebulae) {
                float pad{20};
                Rectangle nebulaRec = {
                    nebula.pos.x + pad,
                    nebula.pos.y + pad,
                    nebula.rec.width - 2*pad,
                    nebula.rec.height - 2*pad
                };
                Rectangle scarfyRec = {
                    scarfyData.pos.x,
                    scarfyData.pos.y,
                    scarfyData.rec.width,
                    scarfyData.rec.height
                };
                // Adjust the positions of the rectangles to consider their actual positions on the screen
                nebulaRec.x += nebula.pos.x;
                nebulaRec.y += nebula.pos.y;
                scarfyRec.x += scarfyData.pos.x;
                scarfyRec.y += scarfyData.pos.y;
                if (CheckCollisionRecs(nebulaRec, scarfyRec)) {
                    collision = true;
                    break;
                }
            }

            // If collision is true, set gameOver to true
            if (collision) {
                gameOver = true;
            } else if (scarfyData.pos.x > finishLine) { // Check if Scarfy crosses the finish line
                win = true;
            } else {
                // Draw Nebula Array
                for (int i = 0; i < numNebulae; i++) { // Loop through the nebulae array
                    DrawTextureRec(nebula, nebulae[i].rec, nebulae[i].pos, GREEN); // Draw the texture using the rectangle and position defined
                }

                // Draw scarfy
                DrawTextureRec(scarfy, scarfyData.rec, scarfyData.pos, WHITE); // Draw the texture using the rectangle and position defined

                // Draw Finish Line
                DrawLine(finishLine, windowDimensions[1] - 50, finishLine, windowDimensions[1], RED); // Draw the finish line
            }
        } else if (gameOver) {
            // Display "Game Over!" text
            DrawText("Game Over!", windowDimensions[0] / 2 - MeasureText("Game Over!", 40) / 2, windowDimensions[1] / 2 - 40, 40, RED);
        } else if (win) {
            // Display "You Win!" text
            DrawText("You Win!", windowDimensions[0] / 2 - MeasureText("You Win!", 40) / 2, windowDimensions[1] / 2 - 40, 40, GREEN);
        }

        EndDrawing();
    }
    UnloadTexture(scarfy); // Unload texture
    UnloadTexture(nebula); // Unload texture
    UnloadTexture(background); // Unload texture
    UnloadTexture(midground); // Unload texture
    UnloadTexture(foreground); // Unload texture
    CloseWindow(); // Close the window
}
