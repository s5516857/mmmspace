#include "spacegame.h"
#include <iostream>

// OpenGL function declaration
void RenderDrawData(ImDrawData* draw_data);

SpaceGame::SpaceGame()
    : window(nullptr, glfwDestroyWindow),
      xOffset(0.0f),
      yOffset(0.0f),
      shader(nullptr),
      spaceship(nullptr),
      gameController(std::make_unique<GameController>()),
      gui(std::make_unique<GUI>(gameController.get())) {} // 初始化 GUI

bool SpaceGame::init() {
    // Initialising the GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return false;
    }

    // Setting the OpenGL version and configuration
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // Create a window, 1024 pixel , 768 pixel
    window.reset(glfwCreateWindow(1024, 768, "Spaceship Shooter", nullptr, nullptr));
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(window.get());
    glfwSetFramebufferSizeCallback(window.get(), framebuffer_size_callback);

    // Initiallising GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return false;
    }

    // Initialising ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    // Setting up ImGui's window bindings
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // 启用键盘控制

    // Creating a Shader Object
    shader = std::make_shared<Shader>("../../shaders/vertex_shader.glsl", "../../shaders/fragment_shader.glsl");

    // Creating a Spaceship Object
    spaceship = std::make_unique<Spaceship>(shader);

    return true;
}

void SpaceGame::run() {
    // rendering cycle
    while (!glfwWindowShouldClose(window.get())) {
        // input processing
        processInput();

        // Render command
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Update Game Controller
        gameController->update();

        // Rendering the current state
        gameController->render();

        // Start ImGui Frame
        ImGui::NewFrame();

        // Rendering GUI
        gui->render();

        // Rendering ImGui content
        ImGui::Render();
        RenderDrawData(ImGui::GetDrawData());

        if (gameController->getState() == GameController::GameState::PLAYING) {
            // Drawing a spaceship
            spaceship->draw(xOffset, yOffset);
        } else if (gameController->getState() == GameController::GameState::EXIT) {
            break;
        }

        // Swap buffers and check events
        glfwSwapBuffers(window.get());
        glfwPollEvents();
    }

    // Clear ImGui
    ImGui::DestroyContext();
    glfwTerminate();
}

void SpaceGame::processInput() {
    gameController->processInput(window.get());

    if (gameController->getState() == GameController::GameState::PLAYING) {
        float moveSpeed = 0.0003f;
        if (glfwGetKey(window.get(), GLFW_KEY_W) == GLFW_PRESS)
            yOffset += moveSpeed;
        if (glfwGetKey(window.get(), GLFW_KEY_S) == GLFW_PRESS)
            yOffset -= moveSpeed;
        if (glfwGetKey(window.get(), GLFW_KEY_A) == GLFW_PRESS)
            xOffset -= moveSpeed;
        if (glfwGetKey(window.get(), GLFW_KEY_D) == GLFW_PRESS)
            xOffset += moveSpeed;
    }
}

void SpaceGame::framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// Functions for rendering data drawn by ImGui
void RenderDrawData(ImDrawData* draw_data) {
    // Backup GL state
    GLint last_program;
    glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);

    // Setting the OpenGL state to draw ImGui
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_SCISSOR_TEST);

    // Setting the viewport
    glViewport(0, 0, (GLsizei)draw_data->DisplaySize.x, (GLsizei)draw_data->DisplaySize.y);

    // Rendering ImGui Plotting Data
    for (int n = 0; n < draw_data->CmdListsCount; n++) {
        const ImDrawList* cmd_list = draw_data->CmdLists[n];
        const ImDrawIdx* idx_buffer_offset = 0;

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)cmd_list->VtxBuffer.Size * sizeof(ImDrawVert), (const GLvoid*)cmd_list->VtxBuffer.Data, GL_STREAM_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx), (const GLvoid*)cmd_list->IdxBuffer.Data, GL_STREAM_DRAW);

        for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++) {
            const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
            if (pcmd->UserCallback) {
                pcmd->UserCallback(cmd_list, pcmd);
            } else {
                glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->TextureId);
                glScissor((int)pcmd->ClipRect.x, (int)(draw_data->DisplaySize.y - pcmd->ClipRect.w), (int)(pcmd->ClipRect.z - pcmd->ClipRect.x), (int)(pcmd->ClipRect.w - pcmd->ClipRect.y));
                glDrawElements(GL_TRIANGLES, (GLsizei)pcmd->ElemCount, sizeof(ImDrawIdx) == 2 ? GL_UNSIGNED_SHORT : GL_UNSIGNED_INT, idx_buffer_offset);
            }
            idx_buffer_offset += pcmd->ElemCount;
        }
    }

    // Restore modified GL state
    glUseProgram(last_program);
    glDisable(GL_SCISSOR_TEST);
    glDisable(GL_BLEND);
}

