#ifndef SAM_TEXT_INTERFACE_H_
#define SAM_TEXT_INTERFACE_H_

namespace SAM {

class TextInterface
{
 public:
    TextInterface();
    virtual ~TextInterface() = default;

    virtual int Run(int argc, const char* const argv[]);

 private:
};

}  // namespace SAM

#endif  // SAM_TEXT_INTERFACE_H_
