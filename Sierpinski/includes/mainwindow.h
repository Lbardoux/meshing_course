#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <string>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
    public:
        explicit MainWindow(QWidget *parent = 0);
        ~MainWindow();
		
		void keyPressEvent(QKeyEvent *event);
        
    private slots:
        void on_actionPTS_curve_triggered();
        void on_actionPTS_triangulation_triggered();
        void on_actionOFF_mesh_triggered();

        void on_actionQuit_triggered();
		
		void on_saveOff_released();
		
		void on_CheckTriangles_stateChanged(int arg1);
		
		void on_CheckCurve_stateChanged(int arg1);
		
		void on_CheckCenters_stateChanged(int arg1);
		
		void on_CheckPoints_stateChanged(int arg1);
		
		void on_CheckCircles_stateChanged(int arg1);
		
		void on_ReloadButton_released();
		
		void on_action2D_Constraint_Triangulation_triggered();
		
private:
        Ui::MainWindow *ui;
		void switchCheckBoxes(bool value);
		std::string loaded;
};

#endif
