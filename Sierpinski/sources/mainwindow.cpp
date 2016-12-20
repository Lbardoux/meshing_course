#include <cstdlib>
#include <iostream>

#include <QFileDialog>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gldisplay.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
                                          ui(new Ui::MainWindow)
{
    ui->setupUi(this);
	this->setWindowState(Qt::WindowMaximized);
    this->switchCheckBoxes(false);
	this->ui->saveOff->setEnabled(false);
	this->ui->ReloadButton->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
		case Qt::Key_Escape:
			close();
			break;
		default:
			QMainWindow::keyPressEvent(event);
    }
}

void MainWindow::switchCheckBoxes(bool value)
{
	this->ui->CheckCells->setEnabled(false);
	this->ui->CheckCenters->setEnabled(value);
	this->ui->CheckCircles->setEnabled(false);
	this->ui->CheckCurve->setEnabled(value);
	this->ui->CheckTriangles->setEnabled(value);
	this->ui->CheckPoints->setEnabled(value);
}

// ### Loading actions ########################################################

void MainWindow::on_actionPTS_curve_triggered()
{
    QString file = QFileDialog::getOpenFileName(this, "Load Curve", QDir::currentPath(), "Curve Files (*.pts *.tri)");
	std::string str = file.toStdString();
    if (str != "")
    {
		this->ui->ReloadButton->setEnabled(true);
		this->ui->widget->reset();
		this->switchCheckBoxes(true);
		this->ui->saveOff->setEnabled(false);
        GLDisplay::gasket.mesh.load2DTriangulationFromPts(str);
        GLDisplay::gasket.mesh.Crust();
		GLDisplay::gasket.config.type = CURVE;
		this->loaded = std::move(str);
    }
}

void MainWindow::on_actionPTS_triangulation_triggered()
{
    QString file = QFileDialog::getOpenFileName(this, "Load Triangulation", QDir::currentPath(), "Vertices Files (*.pts *.tri)");
	std::string str = file.toStdString();
    if (str != "")
    {
		this->ui->ReloadButton->setEnabled(true);
		this->switchCheckBoxes(false);
		this->ui->widget->reset();
		this->ui->saveOff->setEnabled(true);
        GLDisplay::gasket.mesh.load2DTriangulationFromPts(str);
		this->loaded = std::move(str);
		GLDisplay::gasket.config.type = TRIANGULATION;
    }
}

void MainWindow::on_actionOFF_mesh_triggered()
{
    QString file = QFileDialog::getOpenFileName(this, "Load Mesh", QDir::currentPath(), "Mesh Files (*.off)");
	std::string str = file.toStdString();
    if (str != "")
    {
		this->ui->ReloadButton->setEnabled(true);
		this->switchCheckBoxes(false);
		this->ui->widget->reset();
		this->ui->saveOff->setEnabled(false);
        GLDisplay::gasket.mesh.loadMeshFromOff(str);
		GLDisplay::gasket.config.type = MESH;
		this->loaded = std::move(str);
    }
}

void MainWindow::on_ReloadButton_released()
{
    if (GLDisplay::gasket.config.type == MESH)
	{
        GLDisplay::gasket.mesh.loadMeshFromOff(this->loaded);
	}
	else if (GLDisplay::gasket.config.type == TRIANGULATION)
	{
        GLDisplay::gasket.mesh.load2DTriangulationFromPts(this->loaded);
	}
	else if (GLDisplay::gasket.config.type == CURVE)
	{
        GLDisplay::gasket.mesh.load2DTriangulationFromPts(this->loaded);
        GLDisplay::gasket.mesh.Crust();
	}
	else if (GLDisplay::gasket.config.type == CONSTRAINTS)
	{
		GLDisplay::gasket.mesh.loadConstraints(this->loaded);
	}
	this->ui->widget->updateGL();
}

// ############################################################################

void MainWindow::on_actionQuit_triggered()
{
    exit(EXIT_SUCCESS);
}

void MainWindow::on_saveOff_released()
{
	QString file = QFileDialog::getOpenFileName(this, "Save As OFF", QDir::currentPath(), "Mesh Files (*.off)");
	std::string str = file.toStdString();
	if (str != "")
	{
		GLDisplay::gasket.mesh.dumpToOff(str);
	}
}

void MainWindow::on_CheckTriangles_stateChanged(int arg1)
{
    if (arg1 == Qt::Unchecked)
	{
		GLDisplay::gasket.config.triangles = false;
	}
	else
	{
		GLDisplay::gasket.config.triangles = true;
	}
	this->ui->widget->updateGL();
}

void MainWindow::on_CheckCurve_stateChanged(int arg1)
{
	if (arg1 == Qt::Unchecked)
	{
		GLDisplay::gasket.config.curve = false;
	}
	else
	{
		GLDisplay::gasket.config.curve = true;
	}
	this->ui->widget->updateGL();
}

void MainWindow::on_CheckCenters_stateChanged(int arg1)
{
	if (arg1 == Qt::Unchecked)
	{
		GLDisplay::gasket.config.centers = false;
	}
	else
	{
		GLDisplay::gasket.config.centers = true;
	}
	this->ui->widget->updateGL();
}

void MainWindow::on_CheckPoints_stateChanged(int arg1)
{
	if (arg1 == Qt::Unchecked)
	{
		GLDisplay::gasket.config.points = false;
	}
	else
	{
		GLDisplay::gasket.config.points = true;
	}
	this->ui->widget->updateGL();
}

void MainWindow::on_CheckCircles_stateChanged(int arg1)
{
	if (arg1 == Qt::Unchecked)
	{
		GLDisplay::gasket.config.circles = false;
	}
	else
	{
		GLDisplay::gasket.config.circles = true;
	}
	this->ui->widget->updateGL();
}

void MainWindow::on_action2D_Constraint_Triangulation_triggered()
{
	QString file = QFileDialog::getOpenFileName(this, "Load Constraint Triangulation", QDir::currentPath(), "Vertices & Constraints Files (*.ctri)");
	std::string str = file.toStdString();
    if (str != "")
    {
		this->ui->ReloadButton->setEnabled(true);
		this->switchCheckBoxes(false);
		this->ui->widget->reset();
		this->ui->saveOff->setEnabled(false);
        GLDisplay::gasket.mesh.loadConstraints(str);
		GLDisplay::gasket.config.type = CONSTRAINTS;
		this->loaded = std::move(str);
    }
}
