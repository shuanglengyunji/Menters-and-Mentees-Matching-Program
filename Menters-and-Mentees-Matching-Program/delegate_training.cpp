#include "delegate_training.h"
#include <QComboBox>
#include <QPainter>
#include <QApplication>

Delegate_Training::Delegate_Training(QObject *parent)
{
}

QWidget *Delegate_Training::createEditor(QWidget *parent, const QStyleOptionViewItem &/* option */, const QModelIndex &/* index */) const
{
    QComboBox *editor = new QComboBox(parent);
    editor->setFrame(false);
    editor->addItem("y");
    editor->addItem("n");
    return editor;
}

void Delegate_Training::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    QString value = index.model()->data(index, Qt::EditRole).toString();

    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    if (value == "y"){
        comboBox->setCurrentText("y");
    }
    else {
        comboBox->setCurrentText("n");
    }
}

void Delegate_Training::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const
{
    QComboBox *comboBox = static_cast<QComboBox*>(editor);
    QString value = comboBox->currentText();

    model->setData(index, value, Qt::EditRole);
}

void Delegate_Training::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &/* index */) const
{
    editor->setGeometry(option.rect);
}


