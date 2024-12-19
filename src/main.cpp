#include "Headers/Config.h"
#include "Headers/App.h"

// This is my second try at a 3d modeler not a games engine start date 17/12/24

int main() {

	LogInternals::Instance()->Initialize();	// Logging

	
	App::Instance()->RunApp(); // Editor loop

	return 0;
}
