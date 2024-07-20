#include "gui.h"

GUI::GUI(GameController* controller) : gameController(controller) {}

void GUI::render() {
    ImGui::NewFrame(); // Start a new ImGui frame

    if (gameController->getState() == GameController::GameState::MAIN_MENU) {
        ImGui::Begin("Main Menu");
        if (ImGui::Button("Play")) {
            gameController->setState(GameController::GameState::PLAYING);
        }
        if (ImGui::Button("Select Level")) {
            gameController->setState(GameController::GameState::LEVEL_SELECTION);
        }
        if (ImGui::Button("Exit")) {
            gameController->setState(GameController::GameState::EXIT);
        }
        ImGui::End();
    } else if (gameController->getState() == GameController::GameState::PLAYING) {
        ImGui::Begin("Game Playing");
        if (ImGui::Button("Return to Main Menu")) {
            gameController->setState(GameController::GameState::MAIN_MENU);
        }
        ImGui::Text("Game Playing...");
        ImGui::End();
    } else if (gameController->getState() == GameController::GameState::LEVEL_SELECTION) {
        ImGui::Begin("Level Selection");
        if (ImGui::Button("Play")) {
            gameController->setState(GameController::GameState::PLAYING);
        }
        if (ImGui::Button("Return to Main Menu")) {
            gameController->setState(GameController::GameState::MAIN_MENU);
        }
        ImGui::End();
    } else if (gameController->getState() == GameController::GameState::EXIT) {
        ImGui::Text("Exiting Game");
    }

    ImGui::Render(); // Rendering ImGui content
}
