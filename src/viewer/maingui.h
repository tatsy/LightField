#ifdef _MSC_VER
#pragma once
#endif

#ifndef MAINGUI_H
#define MAINGUI_H

#include <vector>

#include <qmainwindow.h>
#include <qboxlayout.h>
#include <qwidget.h>

#include "imageinfo.h"
#include "light_field_widget.h"

class MainGui : public QMainWindow {
    Q_OBJECT

public:
    explicit MainGui(QWidget *parent = nullptr);
    virtual ~MainGui();

private slots:
    void OnLoadButtonClicked();
    void OnFocusSliderValueChanged(int value);
    void OnApertureSliderValueChanged(int value);
    void OnFrameSwapped();

private:
    // UI
    class Ui;
    Ui *ui;
    LightFieldWidget *viewer;

    QWidget *mainWidget;
    QHBoxLayout *mainLayout;

    // Other variables
    std::vector<ImageInfo> viewInfos;

    // Mouse variables
    bool isClicking;
    QPoint mousePosition;
};

#endif  // MAINGUI_H
