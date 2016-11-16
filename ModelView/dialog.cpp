#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::Dialog)
{
	ui->setupUi(this);
	QStringList list;
    //Below is the short list of named features


<<<<<<< Updated upstream
    list << "aapje" << "noten" << "mies" << "toto" << "tata!" << "blhho";
=======
    list << "aap" << "noot" << "mies" << "toto" << "tata!" << "blhho";
>>>>>>> Stashed changes
	_model = new QStringListModel(this);
	_model->setStringList(list);
	ui->comboBox->setModel(_model);
	ui->listView->setModel(_model);
	ui->listView->setEditTriggers(QAbstractItemView::AnyKeyPressed | QAbstractItemView::DoubleClicked);

}

Dialog::~Dialog()
{
	delete ui;
}

void Dialog::on_pushButton_clicked()
{
    //Add
	int row = _model->rowCount();
	_model->insertRows(row, 1);
	QModelIndex index = _model->index(row);
	ui->listView->setCurrentIndex(index);
	ui->listView->edit(index);


}

void Dialog::on_pushButton_2_clicked()
{
    //Insert
	int row = ui->listView->currentIndex().row();
    _model->insertRows(row, 1);
	QModelIndex index = _model->index(row);
	ui->listView->setCurrentIndex(index);
	ui->listView->edit(index);

}

void Dialog::on_pushButton_3_clicked()
{
    //Deleted
	_model->removeRows(ui->listView->currentIndex().row(), 1);
}
