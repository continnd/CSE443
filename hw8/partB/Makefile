CXX=mpicxx
CXXFLAGS=-std=c++11 -Wall -g
PROG_NAME=WordSearch
RUN_PROG=mpiexec
OBJS=HTMLDownloader.o
NODES=6

all: $(PROG_NAME)

$(PROG_NAME): $(OBJS) continnd_Homework8_PartB.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

HTMLDownloader.o: HTMLDownloader.h
	$(CXX) $(CXXFLAGS) -c $<

%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) -c $<

run: $(PROG_NAME)
	$(RUN_PROG) -n $(NODES) $(PROG_NAME) 'http://www.users.miamioh.edu/raodm/ones.txt' one

clean:
	rm -rf $(PROG_NAME) *.o
