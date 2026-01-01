#include "OSFWallpaper.h"
#include <iostream>

int main(int argc, char** argv) {
    std::string imagePath = "";
    if (argc > 1) {
        imagePath = argv[1];
    } else {
        std::cerr << "Usage: osf-wallpaper <path-to-png-image>" << std::endl;
        // Proceed without image (just color)
    }

    opensef::OSFWallpaper wallpaper(imagePath);
    wallpaper.run();

    return 0;
}
