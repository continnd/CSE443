// Copyright 2015 Nick Contini

#ifndef CHART_MAKER_H
#define CHART_MAKER_H

#include <fstream>
#include "Point.h"
#include "continndChart.h"

/** A top-level class to perform operations on chart objects based on
	user input.

	NOTE: You may add as many methods as needed to design this class.
*/
class ChartMaker {
public:
    ChartMaker() {}
    ~ChartMaker() {}
    int run();

protected:
private:
    Chart current;
};

#endif
