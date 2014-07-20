#ifndef _LIGHT_FIELD_H_
#define _LIGHT_FIELD_H_

#include <fstream>
#include <string>
#include <vector>

#include "LFNode.h"

class LightField {
private:
	int nrows;
	int ncols;
	double apertureSize;			// aperture size
	double focalLength;				// ratio of focal length [-1, 1]
	cv::Mat image;					// rendered image
	LFNode* fov;

public:
	LightField();					// default constructor
	LightField(std::string file);	// constructor
	~LightField();					// destructor

	void load(std::string file);	// load folder

	void show(std::string winname);
	void showAt(int r, int c);
	void render();

	void setFocalLength(double focal);
	void setApertureSize(double size);
};

#endif
