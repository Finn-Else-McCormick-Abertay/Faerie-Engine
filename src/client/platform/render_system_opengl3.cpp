#include "render_system_opengl3.h"

#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>

#include <systems/resource_manager.h>
#include <systems/ecs.h>
#include <components/model.h>
#include <components/transform.h>

#include <stdio.h>

bool RenderSystemOpenGl3::InitImpl() {
    // GL 3.0 + GLSL 130
    m_glslVersion = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    return true;
}

void RenderSystemOpenGl3::ShutdownImpl() {
    if (ImGui::GetCurrentContext()) {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();
    }
}

void RenderSystemOpenGl3::CreateContext(SDL_Window* window) {
    p_window = window;

    m_glContext = SDL_GL_CreateContext(p_window);
    SDL_GL_MakeCurrent(p_window, m_glContext);
    SDL_GL_SetSwapInterval(1); // Enable vsync

    GLenum glewErr = glewInit();
    if (glewErr != GLEW_OK) {
        fprintf(stderr, "Error: glewInit(): %s\n", glewGetErrorString(glewErr));
    }
    
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForD3D(p_window);
    ImGui_ImplOpenGL3_Init(m_glslVersion.c_str());

    // TEMP! REMOVE! USE ONE FROM WINDOW (should move shader loading out of here)
    char* pathBuf = SDL_GetBasePath();
    std::string appPath = std::string(pathBuf);
    SDL_free(pathBuf);
    
    glGenVertexArrays(1, &vertexArrayId);
    glBindVertexArray(vertexArrayId);

    static const GLfloat g_vertex_buffer_data[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f,  1.0f, 0.0f,
    };

    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
}

void RenderSystemOpenGl3::BeginImGuiFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();
}

void RenderSystemOpenGl3::Render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 projectionMat = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,0.1f,100.0f); // In world coordinates

    // Camera matrix
    glm::mat4 viewMat = glm::lookAt(
        glm::vec3(4,3,3), // Camera is at (4,3,3), in World Space
        glm::vec3(0,0,0), // and looks at the origin
        glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
    );


    auto view = ECS::Registry().view<const Components::Model, const Components::Transform>();
    for (auto entity : view) {
        auto [model, trans] = view.get(entity);
        
        Shader shader = ResourceManager::Get<Shader>(model.shaderId);
        glUseProgram(shader.ProgramId());
        GLuint matrixId = glGetUniformLocation(shader.ProgramId(), "MVP");

        glm::mat4 worldMat = trans.AsMatrix();

        glm::mat4 mvp = projectionMat * viewMat * worldMat;

        glUniformMatrix4fv(matrixId, 1, GL_FALSE, &mvp[0][0]);

        // 1st attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glVertexAttribPointer(
            0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void*)0            // array buffer offset
        );
        // Draw the triangle !
        glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
        glDisableVertexAttribArray(0);
    }

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(p_window);
}