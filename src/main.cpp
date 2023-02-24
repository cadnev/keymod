#include "main.h"

int main(int argc, char **argv)
{
    if (argc > 1) {
        conf_mode(argc, argv);
    }
    else
        daemon_mode();
    return 0;
}
