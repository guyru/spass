#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>
#include <QMainWindow>

namespace Ui {
	class MainWindow;
}

class SpassStrip;

class MainWindow : public QMainWindow
{
	Q_OBJECT
	
public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

public slots:
	void updateStrength();
	void updateStrip();
	
private slots:
	void on_copyButton_clicked();
	void on_generateButton_clicked();

private:
	Ui::MainWindow *ui;
	std::unique_ptr<SpassStrip> strip;

};

#endif // MAINWINDOW_H
