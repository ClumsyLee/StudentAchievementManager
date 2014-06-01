CXX = g++
CXXFLAGS = -c -std=c++11 -O2 -Wall -Wextra
MKDIR = mkdir

all: obj/course.o obj/student.o obj/manager.o obj/main.o

obj/course.o: src/course.cpp src/course.h| obj
	$(CXX) $(CXXFLAGS) -o $@ $<

obj/student.o: src/student.cpp src/student.h | obj
	$(CXX) $(CXXFLAGS) -o $@ $<

obj/manager.o: src/manager.cpp src/manager.h | obj
	$(CXX) $(CXXFLAGS) -o $@ $<

obj/main.o: src/main.cpp src/text_interface.h | obj
	$(CXX) $(CXXFLAGS) -o $@ $<

src/student.h: src/common.h
src/course.h: src/common.h src/exam.h src/student.h
src/manager.h: src/student.h src/course.h
src/common.h: src/date.h
src/exam.h: src/date.h src/common.h

obj:
	$(MKDIR) $@

bin:
	$(MKDIR) $@
