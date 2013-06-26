#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "Engine.h"
#include "QDebug"
#include "QMessageBox"
#include <QFile>
#include <QFileDialog>
#include "Situation.h"
#include "GuiSettings.h"

extern Engine& en;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    timer = new QTimer();
    ui->glWidget->setMainWindow(this);

    /// Collision interpretation modes.
    ui->optionCollisionCombo->clear();
    ui->optionCollisionCombo->addItem(tr("All collisions"));
    ui->optionCollisionCombo->addItem(tr("First collision"));
    ui->optionCollisionCombo->setCurrentIndex(0);

    /// Character shapes display options.
    ui->optionShowPlayerSolidCombo->clear();
    ui->optionShowPlayerSolidCombo->addItem(tr("Hide"));
    ui->optionShowPlayerSolidCombo->addItem(tr("Wireframe"));
    ui->optionShowPlayerSolidCombo->addItem(tr("Fill"));
    ui->optionShowPlayerSolidCombo->setCurrentIndex(0);

    /// Object shapes display options.
    ui->optionShowObjectSolidCombo->clear();
    ui->optionShowObjectSolidCombo->addItem(tr("Wireframe"));
    ui->optionShowObjectSolidCombo->addItem(tr("Fill"));
    ui->optionShowObjectSolidCombo->setCurrentIndex(0);

    /// Character shapes for physical model.
    ui->optionPlayerShapeCombo->clear();
    ui->optionPlayerShapeCombo->addItem(tr("Point"));
    ui->optionPlayerShapeCombo->addItem(tr("Line segment"));
    ui->optionPlayerShapeCombo->addItem(tr("Triangle"));
    ui->optionPlayerShapeCombo->setCurrentIndex(1);

    /// Application logic criterias.
    ui->optionCriteriaCombo->clear();
    ui->optionCriteriaCombo->addItem(tr("Consider all objects"));
    ui->optionCriteriaCombo->addItem(tr("Consider other than green objects"));
    ui->optionCriteriaCombo->addItem(tr("Consider green objects only"));
    ui->optionCriteriaCombo->setCurrentIndex(0);

    /// connections - comboboxes
    connect(ui->optionCollisionCombo,SIGNAL(currentIndexChanged(int)), this, SLOT(collisionModeChanged()));
    connect(ui->optionCriteriaCombo,SIGNAL(currentIndexChanged(int)), this, SLOT(collisionModeChanged()));
    connect(ui->optionShowPlayerSolidCombo,SIGNAL(currentIndexChanged(int)), this, SLOT(optionsFromGui()));
    connect(ui->optionShowObjectSolidCombo,SIGNAL(currentIndexChanged(int)), this, SLOT(optionsFromGui()));
    connect(ui->optionPlayerShapeCombo,SIGNAL(currentIndexChanged(int)), this, SLOT(collisionMeshChanged()));

    /// connections - buttons
    connect(ui->optionVectorCheck,SIGNAL(clicked()), this, SLOT(optionsFromGui()));
    connect(ui->optionPlaneCheck,SIGNAL(clicked()), this, SLOT(optionsFromGui()));
    connect(ui->playerPosCenterButton,SIGNAL(clicked()), this, SLOT(centerPlayer()));

    /// connections - edits
    connect(ui->playerPosXEdit,SIGNAL(textChanged(QString)), this, SLOT(playerPosFromGui()));
    connect(ui->playerPosYEdit,SIGNAL(textChanged(QString)), this, SLOT(playerPosFromGui()));
    connect(ui->playerPosZEdit,SIGNAL(textChanged(QString)), this, SLOT(playerPosFromGui()));
    connect(ui->playerPosRadEdit,SIGNAL(valueChanged(double)), this, SLOT(playerPosFromGui()));
    connect(ui->optionVectorLengthEdit,SIGNAL(valueChanged(double)), this, SLOT(playerPosFromGui()));
    connect(ui->optionVectorBackEdit,SIGNAL(valueChanged(double)), this, SLOT(playerPosFromGui()));

    /// connections - menu actions
    connect(ui->actionExit,SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->actionAbout,SIGNAL(triggered()), this, SLOT(about()));
    connect(ui->actionAboutQt,SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    connect(ui->actionLoad,SIGNAL(triggered()), this, SLOT(loadSituation()));
    connect(ui->actionSave,SIGNAL(triggered()), this, SLOT(saveSituation()));
    connect(ui->actionCaptureSituation,SIGNAL(triggered()), this, SLOT(captureSituation()));
    connect(ui->actionPlaySequence,SIGNAL(triggered()), this, SLOT(playSequence()));
    connect(ui->actionToFirstSituation,SIGNAL(triggered()), this, SLOT(toFirstSituation()));
    connect(ui->actionNextSituation,SIGNAL(triggered()), this, SLOT(nextSituation()));
    connect(ui->actionPreviousSituation,SIGNAL(triggered()), this, SLOT(previousSituation()));

    /// connections - timer timeout
    connect(timer, SIGNAL(timeout()), this, SLOT(sequenceTimeout()));

    /// connections - from glWidget
    connect(ui->glWidget, SIGNAL(playerPosChanged()), this, SLOT(playerPosToGui()));
    connect(ui->glWidget, SIGNAL(glInitialized()), this, SLOT(collisionModeChanged()));

    ui->collisionTree->header()->resizeSections(QHeaderView::ResizeToContents);
    optionsFromGui();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::guiBlock(bool blocked)
{
    ui->optionShowPlayerSolidCombo->blockSignals(blocked);
    ui->optionShowObjectSolidCombo->blockSignals(blocked);
    ui->optionVectorCheck->blockSignals(blocked);
    ui->optionCollisionCombo->blockSignals(blocked);
    ui->optionPlaneCheck->blockSignals(blocked);
    ui->optionCriteriaCombo->blockSignals(blocked);
}

void MainWindow::playerPosBlock(bool blocked)
{
    ui->playerPosXEdit->blockSignals(blocked);
    ui->playerPosYEdit->blockSignals(blocked);
    ui->playerPosZEdit->blockSignals(blocked);
    ui->playerPosRadEdit->blockSignals(blocked);
    ui->optionVectorLengthEdit->blockSignals(blocked);
    ui->optionVectorBackEdit->blockSignals(blocked);
    ui->optionPlayerShapeCombo->blockSignals(blocked);
}

void MainWindow::playerPosToGui()
{
    playerPosBlock(true);
    ui->playerPosXEdit->setText(QString("%1").arg(en.pl->pos.x));
    ui->playerPosYEdit->setText(QString("%1").arg(en.pl->pos.y));
    ui->playerPosZEdit->setText(QString("%1").arg(en.pl->pos.z));
    ui->playerPosRadEdit->setValue(en.pl->radXZ);
    playerPosBlock(false);
    ui->glWidget->calculateNextMove();
}

QTreeWidgetItem *MainWindow::getTreeItemId()
{
    return ui->collisionTree->itemAt(ui->collisionTree->viewport()->mapFromGlobal(QCursor::pos()));//ui->collisionTree->hoverAt();
}

QPair<float, Object::Id> MainWindow::getItemData()
{
    QTreeWidgetItem* item = getTreeItemId();
    QPair<float, Object::Id> data;
    if(item != NULL)
    {
        data.first = item->text(2).toFloat();
        data.second = item->data(1, Qt::UserRole).toFloat();
    }
    else
    {
        /// Tree element not found.

        data.first = 0.0f;
        data.second = OBJECT_NONE;
    }
    return data;
}

void MainWindow::generateCollisionTree()
{
    ui->collisionTree->blockSignals(true);

    ui->collisionTree->clear();
    switch(en.getGui().logMode)
    {
    case 0:
    {
        /// Display all collision results.

        QMultiMap<float, EnvelopeCollide::Result<SolidGame> >::iterator it;
        for (it = en.resAll.begin(); it != en.resAll.end(); ++it) {
            EnvelopeCollide::Result<SolidGame>& val = it.value();
            QTreeWidgetItem* item = new QTreeWidgetItem();
            ui->collisionTree->addTopLevelItem(item);
            if(val.mode == EnvelopeCollide::EENTER)
                item->setText(0, "enter");
            if(val.mode == EnvelopeCollide::EINSIDE)
                item->setText(0, "inside");
            if(val.mode == EnvelopeCollide::EEXIT)
                item->setText(0, "exit");
            item->setText(1, QString("%1").arg(val.data.id));
            item->setData(1, Qt::UserRole, val.data.id);
            item->setText(2, QString("%1").arg(it.key()));
        }
    }
    break;
    case 1:
    {
        /// Display earliest collision.

        /// front
        QTreeWidgetItem* item = new QTreeWidgetItem();
        ui->collisionTree->addTopLevelItem(item);
        item->setText(0, "Forward");
        if(en.resFirst.forwardb)
        {
            item->setText(1, QString("%1").arg(en.resFirst.forward.data.id));
            item->setText(2, QString("%1").arg(en.resFirst.forwardf));
        }
        else
            item->setText(1, "no collision");
        item->setData(0, Qt::UserRole, en.resFirst.forwardb);
        item->setData(1, Qt::UserRole, en.resFirst.forward.data.id);

        /// back
        item = new QTreeWidgetItem();
        ui->collisionTree->addTopLevelItem(item);
        item->setText(0, "Backward");
        if(en.resFirst.backb)
        {
            item->setText(1, QString("%1").arg(en.resFirst.back.data.id));
            item->setText(2, QString("%1").arg(en.resFirst.backf));
        }
        else
            item->setText(1, "no collision");
        item->setData(0, Qt::UserRole, en.resFirst.backb);
        item->setData(1, Qt::UserRole, en.resFirst.back.data.id);
    }
    break;
    default:
        break;
    }

    ui->collisionTree->blockSignals(false);
}

void MainWindow::collisionModeChanged()
{
    playerPosBlock(true);
    if(ui->optionCollisionCombo->currentIndex() == 0)
    {
        /// In all collisions mode backward value of movement vector must equals zero.
        ui->optionVectorBackEdit->setValue(0.0);
        ui->optionVectorBackEdit->setEnabled(false);
    }
    else
    {
        ui->optionVectorBackEdit->setEnabled(true);
    }
    playerPosBlock(false);
    optionsFromGui();
    playerPosFromGui();
}

void MainWindow::collisionMeshChanged()
{
    playerPosBlock(true);
    if(ui->optionPlayerShapeCombo->currentIndex() == 0)
        en.pl->shape[0] = en.pl->pointShape;
    else if(ui->optionPlayerShapeCombo->currentIndex() == 1)
        en.pl->shape[0] = en.pl->segmentShape;
    else
        en.pl->shape[0] = en.pl->triangleShape;
    playerPosBlock(false);
    ui->glWidget->calculateNextMove();
}

void MainWindow::about()
{
    QMessageBox::about(this, tr("CollisionVisualisator"), tr("Author: Przemysław Bągard"));
}

void MainWindow::loadSituation()
{
    QString filename = QFileDialog::getOpenFileName(this, "Load situations");

    en.loadSituations(filename);
    en.currentSituation = 0;
    if(en.situations.size() > 0)
        toFirstSituation();
}

void MainWindow::saveSituation()
{
    QString filename = QFileDialog::getSaveFileName(this, "Save situations");
    en.saveSituations(filename);
}

void MainWindow::captureSituation()
{
    Situation situation;
    situation.gui = en.getGui();
    situation.pos = en.pl->pos;
    situation.rad = en.pl->radXZ;
    situation.camera = en.camera.pos;
    situation.cameraRad = en.camera.vAngle;
    situation.frontVecLenght = en.pl->vectorFrontLength;
    situation.backVecLenght = en.pl->vectorBackLength;
    situation.playerShape = ui->optionPlayerShapeCombo->currentIndex();
    situation.time = 5.0f;
    en.situations.push_back(situation);
    QString strSekw = tr("Situation recorded. Sequence position: ");
    QString strMsg = QString("%1%2").arg(strSekw).arg(en.situations.size()-1);
    statusBar()->showMessage(strMsg, 3000);
}

void MainWindow::playSequence()
{
    if(!en.playSequence)
    {
        if(en.currentSituation < en.situations.size())
        {
            /// Enables playing situation sequence.

            ui->actionPlaySequence->setText(QString(tr("Stop sequence")));
            QString strSekw = tr("Sequence started. Sequence position: ");
            QString strMsg = QString("%1%2").arg(strSekw).arg(en.currentSituation);
            statusBar()->showMessage(strMsg, en.situations[en.currentSituation].time*1000);
            en.playSequence = true;
            timer->setSingleShot(true);
            situationToGui(en.situations[en.currentSituation]);
            timer->start(en.situations[en.currentSituation].time*1000);
        }
        else
        {
            /// Sequence is empty.
            statusBar()->showMessage(QString(tr("Sequence is finished")), 3000);
        }
    }
    else
    {
        /// Disables playing situation sequence.

        statusBar()->showMessage(QString(tr("Sequence stopped")), 3000);
        en.playSequence = false;
        timer->stop();
        ui->actionPlaySequence->setText(QString(tr("Play sequence")));
    }
}

void MainWindow::toFirstSituation()
{
    en.currentSituation = 0;
    statusBar()->showMessage(QString("Rewinded to first situation in sequence"), 3000);
    if(en.currentSituation < en.situations.size())
    {
        /// Sequence has at least one situation.

        situationToGui(en.situations[en.currentSituation]);
        if(en.playSequence)
        {
            /// During playing sequence. Timer will be reseted.

            timer->setSingleShot(true);
            timer->stop();
            timer->start(en.situations[en.currentSituation].time*1000);
        }
    }
}

void MainWindow::sequenceTimeout()
{
    ++en.currentSituation;
    if(en.currentSituation < en.situations.size())
    {
        /// Enters next situation from sequence.

        situationToGui(en.situations[en.currentSituation]);
        timer->setSingleShot(true);
        if(en.playSequence)
            timer->start(en.situations[en.currentSituation].time*1000);
        QString strSekw = tr("Sequence position: ");
        QString strMsg = QString("%1%2").arg(strSekw).arg(en.currentSituation);
        statusBar()->showMessage(strMsg, en.situations[en.currentSituation].time*1000);
    }
    else
    {
        /// There is no more situations in sequence

        statusBar()->showMessage("Sequence stopped", 3000);
     //   en.currentSituation = 0;
        en.playSequence = false;
        ui->actionPlaySequence->setText(QString(tr("Play sequence")));
    }
}

void MainWindow::nextSituation()
{
    if(en.currentSituation < en.situations.size()-1)
    {
        if(en.playSequence)
            timer->stop();

        /// 'sequenceTimeout' increments current situation index.
        /// It performs also all necessary conditions like if situation with such index exists etc.
        sequenceTimeout();
    }
}

void MainWindow::previousSituation()
{
    if(en.currentSituation > 0)
    {
        if(en.playSequence)
            timer->stop();

        /// Substract 2, because 'sequenceTimeout' will add 1.
        /// Function performs also all necessary conditions like if situation with such index exists etc.
        en.currentSituation -= 2;
        sequenceTimeout();
    }
}

void MainWindow::situationToGui(const Situation& situation)
{
    en.setGui(situation.gui);
    en.pl->pos = situation.pos;
    en.pl->radXZ = situation.rad;
    en.camera.pos = situation.camera;
    en.camera.vAngle = situation.cameraRad;
    en.pl->vectorFrontLength = situation.frontVecLenght;
    en.pl->vectorBackLength = situation.backVecLenght;

    guiBlock(true);
    playerPosBlock(true);
    ui->optionCollisionCombo->setCurrentIndex(situation.gui.logMode);
    ui->optionShowPlayerSolidCombo->setCurrentIndex(situation.gui.showPlayerSolid);
    ui->optionShowObjectSolidCombo->setCurrentIndex(situation.gui.showObjectSolid);
    ui->optionVectorLengthEdit->setValue(en.pl->vectorFrontLength);
    ui->optionVectorBackEdit->setValue(en.pl->vectorBackLength);
    ui->optionVectorCheck->setChecked(situation.gui.showMoveVector);
    ui->optionPlaneCheck->setChecked(situation.gui.showPlane);
    ui->optionPlayerShapeCombo->setCurrentIndex(situation.playerShape);
    ui->optionCriteriaCombo->setCurrentIndex(situation.gui.criteriaMode);
    playerPosBlock(false);
    guiBlock(false);
    playerPosToGui();
    collisionMeshChanged();
    collisionModeChanged();
}

void MainWindow::optionsFromGui()
{
    guiBlock(true);
    GuiSettings gui = en.getGui();
    gui.showPlayerSolid = ui->optionShowPlayerSolidCombo->currentIndex();
    gui.showObjectSolid = ui->optionShowObjectSolidCombo->currentIndex();
    gui.showMoveVector = ui->optionVectorCheck->isChecked();
    gui.showPlane = ui->optionPlaneCheck->isChecked();
    gui.logMode = ui->optionCollisionCombo->currentIndex();
    gui.criteriaMode = ui->optionCriteriaCombo->currentIndex();
    en.setGui(gui);
    guiBlock(false);
}

void MainWindow::playerPosFromGui()
{
    playerPosBlock(true);
    en.pl->pos = Point(ui->playerPosXEdit->text().toFloat(), ui->playerPosYEdit->text().toFloat(), ui->playerPosZEdit->text().toFloat());
    en.pl->radXZ = ui->playerPosRadEdit->value();
    en.pl->vectorFrontLength = ui->optionVectorLengthEdit->value();
    en.pl->vectorBackLength = ui->optionVectorBackEdit->value();
    playerPosBlock(false);
    ui->glWidget->calculateNextMove();
}

void MainWindow::centerPlayer()
{
    en.pl->pos = Point();
    en.pl->radXZ = 0.0f;
    playerPosToGui();
}
