#ifndef _LF_NODE_H_
#define _LF_NODE_H_

#include <opencv2/opencv.hpp>

class LFNode {
	friend class LightField;

private:
	double x, y;
	cv::Mat image;

public:
	LFNode();											// default constructor
	LFNode(double x, double y, const cv::Mat& image);	// constructor

	int    width() const { return image.cols; }
	int    height() const { return image.rows; }
	int    channels() const { return image.channels(); }
	double getX() const { return x; }
	double getY() const { return y; }
	double getRGB(int i, int j, int ch) { return image.at<uchar>(i, j*image.channels()+ch) / 255.0; }
};

#endif
