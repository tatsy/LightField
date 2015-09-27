#ifndef _VIRTUAL_LF_CAMERA_H_
#define _VIRTUAL_LF_CAMERA_H_

#include <string>

class VirtualLFCamera {
private:
	int rows, cols;

public:
	VirtualLFCamera(int rows, int cols);

	~VirtualLFCamera();

	void loadModel(std::string& file);
};

#endif
