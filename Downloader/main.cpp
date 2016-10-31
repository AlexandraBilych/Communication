#include "Downloader.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
       std::cout << "Usage: " << argv[0] << " [url]" << std::endl;
       exit(0);
    }

    Downloader* image = new Downloader(argv[1]);

    (*image)();

    return 0;
}
