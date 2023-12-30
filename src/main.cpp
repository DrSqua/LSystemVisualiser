#include <vector>
#include <unordered_set>
#include "raylib.h"
#include "../include/lsystem/LSystemInterpreter.hpp"
#include "LSystemDrawing.hpp"

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
const float pi = 3.14;

int screenWidth = 800;
int screenHeight = 450;

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------
void UpdateDrawFrame();     // Update and Draw one frame

//----------------------------------------------------------------------------------
// Main Entry Point
//----------------------------------------------------------------------------------
int main() {
    // Initialization
    //--------------------------------------------------------------------------------------
    InitWindow(screenWidth, screenHeight, "LSystemVisualizer");

    #if defined(PLATFORM_WEB)
        emscripten_set_main_loop(UpdateDrawFrame, 0, 1);
    #else
        SetTargetFPS(60);   // Set our game to run at 60 frames-per-second
        //--------------------------------------------------------------------------------------

        // States
        using CharType = std::string;

        const std::unordered_set<CharType> alphabet{"0", "1", "[", "]"};
        const std::vector<CharType> axiom = {"0"};
        std::unordered_set<Production<CharType>> productions{
                Production<CharType>("1", {"1", "1"}),
                Production<CharType>("0", {"1", "[", "0", "]", "0"})
        };
        LSystemInterpreter<CharType> lsystem = LSystemInterpreter(axiom, productions, alphabet);

        // Draw
        const std::vector<DrawRuleStruct<CharType>>draw_rules{
            DrawRuleStruct<CharType>{
                .symbolType =  "0",
                .draw_line_size = 35.0f,
                .end_this_branch = true,
            },
            DrawRuleStruct<CharType>{
                    .symbolType =  "1",
                    .draw_line_size = 35.0f
            },
            DrawRuleStruct<CharType>{
                    .symbolType =  "[",
                    .draw_line_size = 0.0,
                    .turn_angle = -( pi / 4),
                    .push_fifo = true
            },
            DrawRuleStruct<CharType>{
                    .symbolType =  "]",
                    .draw_line_size = 0.0,
                    .turn_angle = pi /4,
                    .pop_fifo = true
            }
        };

        std::size_t current_state_index = 0;
        std::vector<std::vector<CharType>> saved_states{axiom};
        LSystemDrawing<CharType> lsystem_drawing = LSystemDrawing<CharType>(draw_rules, static_cast<float>(screenWidth), static_cast<float>(screenHeight));

        while (!WindowShouldClose())    // Detect window close button or ESC key
        {
            // Handle Input
            if (IsKeyReleased(KEY_RIGHT)) {
                if (current_state_index == (saved_states.size() - 1)) {
                    saved_states.emplace_back(lsystem());
                }
                current_state_index += 1;
            }
            if (IsKeyReleased(KEY_LEFT)) {
                if (current_state_index != 0) {
                    current_state_index -= 1;
                }
            }
            // Update
            const auto current_state = saved_states.at(current_state_index);

            // Draw
            //----------------------------------------------------------------------------------
            BeginDrawing();
            ClearBackground(RAYWHITE); // Always required

            //! Current stats
            std::string state_string;
            for (const auto& substring: current_state) {
                state_string += " " + substring;
            }
            const char* stats_char = state_string.c_str();
            DrawText(stats_char, 4, 50, 16, DARKGRAY);

            //! Root
            DrawRectangleV(
                    {static_cast<float>(screenWidth / 2.0) - 10.0f, static_cast<float>(screenHeight) - 20.0f},
                    {20.0f, 20.0f}, MAROON
                    );

            //! Tree
            lsystem_drawing.Draw(current_state, powf(0.75, static_cast<float>(current_state_index)));


            //! Utilities
            DrawFPS(4, 4);

            EndDrawing();
            //----------------------------------------------------------------------------------
        }
    #endif

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
