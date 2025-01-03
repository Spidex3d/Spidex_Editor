#include "MainScreen.h"
#include "../Headers/Config.h"

#include <stb\stb_image.h>

#include <imgui\ImGuiAF.h>

#include <stdio.h>
#include "../Windows/Settings.h"

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
// Docking Space
bool DocOnOff = true; // make this a menu item or settings

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
           ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f)); // you can add a bit of padding  
        ImGui::Begin("DockSpace Demo", p_open, window_flags);
        if (!opt_padding)
            ImGui::PopStyleVar();

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);
           

        // Submit the DockSpace to the ini file
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
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::Begin("Main scene");
    const float window_width = ImGui::GetContentRegionAvail().x;
    const float window_height = ImGui::GetContentRegionAvail().y;

    Rescale_frambuffer(window_width, window_height);
    glViewport(0, 0, window_width, window_height);

    ImVec2 pos = ImGui::GetCursorScreenPos();

    ImGui::GetWindowDrawList()->AddImage((void*)main_scene_texture_id, ImVec2(pos.x, pos.y),
        ImVec2(pos.x + window_width, pos.y + window_height), ImVec2(0, 1), ImVec2(1, 0));

    ImGui::End();
    ImGui::PopStyleVar();
}

void MainScreen::MainWindowMenu(GLFWwindow* window)
{
    // This is my main window menu
    ImGui::BeginMainMenuBar();
    if (ImGui::BeginMenu("File"))
    {
        if (ImGui::MenuItem("New scene"))
        {

        }
        if (ImGui::MenuItem("Open scene"))
        {

        }
        ImGui::Separator();
        if (ImGui::MenuItem(ICON_FA_SAVE" Save scene"))
        {

        }
        if (ImGui::MenuItem("Save As scene"))
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
        if (ImGui::MenuItem(ICON_FA_COGS" Open Settings"))
        {
            show_settings_window = true; // show settings window

        }
        if (ImGui::MenuItem("Docking On"))
        {
           
        }
        if (ImGui::MenuItem("Docking Off"))
        {
            
        }

        ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Tools"))
    {
        if (ImGui::MenuItem("Open Tool Box"))
        {
            
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
            show_about_window = true; // show About Window
        }

        if (ImGui::MenuItem("Help"))
        {

        }
        ImGui::Separator();
        if (ImGui::MenuItem("Window Opacity on"))
        {
            glfwSetWindowOpacity(window, 0.6f);
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


void MainScreen::SettingsWindow(GLFWwindow* window)   
{
     
    //// at some point we will need to wight this info to an ini file
    //if (show_settings_window) {
    //    ImGui::Begin(ICON_FA_COGS" Spidex 3d editor Settings");
    //    ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Spidex Engine");
    //    ImGui::SeparatorText(" Screen Settings ");
    //    ImGui::ColorEdit4("Screen Colour", BgCol);
    //    if (ImGui::CollapsingHeader(ICON_FA_BARS" Grid", ImGuiTreeNodeFlags_DefaultOpen))
    //    {
    //       // ImGui::Checkbox("Hide Grid", &gridNogrid); // make the grid hide
    //    }

    //    ImGui::Separator();
    //    if (ImGui::Button("Close"))
    //    {
    //        show_settings_window = false;
    //    }

    //    ImGui::End();
    //}
}

void MainScreen::AboutWindow(GLFWwindow* window)
{
    if (show_about_window)
    {
        ImGui::Begin("About the Spidex 3d editor");
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Spidex Engine");
        ImGui::SeparatorText(" Info ");
        ImGui::TextWrapped("Hi I'm The Dyslexic Programmer.I'm not really a programmer, I'm a retired horticulturist."
            "iv only played with C++ and found it very interesting,"
            "so now I have lots of time to try and learn this stuff."
            "my aim is to go from all but zero to making a 3d Model Editor using Opengl."
        );
        ImGui::SeparatorText(" GitHub ");
        ImGui::Text("https://github.com/Spidex3d/Spidex_Editor");

        ImGui::Separator();
        if (ImGui::Button("Close"))
        {
            show_about_window = false;
        }
        ImGui::End();
    }
}

void MainScreen::WinInit(GLFWwindow* window)
{
    MainScreen::NewImguiFrame(window); // new frame
    ImGui::NewFrame();
    MainScreen::MainWindowMenu(window); // Main Menu
    MainScreen::AboutWindow(window);  // set the about window
    //MainScreen::SettingsWindow(window);  // set the Settings window
    
    bool p_open = true;
    MainScreen::MainDockSpace(&p_open); // The Doc Space

    MainScreen::MainSceanWindow(window); // Main Scene Window for drawing objects to

    //MainScreen::BgColour(BgCol);
   
}

void MainScreen::Creat_FrameBuffer()
{
    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    glGenTextures(1, &main_scene_texture_id);
    glBindTexture(GL_TEXTURE_2D, main_scene_texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, SCR_WIDTH, SCR_HEIGHT, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, main_scene_texture_id, 0);

    glGenRenderbuffers(1, &RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCR_WIDTH, SCR_HEIGHT);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR FRAMBUFFER:: framebuffer is not compleate!" << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

unsigned int MainScreen::LoadTextureFiles(const char* filename, GLuint* out_texture, int out_width, int out_height)
{
    return 0;
}

void MainScreen::Bind_Framebuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
}

void MainScreen::Unbinde_Frambuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void MainScreen::Rescale_frambuffer(float width, float height)
{
    glBindTexture(GL_TEXTURE_2D, main_scene_texture_id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, main_scene_texture_id, 0);


    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);
}

// Render the Imgui windows in the main window
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
   
}

void MainScreen::BgColour(float BgCol[4])
{
    glClearColor(BgCol[0], BgCol[1], BgCol[2], BgCol[3]); // main window background colour
}







