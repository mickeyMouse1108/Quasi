#include "Utils/Debug/Logger.h"
#include "src/TestManager.h"

int main(int argc, char* argv[]) {
    Quasi::Debug::QInfo$("Execution Directory: {}", argv[0]);
    Quasi::Debug::QInfo$("Project Directory: {}", Test::PROJECT_DIRECTORY);

    Test::TestManager tManager;
    tManager.OnInit();
    while (tManager.WindowIsOpen())
        tManager.OnRun();
    tManager.OnDestroy();
}
