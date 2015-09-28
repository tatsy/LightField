#ifdef _MSC_VER
#pragma once
#endif

#ifndef _IMAGE_INFO_H_
#define _IMAGE_INFO_H_

#include <QtCore/qstring.h>

//! Class to store information of each camera view image
class ImageInfo {
private:
    QString _path;
    int _row;
    int _col;
    double _cameraY;
    double _cameraX;

public:
    ImageInfo()
        : _path()
        , _row(-1)
        , _col(-1)
        , _cameraY(0.0)
        , _cameraX(0.0) {
    }

    ImageInfo(const QString& path,
              int row, int col,
              double cameraY, double cameraX)
        : _path(path)
        , _row(row)
        , _col(col)
        , _cameraY(cameraY)
        , _cameraX(cameraX) {
    }

    ImageInfo(const ImageInfo& info)
        : _path(info._path)
        , _row(info._row)
        , _col(info._col)
        , _cameraY(info._cameraY)
        , _cameraX(info._cameraX) {
    }

    ImageInfo::~ImageInfo() {
    }

    ImageInfo& operator=(const ImageInfo& info) {
        this->_path = info._path;
        this->_row = info._row;
        this->_col = info._col;
        this->_cameraY = info._cameraY;
        this->_cameraX = info._cameraX;
        return *this;
    }

    inline const QString& path() const { return _path; }
    inline int row() const { return _row; }
    inline int col() const { return _col; }
    inline double cameraY() const { return _cameraY; }
    inline double cameraX() const { return _cameraX; }
};

#endif  // _IMAGE_INFO_H_
