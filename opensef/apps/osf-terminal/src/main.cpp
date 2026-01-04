#include <opensef/OpenSEFAppKit.h>
#include <opensef/OSFWindow.h>
#include <opensef/OpenSEFBase.h>
#include <iostream>

using namespace opensef;

int main(int argc, char** argv) {
    // OSFApplication is a singleton
    
    // Create Main Window
    auto window = std::make_shared<OSFWindow>(800, 500, "Terminal");
    window->show();
    
    // Run Application Loop
    OSFApplication::shared().run();
    return 0;
}
