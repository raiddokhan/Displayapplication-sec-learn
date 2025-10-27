#include "windows/rootwidget.h"
#include "ui_rootwidget.h"


extern ConfigLoader configLoaded;

RootWidget::RootWidget(QWidget *parent) : QWidget(parent),
                                          ui(new Ui::RootWidget)
{
    ui->setupUi(this);

    segment = new Segment;

    ui->stackedWidget->addWidget(segment);

    connect(ui->buttonBack, &QPushButton::released, [this]
            { setPage(0); });

    connect(ui->buttonSettings, &QPushButton::released, [this]
            { setPage(1); });

    // connect(ui->buttonMore, &QPushButton::released, [this]
    //         { setPage(2); });
    // connect(ui->comboBox, &QComboBox::currentTextChanged, ui->labelSettingsData, &QLabel::setText);

    if(true){
        ui->buttonSettings->setVisible(false);
    }

    // connect(segment, &Segment::transferString, ui->labelMoreData, &QLabel::setText);
    connect(segment, &Segment::changePage, this, &RootWidget::setPage);
    // Connect the QDial valueChanged signal to a custom slot
    connect(ui->dial, &QDial::valueChanged, this, &RootWidget::onDialValueChanged);

    // Example usage of the displayImage function
    displayImage("/home/mannigel/Projects/Sec/ui_elements/images/marble.png");  // Replace with your image path

}

void RootWidget::onDialValueChanged(int value)
{
    ui->lcdNumber->display(value);
}

void RootWidget::generateList(int index){
    auto table = vtkSmartPointer<vtkTable>::New();
    table->AddColumn(vtkSmartPointer<vtkDoubleArray>::New());
    table->AddColumn(vtkSmartPointer<vtkDoubleArray>::New());
    table->GetColumn(0)->SetName("X");
    table->GetColumn(1)->SetName("Y");

    table->SetNumberOfRows(100);
    for(int i = 0; i < 100; ++i)
    {
        table->SetValue(i, 0, i);
        table->SetValue(i, 1, i*i);
    }

    // Create chart, view and widget

    auto chart = vtkSmartPointer<vtkChartXY>::New();
    auto line = chart->AddPlot(vtkChart::LINE);
    line->SetInputData(table, 0, 1);

    auto view = vtkSmartPointer<vtkContextView>::New();
    view->GetScene()->AddItem(chart);

    auto widget = new QVTKWidget();
    view->SetInteractor(widget->GetInteractor());
    widget->SetRenderWindow(view->GetRenderWindow());

}

void RootWidget::changeProfileDialChanger(int start, int end, double step)
{
    // Set the range for the QDial
    ui->dial->setRange(start, end); // Example range from 0 to 200
    ui->dial->setSingleStep(step);
}

void RootWidget::updateLCDNumber(int pos)
{
    ui->lcdNumber->display(pos * selectorScaling);
    ui->dial->setValue(pos); // Optionally, update the dial as well
}

void RootWidget::setSelectorScaling(double scaling)
{
    selectorScaling = scaling;
}
RootWidget::~RootWidget()
{
    delete ui;
}

void RootWidget::setPage(int index)
{
    ui->stackedWidget->setCurrentIndex(index);
}

void RootWidget::displayImage(const QString& imagePath)
{
    // // Load an image using vtkPNGReader
    // vtkSmartPointer<vtkPNGReader> pngReader = vtkSmartPointer<vtkPNGReader>::New();
    // pngReader->SetFileName(imagePath.toStdString().c_str());
    // pngReader->Update();

    // // Set the image to the viewer
    // imageViewer->SetInputConnection(pngReader->GetOutputPort());

    // // Set up the QVTKOpenGLNativeWidget
    // imageViewer->SetupInteractor(ui->qvtkWidget->GetRenderWindow()->GetInteractor());
    // ui->qvtkWidget->SetRenderWindow(imageViewer->GetRenderWindow());

    // // Render the image
    // imageViewer->Render();
}