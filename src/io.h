#ifndef SAM_IO_H_
#define SAM_IO_H_

#include "manager.h"

namespace SAM {

class ManagerReader
{
 public:
    bool Read(const std::string &student_file_name,
              const std::string &course_file_name,
              Manager &manager);
    bool ReadFinalScore(const std::string &file_name,
                        Manager &manager,
                        CourseInfo::IDType course_id,
                        std::vector<Student::IDType> &unscored_students);
};

class ManagerWriter
{
 public:
    bool Write(const std::string &student_file_name,
               const std::string &course_file_name,
               const Manager &manager);
};

class ManagerIO : public ManagerReader, public ManagerWriter
{
};


}  // namespace SAM

#endif  // SAM_IO_H_
