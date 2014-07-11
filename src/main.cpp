#include "command_line_interface.h"

int main(int argc, char *argv[])
{
    SAM::CommandLineInterface interface;

    return interface.Run(argc, argv);
}
