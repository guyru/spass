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

#include <algorithm>
#include <sstream>
#include <QDebug>
#include <QClipboard>
#include <cmath>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "audio_random.h"
#include "audio_random_alsa.h"
#include "audio_random_portaudio.h"
#include "audio_random_oss.h"
#include "spass_utils.h"
#include "config.h"


const int DEFAULT_MAX_STRENGTH = 128;
extern "C" char * Dicewds8k[1<<13];

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	ui->strengthBar->setMaximum(DEFAULT_MAX_STRENGTH);
	setWindowTitle(PACKAGE_STRING);
	AudioRandom::getInstance()->setBackend(AUDIO_RANDOM_BACKEND::getInstance());
	updateStrip();
}

MainWindow::~MainWindow()
{
	delete ui;
}


void MainWindow::on_copyButton_clicked()
{
	QApplication::clipboard()->setText(ui->outputEdit->text());
}


void MainWindow::on_generateButton_clicked()
{
	int length = ui->lengthSpinBox->value();
	string out;

	strip->generatePassword(length, out);
	ui->outputEdit->setText(QString::fromUtf8(out.c_str()));
}


void MainWindow::updateStrength()
{
	double bits_per_symbol = max(log2(strip->strip.size()), 0.0);
	qDebug() << "Bits" << bits_per_symbol;
	int total_bits = bits_per_symbol * ui->lengthSpinBox->value();
	ui->strengthBar->setMaximum(std::max(DEFAULT_MAX_STRENGTH, total_bits));
	ui->strengthBar->setValue(total_bits);
}


void MainWindow::updateStrip()
{
	strip.reset(new SpassStrip);
	if (ui->tabWidget->currentWidget() == ui->tabPassword) {
		strip->separator = "";
		for (const auto& widget : ui->tabPassword->findChildren<QCheckBox *>()) {
			if (widget->isChecked()) {
				const QByteArray strip_chars =  widget->property("strip").toString().toLatin1();
				strip->strip.reserve(strip->strip.size() + strip_chars.size());
				const char *i = strip_chars.data();
				for (const char *i = strip_chars.data(); *i; i++)
					strip->strip.push_back(string(1, *i));
			}
		}
	} else if (ui->tabWidget->currentWidget() == ui->tabPassphrase) {
		strip->separator = " ";
		strip->strip.assign(std::begin(Dicewds8k), std::end(Dicewds8k));
	} else if (ui->tabWidget->currentWidget() == ui->tabCustom) {
		// first copy the custom strip into the "strip" property of
		// the corresponding radioButton.
		ui->radioButtonCustom->setProperty("strip", ui->lineEditCustom->text());

		strip->separator = "";
		for (const auto& widget : ui->tabCustom->findChildren<QRadioButton *>()) {
			if (!widget->isChecked())
				continue;

			const QByteArray strip_chars =  widget->property("strip").toString().toLatin1();
			strip->strip.clear();
			for (const char *i = strip_chars.data(); *i; i++)
					strip->strip.push_back(string(1, *i));
			break;
		}
	} else {
		qWarning() << "MainWindow::updateStrip: Unknown tab";
	}

	updateStrength();
}
