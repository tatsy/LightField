#include "main_interface.h"

#include <iostream>

#include <QtCore/qdebug.h>
#include <QtWidgets/qfiledialog.h>

MainInterface::MainInterface(QWidget* parent)
    : QWidget(parent)
    , hBoxLayout(new QHBoxLayout)
    , lightFieldWidget(new LightFieldWidget)
    , rightContainer(new QWidget)
    , vLayoutForRight(new QVBoxLayout)
    , loadButton(new QPushButton)
    , focusLabel(new QLabel("focus point"))
    , apertureLabel(new QLabel("aperture size"))
    , focusEdit(new QLineEdit("0.00"))
    , apertureEdit(new QLineEdit("5.00"))
    , focusSlider(new QSlider(Qt::Orientation::Horizontal))
    , apertureSlider(new QSlider(Qt::Orientation::Horizontal))
    , viewInfos()
    , isClicking(false)
    , mousePosition(0, 0) {
    // Font setting
    setFont(QFont("Arial"));

    // Main container
    setLayout(hBoxLayout);

    QSizePolicy leftSize(QSizePolicy::Preferred, QSizePolicy::Preferred);
    leftSize.setHorizontalStretch(5);
    lightFieldWidget->setSizePolicy(leftSize);
    hBoxLayout->addWidget(lightFieldWidget);

    QSizePolicy rightSize(QSizePolicy::Preferred, QSizePolicy::Preferred);
    rightSize.setHorizontalStretch(1);
    rightContainer->setSizePolicy(rightSize);
    hBoxLayout->addWidget(rightContainer);

    // Right container
    rightContainer->setLayout(vLayoutForRight);

    loadButton->setText("Load");
    vLayoutForRight->addWidget(loadButton);

    focusSlider->setRange(-100, 100);
    focusSlider->setSingleStep(1);
    focusSlider->setSliderPosition(0);
    vLayoutForRight->addWidget(focusLabel);
    vLayoutForRight->addWidget(focusEdit);
    vLayoutForRight->addWidget(focusSlider);

    apertureSlider->setRange(0, 200);
    apertureSlider->setSingleStep(1);
    apertureSlider->setSliderPosition(50);
    vLayoutForRight->addWidget(apertureLabel);
    vLayoutForRight->addWidget(apertureEdit);
    vLayoutForRight->addWidget(apertureSlider);

    vLayoutForRight->addStretch();

    // Signal and slots
    connect(loadButton, SIGNAL(clicked()), this, SLOT(OnLoadButtonClicked()));
    connect(focusSlider, SIGNAL(valueChanged(int)), SLOT(OnFocusSliderValueChanged(int)));
    connect(apertureSlider, SIGNAL(valueChanged(int)), SLOT(OnApertureSliderValueChanged(int)));
}

MainInterface::~MainInterface() {
    delete focusLabel;
    delete focusEdit;
    delete focusSlider;

    delete apertureLabel;
    delete apertureEdit;
    delete apertureSlider;

    delete loadButton;

    delete vLayoutForRight;
    delete rightContainer;

    delete lightFieldWidget;
    delete hBoxLayout;
}

void MainInterface::OnLoadButtonClicked() {
    QString dirString = QFileDialog::getExistingDirectory(this,
                                                          tr("Open Directory"),
                                                          "./",
                                                          QFileDialog::ShowDirsOnly);
    if (dirString == "") return;

    int iMax = 0;
    int jMax = 0;
    viewInfos.clear();
    QStringList files = QDir(dirString).entryList();
    for (int i = 0; i < files.size(); i++) {
        QFileInfo f(files[i]);
        if (f.suffix() == "png") {
            int row, col;
            double camx, camy;
            char ext[64];
            int ret = sscanf(files[i].toStdString().c_str(),
                             "out_%d_%d_%lf_%lf%s", &row, &col, &camx, &camy, ext);
            if (ret == 5) {
                qDebug("File name parsed: %s", files[i].toStdString().c_str());
                if (row >= 16 || col >= 16) continue;
                viewInfos.emplace_back(dirString + "/" + files[i],
                                       row, col, camx, camy);
                iMax = std::max(iMax, row + 1);
                jMax = std::max(jMax, col + 1);
            } else {
                std::cerr << "[ERROR] File name is invalid!!" << std::endl;
                std::cerr << "        It must be like \"out_%d_%d_%lf_%lf.png\"" << std::endl;
                std::abort();
            }
        }        
    }

    // If # of images is invalid, then return
    if (iMax * jMax != viewInfos.size()) {
        qDebug("[ERROR] # of images invalid!!");
        qDebug("%d images necessary but %d provided!!", iMax * jMax, viewInfos.size());
        return;
    }

    // If # of images is OK, set images to light field widget
    lightFieldWidget->setLightField(viewInfos, iMax, jMax);
}

void MainInterface::OnFocusSliderValueChanged(int value) {
    const float newValue = value / 10.0f;
    lightFieldWidget->setFocusPoint(newValue);
    focusEdit->setText(QString::number(newValue, 'f', 2));
}

void MainInterface::OnApertureSliderValueChanged(int value) {
    const float newValue = value / 10.0f;
    lightFieldWidget->setApertureSize(newValue);
    apertureEdit->setText(QString::number(newValue, 'f', 2));
}