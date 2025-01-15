#include "Settings.h"
#include <imgui\ImGuiAF.h>
#include "../SpdLog/LogInternals.h"
#include <sstream> // Include sstream for string operations


bool show_settings_window = false;

bool gridNogrid = false;
float BgCol[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

void Settings::SettingsWindow(GLFWwindow* window)
{
    // at some point we will need to wight this info to an txt file
    if (show_settings_window) {
        ImGui::Begin(ICON_FA_COGS" Spidex 3d editor Settings");
        ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Spidex Editor");
        ImGui::SeparatorText(" Screen Settings ");
        if (ImGui::CollapsingHeader(ICON_FA_PALETTE" Screen Settings", ImGuiTreeNodeFlags_DefaultOpen))
        {
            
            ImGui::ColorEdit4("Screen Colour", BgCol);

        }
        if (ImGui::CollapsingHeader(ICON_FA_BARS" Grid", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::Checkbox("Hide Grid", &gridNogrid); // make the grid hide
        }

        ImGui::Separator();

            if (ImGui::Button("Save Settings")) //  I want to record the settings to an ini file for later use
            {
                SaveSettings("settings.txt");
                LogInternals::Instance()->Info("Engine Settings successfully Saved");
                show_settings_window = false;
            }
            ImGui::SameLine();
            if (ImGui::Button("Exit"))
            {
                show_settings_window = false;
            }
                    
        ImGui::End();
    }
}


        

void Settings::SaveSettings(const std::string& filename)
{
    std::ofstream file(filename);
    if (file.is_open()) {
        file << "BgCol=" << BgCol[0] << "," << BgCol[1] << "," << BgCol[2] << "," << BgCol[3] << "\n";
        file << "Grid=" << (gridNogrid ? "true" : "false") << "\n";
        // add more as needed
        file.close(); }
    else { 
        LogInternals::Instance()->Error("Failed to open settings file for writing");
    }
}


void Settings::LoadSettings(const std::string & filename)
    { std::ifstream file(filename);
        if (file.is_open()) { std::string line;
            while (std::getline(file, line)) { std::istringstream is_line(line);
                    std::string key;
                if (std::getline(is_line, key, '=')) { std::string value;
                if (std::getline(is_line, value)) { if (key == "BgCol") { 
                    std::istringstream is_value(value);
                    std::string col; int i = 0;
                    while (std::getline(is_value, col, ',') && i < 4) { 
                        BgCol[i++] = std::stof(col);
                    }
                } else if (key == "Grid") { gridNogrid = (value == "true");
            }
        } 
    } 
    } file.close();
    } else { LogInternals::Instance()->Error("Failed to open settings file for reading");
    }
}


//void Settings::LoadSettings(const std::string& filename)
//{
//    std::ifstream file(filename);
//    if (file.is_open()) { std::string line; while (std::getline(file, line)) { std::istringstream is_line(line);
//    std::string key;
//    if (std::getline(is_line, key, '=')) { std::string value;
//    if (std::getline(is_line, value)) { if (key == "BgCol") { std::istringstream is_value(value);
//    std::string col; int i = 0;
//    while (std::getline(is_value, col, ',') && i < 4) { BgCol[i++] = std::stof(col);
//    } } else if (key == "Grid") { gridNogrid = (value == "true"); } } } } file.close(); }
//    else { LogInternals::Instance()->Error("Failed to open settings file for reading"); }
//}