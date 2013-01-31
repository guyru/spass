#include <algorithm>
#include <sstream>
#include <QDebug>
#include <QClipboard>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "audio_random.h"
#include "audio_random_alsa.h"
#include "spass_utils.h"

const int DEFAULT_MAX_STRENGTH = 128;
extern "C" {char* getDiceWd (int n);}

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	ui->strengthBar->setMaximum(DEFAULT_MAX_STRENGTH);
	AudioRandom::getInstance()->setBackend(AudioRandomAlsa::getInstance());
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
	stringstream output;
	AudioRandom *arand = AudioRandom::getInstance();
	int length = ui->lengthSpinBox->value();

	if (ui->tabWidget->currentWidget() == ui->tabPassword) {
		string strip = "";
		for (const auto& widget : ui->tabPassword->findChildren<QCheckBox *>()) {
			if (widget->checkState())
				strip += widget->property("strip").toString().toStdString();
		}
		qDebug() << "Strip: " << strip.c_str() ;
		if (!strip.empty())
			generate_password(length, strip, output);

	} else {
		generate_passphrase(length, output);
	}

	QString qs_output = QString::fromUtf8(output.str().c_str());
	ui->outputEdit->setText(qs_output);
}


void MainWindow::updateStrength()
{
	double bits_per_symbol = 4;
	int total_bits = bits_per_symbol * ui->lengthSpinBox->value();
	ui->strengthBar->setMaximum(std::max(DEFAULT_MAX_STRENGTH, total_bits));
	ui->strengthBar->setValue(total_bits);
}
