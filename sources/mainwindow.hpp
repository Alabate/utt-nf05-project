#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QList>
#include <QEvent>
#include <QKeyEvent>
#include <QString>
#include <stdexcept>

#include "lib/parser.h"
#include "lib/calculable.h"
#include "lib/varNode.h"


namespace Ui {
	class MainWindow;
}

class MainWindow : public QMainWindow
{
		Q_OBJECT

	public:
		explicit MainWindow(QWidget *parent = 0);
		~MainWindow();
        virtual bool event(QEvent *event);

    private:
        void memorySync();
        int currentPos;
        QList<VarNode *> *registry;
        QList<QString> history;
		Ui::MainWindow *ui;

    private slots:
        void deleteVar(QString varName);
        void eval();
};

#endif // MAINWINDOW_HPP
