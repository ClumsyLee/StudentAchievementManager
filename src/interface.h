#ifndef SAM_INTERFACE_H_
#define SAM_INTERFACE_H_

namespace SAM {

class Interface
{
 public:
    Interface() = default;
    virtual ~Interface() = default;

    // Run SAM with the given command-line parameters.
    // Returns the error code which should be returned by main().
    virtual int Run(int argc, const char* const argv[]) = 0;
};

}  // namespace SAM

#endif  // SAM_INTERFACE_H_
