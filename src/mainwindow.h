#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QScriptEngineDebugger>
#include <QFileDialog>
#include <QDesktopServices>
#include <QUrl>

#include "Caculator.h"
#include "functionhelper.h"
#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexerjavascript.h>
#include <Qsci/qsciapis.h>


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    enum ScriptSource{FORMULA, SCRIPT, FILE};
    QMap<int, ScriptSource> tableInfoList;

    Ui::MainWindow *ui;
    Caculator c;
    QsciScintilla *te_script;
public:
    Q_OBJECT
    void initBtn();
    void initTable();
    void initScriptEditor();

    void clearTable();

    void insertFormula(QString formula, int selectionLength);
    void insertTable(Caculator::Result result, QString formula, ScriptSource scriptSource);

    void showFunctionEditor();
    void hideFunctionEditor();

    void evaluateScript(ScriptSource src);
    void evaluateFile();

    void onCellDoubleClicked(int row);
    void onResetJSEngineBtnClicked();
    void onFunctionEditorBtnClicked();
    void keyPressEvent(QKeyEvent *event);
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:

};
#endif // MAINWINDOW_H
