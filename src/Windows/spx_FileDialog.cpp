#include "spx_FileDialog.h"
#include <iostream>

std::string spx_FileDialog::openFileDialog()
{
    OPENFILENAME ofn;
    wchar_t filename[MAX_PATH];

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = filename;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(filename);
    if (IsTexture) {
        ofn.lpstrFilter = L"Image Files\0*.jpg;*.jpeg;*.png;*.bmp\0All Files\0*.*\0";
    }
    else {
        ofn.lpstrFilter = L"Model Files\0*.obj;";

    }
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    if (GetOpenFileNameW(&ofn)) {
        std::wcout << L"File selected: " << ofn.lpstrFile << std::endl;
        std::wstring ws(ofn.lpstrFile);
        return std::string(ws.begin(), ws.end());
    }
    else {
        std::wcout << L"File selection failed or was canceled." << std::endl;
        return "";
    }
}
