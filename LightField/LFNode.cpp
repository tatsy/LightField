#include "LFNode.h"

LFNode::LFNode() :
	x(0.0),
	y(0.0),
	image()
{
}

LFNode::LFNode(double x_, double y_, const cv::Mat& image_) :
	x(x_),
	y(y_),
	image(image_)
{
	cv::resize(image, image, cv::Size(), 0.40, 0.40);
}
