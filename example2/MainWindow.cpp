#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qobject_cast<GLView*>(centralWidget())->setMainWindow(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
