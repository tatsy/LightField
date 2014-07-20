#include <iostream>
#include <string>
using namespace std;

#include "LightField.h"

const string folder = "rectified/";
const string winname = "Light Field";

int focalValue = 10;
int focalScale = 10;
int minFocal   = 0;
int maxFocal   = 20;

int apertureValue = 500;
int minAperture = 100;
int maxAperture = 5000;
LightField lightField;

void repaint() {
	double focal    = focalValue / (double)focalScale - 1.0;
	printf("focal = %f\n", focal);
	lightField.setFocalLength(focal);
	lightField.setApertureSize(apertureValue);
	lightField.render();
	lightField.show(winname);
	printf("Render finish!\n");
}

int main(int argc, char** argv) {
	// load light field
	lightField.load(folder);

	// setup interface
	cv::namedWindow(winname);
	lightField.setFocalLength(0.0);
	lightField.render();
	lightField.show(winname);
	char key = 0;
	while(key != 0x1b) {
		key = cv::waitKey(100);
		if(key == 'u') { // up key
			focalValue++;
			focalValue = min(focalValue, maxFocal);
			repaint();
		}
		if(key == 'd') { // down key
			focalValue--;
			focalValue = max(focalValue, minFocal);
			repaint();
		}
		if(key == 's') { // aperture smaller
			apertureValue -= 100;
			apertureValue = max(apertureValue, minAperture);
		}
		if(key == 'l') { // aperture larger
			apertureValue += 100;
			apertureValue = min(apertureValue, maxAperture);
		}
		if(key == 'i') {
			int f;
			cout << "focal value [0, 20]: ";
			cin >> f;
			if(f >= 0 && f <= 20) {
				focalValue = f;
				repaint();
			}
		}
		if(key == 'a') {
			int a;
			cout << "aperture size: ";
			cin >> a;
			apertureValue = a;
			repaint();
		}
	}
	cv::destroyAllWindows();
}
