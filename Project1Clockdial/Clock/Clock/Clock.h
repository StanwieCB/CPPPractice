//programmed twice
//10-29-2016
//author:hank zhang(StanwieCB)

#ifndef _Clock_h
#define _Clock_h

#include <iostream>
#include <cmath>
#include <ctime>
#include <Windows.h>
#include <FL\Fl.H>
#include <FL\Fl_Window.H>
#include <FL\Fl_PNG_Image.H>
#include <Fl\Fl_Box.H>
#include <FL\fl_draw.H>

#define PI 3.1415626535897932384626

//Point structure to record x y coordinate
struct Point
{
	double X, Y;
	Point(int x, int y) :X(x), Y(y) {}
	Point() {}
};

//Clock class
class Clock:public Fl_Widget
{
private:
	Fl_Box* box;
	Fl_PNG_Image* dial;
	//boxes and number images
	Fl_Box* bgBox, *boxH1, *boxH2, *boxM1, *boxM2, *boxS1, *boxS2;
	Fl_PNG_Image* bg, *num[10];

	//num image size
	const int width = 160, height = 280;

	//time counters
	SYSTEMTIME clockTime;
	int second;
	int minute;
	int hour;

	//coordinates
	Point center;
	Point secondHand;
	Point minuteHand;
	Point hourHand;

	//length of hands
	const int secondLength = 190;
	const int minuteLength = 160;
	const int hourLength = 125;

	//function to get system time
	void inline updateTime();

	//function to update second hand
	void inline updateSecondHand();

	//function to update minute hand
	void inline updateMinuteHand();

	//function to update hour hand
	void inline updateHourHand();

	//function to draw second hand
	void inline drawSecondHand();

	//function to draw minute hand
	void inline drawMinuteHand();

	//function to draw hour hand
	void inline drawHourHand();

	//function to display second number
	void inline dispSecond();

	//function to display minute number
	void inline dispMinute();

	//function to display hour number
	void inline dispHour();

	//draw function inherited
	void draw();

	//Fl Timer
	static void Timer_CB(void *userdata)
	{
		Clock *o = (Clock*)userdata;
		o->redraw();
		Fl::repeat_timeout(1, Timer_CB, userdata);
	}

public:
	//constructor
	//usage:Clock c(int x, int y, int w, int h, const char* label)
	Clock(int x, int y, int w, int h, const char* la):Fl_Widget(x, y, w, h, la)
	{
		//load images
		bg = new Fl_PNG_Image("bg.png");
		num[0] = new Fl_PNG_Image("num0.png");
		num[1] = new Fl_PNG_Image("num1.png");
		num[2] = new Fl_PNG_Image("num2.png");
		num[3] = new Fl_PNG_Image("num3.png");
		num[4] = new Fl_PNG_Image("num4.png");
		num[5] = new Fl_PNG_Image("num5.png");
		num[6] = new Fl_PNG_Image("num6.png");
		num[7] = new Fl_PNG_Image("num7.png");
		num[8] = new Fl_PNG_Image("num8.png");
		num[9] = new Fl_PNG_Image("num9.png");

		//load boxes
		bgBox = new Fl_Box(500, 0, 1300, 500);
		boxS1 = new Fl_Box(881+500, 112, width, height);
		boxS2 = new Fl_Box(1069+500, 112, width, height);
		boxM1 = new Fl_Box(473+500, 112, width, height);
		boxM2 = new Fl_Box(659+500, 112, width, height);
		boxH1 = new Fl_Box(71+500, 112, width, height);
		boxH2 = new Fl_Box(260+500, 112, width, height);
		bgBox->image(bg);

		//initialize coordinates
		center.X = 250;
		center.Y = 250;
		secondHand.X = secondHand.Y = minuteHand.X = minuteHand.Y = hourHand.X = hourHand.Y = center.X;
		
		//display clockDial
		box = new Fl_Box(0, 0, 500, 500);
		dial = new Fl_PNG_Image("clockDial.png");
		box->image(dial);
		updateTime();

		//Run clock
		Fl::add_timeout(0.1, Timer_CB, (void*)this);
	}

	//default destructor
	~Clock() 
	{
		delete bg;
		for (int i = 0; i < 9; i++)
		{
			delete num[i];
		}
		delete bgBox, boxH1, boxH2, boxM1, boxM2, boxS1, boxH2;
	}
};

//function to get system time
void inline Clock::updateTime()
{
	GetLocalTime(&clockTime);
	second = clockTime.wSecond;
	minute = clockTime.wMinute;
	hour = clockTime.wHour;
}

//function to update second hand
void inline Clock::updateSecondHand()
{
	secondHand.X = center.X + secondLength * cos(PI / 2 - second / 30.0 * PI);
	secondHand.Y = center.Y - secondLength * sin(PI / 2 - second / 30.0 * PI);
}

//function to update minute hand
void inline Clock::updateMinuteHand()
{
	minuteHand.X = center.X + minuteLength * cos(PI / 2 - (minute + second / 60.0) / 30.0 * PI);
	minuteHand.Y = center.Y - minuteLength * sin(PI / 2 - (minute + second / 60.0) / 30.0 * PI);
}

//function to update hour hand
void inline Clock::updateHourHand()
{
	hourHand.X = center.X + hourLength * cos(PI / 2 - (hour % 12 + minute / 60.0) / 6.0 * PI);
	hourHand.Y = center.Y - hourLength * sin(PI / 2 - (hour % 12 + minute / 60.0) / 6.0 * PI);
}

//function to draw second hand
void inline Clock::drawSecondHand()
{
	fl_line_style(0, 0);
	fl_line(center.X, center.Y, secondHand.X, secondHand.Y);
}

//function to draw minute hand
void inline Clock::drawMinuteHand()
{
	fl_line_style(0, 3);
	fl_line(center.X, center.Y, minuteHand.X, minuteHand.Y);
}

//function to draw hour hand
void inline Clock::drawHourHand()
{
	fl_line_style(0, 5);
	fl_line(center.X, center.Y, hourHand.X, hourHand.Y);
}

//function to display second number
void inline Clock::dispSecond()
{
	int s1 = second / 10;
	int s2 = second % 10;

	//display second image
	boxS1->image(*(num + s1));
	boxS2->image(*(num + s2));
	boxS1->redraw();
	boxS2->redraw();
}

//function to display minute number
void inline Clock::dispMinute()
{
	int m1 = minute / 10;
	int m2 = minute % 10;
	//display minute image
	boxM1->image(*(num + m1));
	boxM2->image(*(num + m2));
	boxM1->redraw();
	boxM2->redraw();
}

//function to display hour number
void inline Clock::dispHour()
{
	int h1 = hour / 10;
	int h2 = hour % 10;
	//display hour image
	boxH1->image(*(num + h1));
	boxH2->image(*(num + h2));
	boxH1->redraw();
	boxH2->redraw();
}

//draw function inherited
void  Clock::draw()
{
	//cover old hands
	fl_color(FL_BLACK);
	drawSecondHand();
	drawMinuteHand();
	drawHourHand();

	//update new time
	updateTime();

	//newtime display in number Clock
	dispSecond();
	dispMinute();
	dispHour();

	//update coordinates
	updateSecondHand();
	updateMinuteHand();
	updateHourHand();

	//draw new hands
	fl_color(FL_RED);
	drawSecondHand();
	fl_color(FL_WHITE);
	drawMinuteHand();
	drawHourHand();
}
#endif