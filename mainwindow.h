/*
 * Copyright (C) 2013  Guy Rutenberg
 * http://www.guyrutenberg.com
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
