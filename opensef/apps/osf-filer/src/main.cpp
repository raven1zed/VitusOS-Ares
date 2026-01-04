#include <opensef/OpenSEFAppKit.h>
#include <opensef/OSFWindow.h>
#include <opensef/OpenSEFBase.h>
#include <iostream>

using namespace opensef;

int main(int argc, char** argv) {
    auto window = std::make_shared<OSFWindow>(900, 600, "Filer");
    window->show();
    
    OSFApplication::shared().run();
    return 0;
}
