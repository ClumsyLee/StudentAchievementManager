#include "student.h"

namespace SAM {

Student::Student(const std::string &name, IDType id, bool is_male)
        : name_(name),
          id_(id),
          is_male_(is_male),
          courses_taken_()
{
}

}  // namespace SAM
