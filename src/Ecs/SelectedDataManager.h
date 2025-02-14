#pragma once


// This manages the selected data for the imguitreeveiw
class SelectedDataManager {
public:
    static SelectedDataManager& Instance() {
        static SelectedDataManager instance;
        return instance;
    }

    BaseModel* GetSelectedData() const {
        return selectedData;
    }

    void SetSelectedData(BaseModel* data) {
        selectedData = data;
    }

private:
    SelectedDataManager() : selectedData(nullptr) {}
    ~SelectedDataManager() {}

    SelectedDataManager(const SelectedDataManager&) = delete;
    SelectedDataManager& operator=(const SelectedDataManager&) = delete;

    BaseModel* selectedData;
};




