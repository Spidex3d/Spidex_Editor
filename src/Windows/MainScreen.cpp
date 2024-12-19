#include "MainScreen.h"
#include "../Headers/Config.h"

#include <stb\stb_image.h>

#include <imgui\ImGuiAF.h>

#include <stdio.h>


MainScreen* MainScreen::Instance()
{
    static MainScreen* screen = new MainScreen;
    return screen;
}

void MainScreen::Initialize(GLFWwindow* window)
{
    // Load an icon to the main window
    GLFWimage images[1];
    images[0].pixels = stbi_load("Textures/Icon.png", &images[0].width, &images[0].height, 0, 4); // rgba = png
    glfwSetWindowIcon(window, 1, images);
    stbi_image_free(images[0].pixels);
}

void MainScreen::SetUpImGui(GLFWwindow* window)
{
     //ImGui set up
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
    //io.KeyMap[ImGuiKey_H] = GLFW_KEY_HOME;
    ImGui::StyleColorsDark();

    ImGui_ImplOpenGL3_Init(SHADER_VERSION);
    ImGui_ImplGlfw_InitForOpenGL(window, true);


    // Make it possible to use Icons From FontAwesome5
    ImFontConfig fontconfig;
    fontconfig.MergeMode = true;
    fontconfig.PixelSnapH = true;
    static const ImWchar ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };

    // set the fonts
    fontconfig.GlyphOffset = ImVec2(0.0f, 1.0f);
    //io.Fonts->AddFontFromFileTTF(ROBOTO_REG_PATH, FONT_SIZE); // sandard font type
    io.Fonts->AddFontFromFileTTF(FONT_PATH_MAIN, FONT_SIZE); // comic sans font type
    io.Fonts->AddFontFromFileTTF(FA_SOLID_PATH, FONT_SIZE, &fontconfig, ranges);

}

void MainScreen::NewImguiFrame(GLFWwindow* window)
{
    // New Frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
}
bool DocOnOff = true; // make this a menu item or settings
// Docking Space
void MainScreen::MainDockSpace(bool* p_open)
{
    if (DocOnOff) { // Docking on or off
        static bool opt_fullscreen = true;
        static bool opt_padding = false;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;// I changed this so my scean shows up on start up

        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen)
        {
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }
        else
        {
            dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
        }

        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        if (!opt_padding)
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace Demo", p_open, window_flags);
        if (!opt_padding)
            ImGui::PopStyleVar();

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        // Submit the DockSpace
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        ImGui::End();
    }
}
// main scean window to draw objects to
void MainScreen::MainSceanWindow(GLFWwindow* window)
{
}

void MainScreen::MainWindowMenu(GLFWwindow* window)
{
    // This is my main window menu
    ImGui::BeginMainMenuBar();
    if (ImGui::BeginMenu("File"))
    {
        if (ImGui::MenuItem("New Scean"))
        {

        }
        if (ImGui::MenuItem("Open Scean"))
        {

        }
        ImGui::Separator();
        if (ImGui::MenuItem(ICON_FA_SAVE" Save Scean"))
        {

        }
        if (ImGui::MenuItem("Save As Scean"))
        {

        }
        ImGui::Separator();
        if (ImGui::MenuItem(ICON_FA_SIGN_OUT_ALT" Exit"))
        {
            glfwSetWindowShouldClose(window, true);
        }
        ImGui::EndMenu();

    }
    if (ImGui::BeginMenu("Edit"))
    {
        if (ImGui::MenuItem("Cut"))
        {

        }
        if (ImGui::MenuItem("Copy"))
        {

        }
        if (ImGui::MenuItem("Paste"))
        {

        }
        ImGui::Separator();
        if (ImGui::MenuItem("Wire Frame"))
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        if (ImGui::MenuItem("Wire Frame off"))
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Objects"))
    {
        if (ImGui::MenuItem("Ground Plane"))
        {

        }

        if (ImGui::MenuItem("Water Plane"))
        {

        }

        if (ImGui::MenuItem("Cube"))
        {

        }
        ;
        if (ImGui::MenuItem("Plane"))
        {

        }
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Settings"))
    {
        if (ImGui::MenuItem("Open Settings"))
        {
            // this will open a new panel to select a new sky Texture
        }

        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Tools"))
    {
        if (ImGui::MenuItem("Open Tool Box"))
        {
            // this will open a new panel to select a new Texture
        }

        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("View"))
    {
        if (ImGui::MenuItem("Hi"))
        {

        }
        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Render"))
    {
        if (ImGui::MenuItem("Testing"))
        {


        }
        ImGui::EndMenu();
    }

    if (ImGui::BeginMenu("About"))
    {
        if (ImGui::MenuItem("About Screen"))
        {
            //show_about_window = true;
        }

        if (ImGui::MenuItem("Help"))
        {

        }
        ImGui::Separator();
        if (ImGui::MenuItem("Window Opacity on"))
        {
            glfwSetWindowOpacity(window, 0.5f);
        }
        if (ImGui::MenuItem("Window Opacity off"))
        {
            glfwSetWindowOpacity(window, 1.0f);
        }
        ImGui::Separator();
        if (ImGui::MenuItem("Visit Web Site"))
        {

        }
        ImGui::EndMenu();
    }

    ImGui::EndMainMenuBar();

}

// Rener the Imgui windows in the main window
void MainScreen::RenderImGui(GLFWwindow* window)
{
    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_context = glfwGetCurrentContext(); // Get the current window conttext
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_context);
    }
}

void MainScreen::ClearScreen()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // main window background colour
}

void MainScreen::SwapBuffers()
{
    glfwSwapBuffers(window);
}

void MainScreen::PollEvents()
{
    glfwPollEvents();
}
