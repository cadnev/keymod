#include "main.h"

int main(int argc, char **argv)
{
    logger::init(logger::debug, logger::cout);

    if (argc > 1)
    {
        conf_mode(argc, argv);
    }
    else
    {
        daemon_mode();
    }

    return 0;
}
