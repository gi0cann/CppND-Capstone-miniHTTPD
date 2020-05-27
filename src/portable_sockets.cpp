#include "portable_sockets.h"

bool init_sockets() {
#if defined(_WIN32)
    WSADATA d;
    if (WSAStartup(MAKEWORD(2, 2), &d)) {
        std::cerr << "Failed to initialize.\n" << "\n";
        return false;
    }
#endif
    return true;
}

void cleanup_sockets() {
#if defined(_WIN32)
    WSACleanup();
#endif
}
