#define _CRT_SECURE_NO_WARNINGS
#include <vector>
using namespace std;

#ifdef _OPENMP
#include <omp.h>
#endif

#include "LightField.h"

LightField::LightField() :
	nrows(0),
	ncols(0),
	apertureSize(1.0),
	focalLength(0.0),
	image(),
	fov()
{
}

LightField::LightField(std::string folder) :
	nrows(0),
	ncols(0),
	apertureSize(1.0),
	focalLength(0.0),
	image(),
	fov()
{
	this->load(folder);
}

LightField::~LightField()
{
	delete [] fov;
}

void LightField::load(string folder)
{
	int r, c;
	double x, y;
	std::string path;

	std::ifstream ifs(folder + "/dataset.txt", std::ios::in);
	if(!ifs.is_open()) {
		std::cerr << "Failed to open file" << std::endl;
		exit(-1);
	}

	ifs >> nrows >> ncols;

	cv::Mat I;
	fov = new LFNode[nrows * ncols];
	for(int i=0; i<nrows; i++) {
		for(int j=0; j<ncols; j++) {
			ifs >> r >> c >> x >> y >> path;
			printf("loading image at (%d, %d) ...\n", r, c);
			
			I = cv::imread(folder + "/" + path, CV_LOAD_IMAGE_COLOR);
			if(I.empty()) {
				std::cerr << "Failed to load image at (" << r << ", " << c << ")" << std::endl;
				exit(-1);
			}
			fov[i * ncols + j] = LFNode(x, y, I);
		}
	}
}

void LightField::showAt(int r, int c) 
{
	char winname[64];
	sprintf(winname, "image at (%d, %d)", r, c);
	cv::imshow(winname, fov[r * ncols + c].image);
	cv::waitKey(0);
	cv::destroyWindow(winname);
}

void LightField::show(string winname)
{
	cv::imshow(winname, image);
}

void LightField::setFocalLength(double focal)
{
	assert(focal >= -1.0 && focal <= 1.0);
	this->focalLength = focal;
}

void LightField::setApertureSize(double size)
{
	this->apertureSize = size;
}

void LightField::render()
{
	const int width  = fov[0].width();
	const int height = fov[0].height();
	const int ch     = fov[0].channels();
	const double gapRatio = 8.5;
	const double cameraGapX = width  / (double)(ncols - 1) * gapRatio;
	const double cameraGapY = height / (double)(nrows - 1) * gapRatio;
	const double initCameraX = -cameraGapX * (ncols - 1) / 2.0;
	const double initCameraY = -cameraGapY * (nrows - 1) / 2.0;
	const double focalRatio = 10.0 * gapRatio;

	image = cv::Mat(height, width, CV_MAKETYPE(CV_64F, ch));

	#ifdef _OPENMP
	#pragma omp parallel num_threads(2)
	#endif
	for(int y=0; y<height; y++) {
		for(int x=0; x<width; x++) {
			vector<double> vals(ch, 0.0);
			int validPixelCount = 0;
			for(int i=0; i<nrows; i++) {
				for(int j=0; j<ncols; j++) {
					double di = i - nrows / 2.0;
					double dj = j - ncols / 2.0;
					//if(di * di + dj * dj > (i - nrows) * (i - nrows)) continue;

					double cameraX = initCameraX + j * cameraGapX;
					double cameraY = initCameraY + i * cameraGapY;
					double radius = hypot(cameraX, cameraY);
					if(radius > apertureSize) continue;

					double projX   = 0.5 + x - width  / 2.0;
					double projY   = 0.5 + y - height / 2.0;
					double pixelX = cameraX + (projX - cameraX) / (1.0 + focalLength / focalRatio);
					double pixelY = cameraY + (projY - cameraY) / (1.0 + focalLength / focalRatio);
					int px = cvRound(pixelX + width / 2.0);
					int py = cvRound(pixelY + height / 2.0);
					if(px >= 0 && py >= 0 && px < width && py < height) {
						for(int c=0; c<ch; c++) {
							vals[c] += fov[i*ncols+j].getRGB(py, px, c);
						}
						validPixelCount++;
					}
				}
			}

			for(int c=0; c<ch; c++) {
				image.at<double>(y, x*ch+c) = vals[c] / validPixelCount;
			}
		}
	}
}
