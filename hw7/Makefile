CXX=g++
CXXFLAGS=-std=c++11 -Wall -O3 -fopenmp -lpng -g
PROG_NAME=ImageSearch
OBJS=PNG.o

all: $(PROG_NAME)

$(PROG_NAME): $(OBJS) continnd_ImageSearch.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^

PNG.o: PNG.cpp PNG.h
	$(CXX) $(CXXFLAGS) -c $<

%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) -c $<

run_text_and: $(PROG_NAME)
	./$(PROG_NAME) images/TestImage.png images/and_mask.png text_and.png true 75 16

run_text_i: $(PROG_NAME)
	./$(PROG_NAME) images/TestImage.png images/i_mask.png text_i.png true

run_flag: $(PROG_NAME)
	./$(PROG_NAME) images/Flag_of_the_US.png images/star_mask.png flag.png true 50 32

run_miami: $(PROG_NAME)
	./$(PROG_NAME) images/MiamiMarcumCenter.png images/WindowPane_mask.png miami.png true 50 64

clean:
	rm -rf $(PROG_NAME) *.o
