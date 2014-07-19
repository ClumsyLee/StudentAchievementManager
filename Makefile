CXX = g++
CXXFLAGS = -c -std=c++11 -Wall -Wextra
MKDIR = mkdir

OBJS = obj/command_line_interface.o obj/common.o obj/course.o obj/io.o obj/main.o obj/manager.o obj/student.o

bin/SAM: $(OBJS) | bin
	$(CXX) -o $@ $^ # -lncurses

obj/command_line_interface.o: src/command_line_interface.cpp src/command_line_interface.h src/io.h | obj
	$(CXX) $(CXXFLAGS) -o $@ $<

obj/common.o: src/common.cpp src/common.h
	$(CXX) $(CXXFLAGS) -o $@ $<

obj/course.o: src/course.cpp src/course.h| obj
	$(CXX) $(CXXFLAGS) -o $@ $<

obj/io.o: src/io.cpp src/io.h| obj
	$(CXX) $(CXXFLAGS) -o $@ $<

obj/main.o: src/main.cpp src/command_line_interface.h | obj
	$(CXX) $(CXXFLAGS) -o $@ $<

obj/manager.o: src/manager.cpp src/manager.h | obj
	$(CXX) $(CXXFLAGS) -o $@ $<

obj/student.o: src/student.cpp src/student.h | obj
	$(CXX) $(CXXFLAGS) -o $@ $<


# obj/text_interface.o: src/text_interface.cpp src/text_interface.h | obj
# 	$(CXX) $(CXXFLAGS) -o $@ $<


src/command_line_interface.h: src/interface.h src/manager.h
src/course.h: src/common.h src/student.h
src/io.h: src/manager.h
src/manager.h: src/student.h src/course.h
src/student.h: src/common.h
# src/text_interface.h: src/interface.h

obj:
	$(MKDIR) $@

bin:
	$(MKDIR) $@

clean:
	-rm obj/*.o
