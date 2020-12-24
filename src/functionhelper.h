#ifndef FUNCTIONHELPER_H
#define FUNCTIONHELPER_H

#include "Caculator.h"

#include <QDialog>
#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexerjavascript.h>
#include <Qsci/qsciapis.h>

namespace Ui {
class FunctionHelper;
}

class FunctionHelper : public QDialog
{
    Q_OBJECT
    QsciScintilla *te_script;

    QList<QPair<QString, QString>> functionList;

public:
    explicit FunctionHelper(QList<QPair<QString, QString>> functionList, QWidget *parent = nullptr);
    ~FunctionHelper();

    void initScriptViewer();

    static FunctionHelper *functionHelperBuilder(QList<QPair<QString, QString>> functionList);

private:
    Ui::FunctionHelper *ui;
};

#endif // FUNCTIONHELPER_H
