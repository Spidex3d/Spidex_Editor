#include "Headers/Config.h"
#include "Headers/App.h"

// This is my games engine start date 16/03/2025
// Spidex World Horizon amde with my Spidex 3d engine
// I hate Programing
// I hate Programing
// I hate Programing
// <It works.>
// I Love Programing

int main() {



	LogInternals::Instance()->Initialize();	// Logging
	
	App::Instance()->RunApp(); // Editor loop

	return 0;
}
