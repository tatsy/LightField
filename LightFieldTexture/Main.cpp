#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#include <opencv2/opencv.hpp>

int main(int argc, char** argv) {
	if(argc <= 1) {
		cerr << "usage: ConstructLightFieldTexture.exe [folder]" << endl;
		exit(1);
	}

	string folder(argv[1]);
	ifstream ifs(folder + "/dataset.txt", ios::in);
	if(!ifs.is_open()) {
		cerr << "Failed to load data file" << endl;
		exit(1);
	}

	int ncols, nrows;

	ifs >> nrows >> ncols;
	cv::Mat texture;
	printf("%d %d\n", nrows, ncols);

	int row, col;
	double x, y;
	int imgW = -1, imgH = -1;
	
	string path;

	cv::Mat I;
	for(int i=0; i<nrows; i++) {
		for(int j=0; j<ncols; j++) {
			ifs >> row >> col >> x >> y >> path;
			I = cv::imread(folder + "/" + path, CV_LOAD_IMAGE_COLOR);
			cout << "Loading " << path << " ..." << endl;
			if(I.empty()) {
				cerr << "Failed to load image file at " << row << ", " << col << endl;
				exit(1);
			}

			cv::resize(I, I, cv::Size(), 0.25, 0.25);
			if(imgW == -1 && imgH == -1) {
				imgW = I.cols;
				imgH = I.rows;
				texture = cv::Mat(imgH * nrows, imgW * ncols, CV_8UC3);
			}

			int startX = i * imgW;
			int startY = j * imgH;
			I.convertTo(texture(cv::Rect(startX, startY, imgW, imgH)), CV_8U);
		}
	}	

	cv::imwrite("light_field_texture.png", texture);
}

