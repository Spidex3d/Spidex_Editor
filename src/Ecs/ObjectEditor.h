#pragma once
#include "EntityNodes.h"
class ObjectEditor
{
public:
	static ObjectEditor* Instance();

	void Initialize();

	void renderObjectEditor(std::vector<std::unique_ptr<BaseModel>>& ObjectVector);
	//void renderObjectEditor(GameData* selectedData, bool& showObjectEditor);


private:
	bool ShowObjectEditor;

};

