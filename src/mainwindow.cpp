#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    hideFunctionEditor();
    initBtn();
    initTable();
    initScriptEditor();
    setWindowTitle("JS Core Caculator");
}

void MainWindow::showFunctionEditor()
{
        ui->widget_scriptEditor->setVisible(true);
        this->resize(width() * 2, this->geometry().height());
}

void MainWindow::hideFunctionEditor()
{
        ui->widget_scriptEditor->setVisible(false);
        this->resize(width() / 2, this->geometry().height());
}

void MainWindow::insertFormula(QString formula, int selectionLength)
{
    ui->et_formula->insert(formula);
    ui->et_formula->cursorBackward(false, 1);
    ui->et_formula->cursorBackward(true, selectionLength);
}

void MainWindow::insertTable(Caculator::Result res, QString formula, ScriptSource scriptSource)
{
    int curRow = ui->tb_resultHistory->rowCount();
    ui->tb_resultHistory->insertRow(curRow);
    QTableWidgetItem *resultItem = new QTableWidgetItem(QString("%1").arg(res.first));
    ui->tb_resultHistory->setItem(curRow, 0, resultItem);
    ui->tb_resultHistory->setItem(curRow, 1, new QTableWidgetItem(formula));

    if(res.second == Caculator::EvaluateState::RunningState)
        resultItem->setTextColor(QColor(255, 0, 0));
    else if(res.second == Caculator::EvaluateState::SuspendedState)
        resultItem->setTextColor(QColor(0, 200, 0));

    tableInfoList[curRow] = scriptSource;
    ui->tb_resultHistory->scrollToBottom();
}

void MainWindow::evaluateScript(ScriptSource src)
{
    QString formula = (src == ScriptSource::FORMULA ? ui->et_formula->text() : te_script->text());
    Caculator::Result res = c.evaluateScript(formula);

    if(src == ScriptSource::FORMULA)
        ui->et_formula->setText(QString("%1").arg(res.first));

    insertTable(res, formula, src);
}

void MainWindow::evaluateFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Load .js File", QString(), "JavaScript File(*.js)");
    Caculator::Result res = c.evaluateFile(fileName);
    insertTable(res, fileName, ScriptSource::FILE);
}

void MainWindow::initBtn()
{
    connect(ui->btn_enter, &QPushButton::clicked, [this]() { evaluateScript(ScriptSource::FORMULA); });
    connect(ui->btn_scriptEnter, &QPushButton::clicked, [this]() { evaluateScript(ScriptSource::SCRIPT); });

    connect(ui->btn_sin, &QPushButton::clicked, ui->et_formula, [this]() { insertFormula("sin(x)", 1); });
    connect(ui->btn_cos, &QPushButton::clicked, ui->et_formula, [this]() { insertFormula("cos(x)", 1); });
    connect(ui->btn_tan, &QPushButton::clicked, ui->et_formula, [this]() { insertFormula("tan(x)", 1); });
    connect(ui->btn_asin, &QPushButton::clicked, ui->et_formula, [this]() { insertFormula("asin(x)", 1); });
    connect(ui->btn_acos, &QPushButton::clicked, ui->et_formula, [this]() { insertFormula("acos(x)", 1); });
    connect(ui->btn_atan, &QPushButton::clicked, ui->et_formula, [this]() { insertFormula("atan(x)", 1); });
    connect(ui->btn_toRad, &QPushButton::clicked, ui->et_formula, [this](){ insertFormula("toRad(x)", 1); });
    connect(ui->btn_log, &QPushButton::clicked, ui->et_formula, [this](){ insertFormula("log(x, y)", 4); });
    connect(ui->btn_ln, &QPushButton::clicked, ui->et_formula, [this](){ insertFormula("ln(x)", 1); });
    connect(ui->btn_pi, &QPushButton::clicked, ui->et_formula, [this]() { ui->et_formula->insert("pi"); });
    connect(ui->btn_e, &QPushButton::clicked, ui->et_formula, [this]() { ui->et_formula->insert("e"); });
    connect(ui->btn_ce, &QPushButton::clicked, ui->et_formula, [this]() { ui->et_formula->setText(""); });

    connect(ui->btn_emptyList, &QPushButton::clicked, ui->tb_resultHistory, [this]() { clearTable(); });
    connect(ui->btn_functionEditor, &QPushButton::clicked, [this]() { onFunctionEditorBtnClicked(); });
    connect(ui->btn_resetEngine, &QPushButton::clicked, ui->et_formula, [this]() { onResetJSEngineBtnClicked(); });

    ui->btn_ce->setShortcut(Qt::Key_Escape);

    connect(ui->actionLoadJSFiles, &QAction::triggered, this, [this](){ evaluateFile(); });
    connect(ui->actionHelpFunction, &QAction::triggered, this, [this](){ FunctionHelper::functionHelperBuilder(c.getPredefinedList())->show(); });
    connect(ui->actionAbout, &QAction::triggered, this, [](){ QDesktopServices::openUrl(QUrl("https://paopaopaoge.xyz/archives/112.html")); });
}

void MainWindow::initTable()
{
    ui->tb_resultHistory->setFocusPolicy(Qt::NoFocus);
    ui->tb_resultHistory->setRowCount(0);
    ui->tb_resultHistory->setColumnCount(2);
    ui->tb_resultHistory->setHorizontalHeaderLabels(QStringList() << "result"
                                                                  << "formula");
    ui->tb_resultHistory->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tb_resultHistory->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tb_resultHistory->horizontalHeader()->setStretchLastSection(true);
    ui->tb_resultHistory->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tb_resultHistory->verticalHeader()->setVisible(false);
    connect(ui->tb_resultHistory, &QTableWidget::cellDoubleClicked, this, &MainWindow::onCellDoubleClicked);
}

void MainWindow::initScriptEditor()
{
    //实例化编辑器类
    te_script = new QsciScintilla(this);

    //设置词法分析器
    QsciLexerJavaScript *scriptLexer = new QsciLexerJavaScript();
    te_script->setLexer(scriptLexer);

    //设置颜色
    scriptLexer->setPaper(QColor(255, 255, 255));
    scriptLexer->setColor(QColor(0, 170, 0), QsciLexerJavaScript::Comment);

    QsciAPIs *apis = new QsciAPIs(scriptLexer);
    QStringList keyWordList = QStringList()
                              << "function"
                              << "break"
                              << "case"
                              << "catch"
                              << "continue"
                              << "default"
                              << "delete"
                              << "do"
                              << "else"
                              << "finally"
                              << "for"
                              << "if"
                              << "in"
                              << "instanceof"
                              << "new"
                              << "return"
                              << "switch"
                              << "this"
                              << "throw"
                              << "try"
                              << "typeof"
                              << "var"
                              << "void"
                              << "while"
                              << "with";
    for (auto i : keyWordList)
        apis->add(i);

    QStringList mathKeyWordList = QStringList()
                                  << "sin"
                                  << "cos"
                                  << "tan"
                                  << "asin"
                                  << "acos"
                                  << "atan"
                                  << "integer"
                                  << "log"
                                  << "ln"
                                  << "pi"
                                  << "e"
                                  << "pow";
    for (auto i : mathKeyWordList)
        apis->add(i);

    apis->prepare();

    te_script->setAutoCompletionSource(QsciScintilla::AcsAll);
    te_script->setAutoCompletionCaseSensitivity(true);
    te_script->setAutoCompletionThreshold(1);
    te_script->SendScintilla(QsciScintilla::SCI_SETCODEPAGE, QsciScintilla::SC_CP_UTF8);

    te_script->setIndentationsUseTabs(false); //false用表示用空格代替\t
    te_script->setTabWidth(4);                //\t宽度设为四个空格
    te_script->setIndentationGuides(true);    //用tab键缩进时，在缩进位置上显示一个竖点线，缩进有效，在字符串后加空格不显示
    te_script->setIndentationWidth(0);        //如果在行首部空格位置tab，缩进的宽度字符数，并且不会转换为空格
    te_script->setAutoIndent(true);           //换行后自动缩进
    te_script->setBackspaceUnindents(true);
    te_script->setTabIndents(true);                               //True如果行前空格数少于tabWidth，补齐空格数,False如果在文字前tab同true，如果在行首tab，则直接增加tabwidth个空格
    te_script->setBraceMatching(QsciScintilla::SloppyBraceMatch); //括号匹配

    te_script->setCaretWidth(2);                            //光标宽度，0表示不显示光标
    te_script->setCaretForegroundColor(QColor("darkCyan")); //光标颜色

    te_script->setFolding(QsciScintilla::BoxedTreeFoldStyle); //折叠样式
    te_script->setFoldMarginColors(Qt::white, Qt::white);     //折叠栏颜色

    te_script->setWhitespaceVisibility(QsciScintilla::WsVisibleOnlyInIndent); //此时空格为点，\t为箭头
    te_script->setWhitespaceSize(2);                                          //空格点大小

    te_script->setMarginType(0, QsciScintilla::NumberMargin);
    te_script->setMarginLineNumbers(0, true);
    te_script->setMarginWidth(0, 15);

    QWidget *console = c.getScriptEngineDebuggerConsoleWidget();

    ui->layout_script->addWidget(te_script);
    ui->layout_script->addWidget(console);

    ui->layout_script->setStretch(0, 4);
    ui->layout_script->setStretch(1, 80);
    ui->layout_script->setStretch(2, 16);
}

void MainWindow::onCellDoubleClicked(int row)
{
    if(tableInfoList[row] == ScriptSource::FORMULA)
        ui->et_formula->setText(ui->tb_resultHistory->item(row, 1)->text());
    else if(tableInfoList[row] == ScriptSource::SCRIPT)
    {
        te_script->setText(ui->tb_resultHistory->item(row, 1)->text());
        if(ui->widget_scriptEditor->isVisible() == false)
            showFunctionEditor();
    }
}

void MainWindow::onResetJSEngineBtnClicked()
{
    c.resetEngine();
    clearTable();
    ui->et_formula->setText("reset Ok");
}

void MainWindow::onFunctionEditorBtnClicked()
{
    if (ui->widget_scriptEditor->isVisible() == false)
        showFunctionEditor();
    else
        hideFunctionEditor();
}

void MainWindow::clearTable()
{
    ui->tb_resultHistory->clearContents();
    ui->tb_resultHistory->setRowCount(0);
    tableInfoList.clear();
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Enter:
    case Qt::Key_Return:
    {
        QObject *focusObj = QApplication::focusWidget();
        if (focusObj != te_script && focusObj)
            evaluateScript(ScriptSource::FORMULA);
        break;
    }
    default:
    {
        QObject *focusObj = QApplication::focusWidget();
        if (focusObj != te_script && focusObj)
        {
            ui->et_formula->setFocus();
            ui->et_formula->insert(event->text());
        }
        break;
    }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
