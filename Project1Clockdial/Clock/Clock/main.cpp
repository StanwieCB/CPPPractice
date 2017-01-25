//programmed twice
//10-29-2016
//author:hank zhang(StanwieCB)

#define _CRT_SECURE_NO_WARNINGS

#include "Clock.h"
#include "NumClock.h"

int main(void)
{
	Fl_Window *w1 = new Fl_Window(1800, 500, "StanwieCB's Clock");
	Clock clock(0, 0, 1800, 500, "clock");
	w1->show();
	return Fl::run();
}

