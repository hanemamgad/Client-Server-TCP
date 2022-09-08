#include <iostream>
#include "Server.h"

using namespace std;


int main() {
    Server server("127.0.0.1", 8080);

    if (server.Init()) {
        server.run();
    }
    return 0;
}

