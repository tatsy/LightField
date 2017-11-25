#include "maingui.h"

#include <iostream>

#include <QtWidgets>
#include <qdebug.h>

class MainGui::Ui : public QWidget {
public:
    explicit Ui(QWidget *parent = nullptr)
        : QWidget(parent) {

        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
        setSizePolicy(sizePolicy);

        layout = new QVBoxLayout(this);
        layout->setAlignment(Qt::AlignTop);
        setLayout(layout);

        loadButton = new QPushButton("Load", this);
        loadButton->setText("Load");
        layout->addWidget(loadButton);

        focusLabel = new QLabel("focus point", this);
        layout->addWidget(focusLabel);

        focusEdit = new QLineEdit("0.00", this);
        layout->addWidget(focusEdit);

        focusSlider = new QSlider(Qt::Orientation::Horizontal, this);
        focusSlider->setRange(-100, 100);
        focusSlider->setSingleStep(1);
        focusSlider->setSliderPosition(0);
        layout->addWidget(focusSlider);

        apertureLabel = new QLabel("aperture size", this);
        layout->addWidget(apertureLabel);

        apertureEdit = new QLineEdit("5.00", this);
        layout->addWidget(apertureEdit);

        apertureSlider = new QSlider(Qt::Orientation::Horizontal, this);
        apertureSlider->setRange(0, 200);
        apertureSlider->setSingleStep(1);
        apertureSlider->setSliderPosition(50);
        layout->addWidget(apertureSlider);
    }

    QSize sizeHint() const {
        return QSize(256, 512);
    }

    QVBoxLayout *layout;

    QPushButton* loadButton;
    QLabel* focusLabel;
    QLabel* apertureLabel;
    QLineEdit* focusEdit;
    QLineEdit* apertureEdit;
    QSlider* focusSlider;
    QSlider* apertureSlider;
};

MainGui::MainGui(QWidget *parent)
    : QMainWindow(parent)
    , viewInfos()
    , isClicking(false)
    , mousePosition(0, 0) {
    // Font setting
    setFont(QFont("Arial"));

    // Main container
    mainWidget = new QWidget(this);
    mainLayout = new QHBoxLayout(mainWidget);
    mainWidget->setLayout(mainLayout);
    setCentralWidget(mainWidget);

    // Viewer
    viewer = new LightFieldWidget(mainWidget);
    mainLayout->addWidget(viewer);

    // UI
    ui = new Ui(mainWidget);
    mainLayout->addWidget(ui);

    // Signal and slots
    connect(ui->loadButton, SIGNAL(clicked()), this, SLOT(OnLoadButtonClicked()));
    connect(ui->focusSlider, SIGNAL(valueChanged(int)), SLOT(OnFocusSliderValueChanged(int)));
    connect(ui->apertureSlider, SIGNAL(valueChanged(int)), SLOT(OnApertureSliderValueChanged(int)));
    connect(viewer, SIGNAL(frameSwapped()), this, SLOT(OnFrameSwapped()));
}

MainGui::~MainGui() {
}

void MainGui::OnLoadButtonClicked() {
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
        qDebug("%d images necessary but %zu provided!!", iMax * jMax, viewInfos.size());
        return;
    }

    // If # of images is OK, set images to light field widget
    viewer->setLightField(viewInfos, iMax, jMax);
}

void MainGui::OnFocusSliderValueChanged(int value) {
    const float newValue = value / 10.0f;
    viewer->setFocusPoint(newValue);
    ui->focusEdit->setText(QString::number(newValue, 'f', 2));
}

void MainGui::OnApertureSliderValueChanged(int value) {
    const float newValue = value / 10.0f;
    viewer->setApertureSize(newValue);
    ui->apertureEdit->setText(QString::number(newValue, 'f', 2));
}

void MainGui::OnFrameSwapped() {
static bool isStarted = false;
    static QElapsedTimer timer;
    static long long lastTime;
    static int frameCount = 0;

    if (!isStarted) {
        isStarted = true;
        frameCount = 0;
        timer.start();
    } else if (timer.elapsed() > 500) {
        long long currentTime = timer.elapsed();
        double fps = frameCount * 1000.0 / (currentTime - lastTime);
        setWindowTitle(QString("Light Field Viewer: %1 fps").arg(fps, 0, 'f', 1));
        timer.restart();
        frameCount = 0;
    }
    frameCount += 1;
}