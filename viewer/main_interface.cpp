#include "main_interface.h"

#include <QtCore/qdebug.h>
#include <QtWidgets/qfiledialog.h>

MainInterface::MainInterface(QWidget* parent)
    : QWidget(parent)
    , hBoxLayout(new QHBoxLayout)
    , lightFieldWidget(new LightFieldWidget)
    , rightContainer(new QWidget)
    , vLayoutForRight(new QVBoxLayout)
    , loadButton(new QPushButton)
    , focalSlider(new QSlider(Qt::Orientation::Horizontal))
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

    focalSlider->setRange(-100, 100);
    focalSlider->setSingleStep(1);
    focalSlider->setSliderPosition(0);
    vLayoutForRight->addWidget(focalSlider);

    apertureSlider->setRange(0, 200);
    apertureSlider->setSingleStep(1);
    apertureSlider->setSliderPosition(50);
    vLayoutForRight->addWidget(apertureSlider);

    loadButton->setText("Load");
    vLayoutForRight->addWidget(loadButton);

    // Signal and slots
    connect(loadButton, SIGNAL(clicked()), this, SLOT(OnLoadButtonClicked()));
    connect(focalSlider, SIGNAL(valueChanged(int)), SLOT(OnFocalSliderValueChanged(int)));
    connect(apertureSlider, SIGNAL(valueChanged(int)), SLOT(OnApertureSliderValueChanged(int)));
}

MainInterface::~MainInterface() {
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
            int ret = sscanf(files[i].toStdString().c_str(),
                             "out_%d_%d_%lf_%lf.png", &row, &col, &camx, &camy);
            if (ret == 4) {
                qDebug("File name parsed: %s", files[i].toStdString().c_str());
                if (row >= 16 || col >= 16) continue;
                viewInfos.emplace_back(dirString + "/" + files[i],
                                       row, col, camx, camy);
                iMax = std::max(iMax, row + 1);
                jMax = std::max(jMax, col + 1);
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

void MainInterface::OnFocalSliderValueChanged(int value) {
    lightFieldWidget->setFocalLength(value / 10.0f);
}

void  MainInterface::OnApertureSliderValueChanged(int value) {
    lightFieldWidget->setApertureSize(value / 10.0f);
}