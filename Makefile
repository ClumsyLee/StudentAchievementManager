CXX = g++
CXXFLAGS = -c -std=c++11 -O2 -Wall -Wextra
MKDIR = mkdir

OBJS = obj/course.o obj/student.o obj/manager.o obj/main.o obj/command_line_interface.o

bin/SAM: $(OBJS) | bin
	$(CXX) -o $@ $^ -lncurses

obj/course.o: src/course.cpp src/course.h| obj
	$(CXX) $(CXXFLAGS) -o $@ $<

obj/student.o: src/student.cpp src/student.h | obj
	$(CXX) $(CXXFLAGS) -o $@ $<

obj/manager.o: src/manager.cpp src/manager.h | obj
	$(CXX) $(CXXFLAGS) -o $@ $<

obj/main.o: src/main.cpp src/text_interface.h | obj
	$(CXX) $(CXXFLAGS) -o $@ $<

# obj/text_interface.o: src/text_interface.cpp src/text_interface.h | obj
# 	$(CXX) $(CXXFLAGS) -o $@ $<

obj/command_line_interface.o: src/command_line_interface.cpp src/command_line_interface.h | obj
	$(CXX) $(CXXFLAGS) -o $@ $<

src/student.h: src/common.h
src/course.h: src/common.h src/exam.h src/student.h
src/manager.h: src/student.h src/course.h
src/common.h: src/date.h
src/exam.h: src/date.h src/common.h
# src/text_interface.h: src/interface.h
src/command_line_interface.h: src/interface.h src/manager.h

obj:
	$(MKDIR) $@

bin:
	$(MKDIR) $@
