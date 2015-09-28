#ifdef _MSC_VER
#pragma once
#endif

#ifndef _MAIN_INTERFACE_H_
#define _MAIN_INTERFACE_H_

#if defined(_WIN32) || defined(__WIN32__)
    #ifdef MAIN_INTERFACE_EXPORT
        #define MAIN_INTERFACE_DLL __declspec(dllexport)
    #else
        #define MAIN_INTERFACE_DLL __declspec(dllimport)
    #endif
#else
    #define MAIN_INTERFACE_DLL
#endif

#include <vector>

#include <QtWidgets/qwidget.h>
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qslider.h>
#include <QtGui/qevent.h>

#include "image_info.h"
#include "light_field_widget.h"

class MAIN_INTERFACE_DLL MainInterface : public QWidget {
    Q_OBJECT

private:
    // Main layout
    QHBoxLayout* hBoxLayout;

    // Light field widget
    LightFieldWidget* lightFieldWidget;

    // Right container
    QWidget* rightContainer;
    QVBoxLayout* vLayoutForRight;
    QPushButton* loadButton;
    QSlider* focalSlider;
    QSlider* apertureSlider;

    // Other variables
    std::vector<ImageInfo> viewInfos;

    // Mouse variables
    bool isClicking;
    QPoint mousePosition;

public:
    MainInterface(QWidget* parent = nullptr);
    ~MainInterface();

protected slots:
    void OnLoadButtonClicked();
    void OnFocalSliderValueChanged(int value);
    void OnApertureSliderValueChanged(int value);
};

#endif  // _MAIN_INTERFACE_H_
