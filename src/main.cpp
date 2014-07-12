#include <memory>

#include "command_line_interface.h"

int main(int argc, char *argv[])
{
    std::unique_ptr<SAM::Interface> interface(new SAM::CommandLineInterface);

    return interface->Run(argc, argv);
}
