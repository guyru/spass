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
extern char * Dicewds8k[1<<13];

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	ui->strengthBar->setMaximum(DEFAULT_MAX_STRENGTH);
	AudioRandom::getInstance()->setBackend(AudioRandomAlsa::getInstance());
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
			if (widget->checkState()) {
				const string strip_string = widget->property("strip").toString().toStdString();
				strip->strip.reserve(strip->strip.size() + strip_string.size());
				for (const auto &i : strip_string)
					strip->strip.push_back(string(1, i));
			}
		}
	} else {
		strip->separator = " ";
		strip->strip.assign(std::begin(Dicewds8k), std::end(Dicewds8k));
	}
	updateStrength();
}
