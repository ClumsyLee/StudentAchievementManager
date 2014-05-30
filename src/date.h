#ifndef SAM_DATE_H_
#define SAM_DATE_H_

namespace SAM {

class Date
{
 public:
    Date(int year, int month, int day);

    

 private:
    int year_;
    int month_;
    int day_;
};

}  // namespace SAM

#endif  // SAM_DATE_H_
