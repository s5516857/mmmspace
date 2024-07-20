#include "gamecontroller.h"
#include <iostream>

GameController::GameController() : currentState(GameState::MAIN_MENU) {}

void GameController::processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
        if (currentState == GameState::MAIN_MENU) {
            currentState = GameState::PLAYING;
        } else if (currentState == GameState::LEVEL_SELECTION) {
            currentState = GameState::PLAYING;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        if (currentState == GameState::MAIN_MENU) {
            currentState = GameState::EXIT;
        } else if (currentState == GameState::PLAYING || currentState == GameState::LEVEL_SELECTION) {
            currentState = GameState::MAIN_MENU;
        }
    }
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        if (currentState == GameState::MAIN_MENU) {
            currentState = GameState::LEVEL_SELECTION;
        }
    }
}

void GameController::update() {
    if (currentState == GameState::PLAYING) {
        // Add game state update logic here, for example:
        // - collision detection
        // - calculation of scores
        // - Game Over Condition Judgement
        // This logic can be extended and implemented according to specific game requirements
    }
}


void GameController::render() {
    if (currentState == GameState::MAIN_MENU) {
        std::cout << "Main Menu: Press Enter to Play, L to Select Level, ESC to Exit" << std::endl;
    } else if (currentState == GameState::PLAYING) {
        std::cout << "Game Playing: Press ESC to return to Main Menu" << std::endl;
    } else if (currentState == GameState::LEVEL_SELECTION) {
        std::cout << "Level Selection: Press Enter to Play, ESC to return to Main Menu" << std::endl;
    } else if (currentState == GameState::EXIT) {
        std::cout << "Exiting Game" << std::endl;
    }
}

GameController::GameState GameController::getState() const {
    return currentState;
}

void GameController::setState(GameState state) {
    currentState = state;
}
