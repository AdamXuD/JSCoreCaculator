#include "functionhelper.h"
#include "ui_functionhelper.h"

FunctionHelper::FunctionHelper(QList<QPair<QString, QString>> list, QWidget *parent) :
    QDialog(parent),
    functionList(list),
    ui(new Ui::FunctionHelper)
{
    ui->setupUi(this);
    setWindowTitle("About Function");
    initScriptViewer();
    ui->horizontalLayout->setStretch(0, 1);
    ui->horizontalLayout->setStretch(1, 4);
    connect(ui->listWidget, &QListWidget::doubleClicked, [this](QModelIndex i){
        te_script->setText(functionList[i.row()].second);
    });
    setAttribute(Qt::WA_DeleteOnClose);
}

FunctionHelper::~FunctionHelper()
{
    delete ui;
}

FunctionHelper *FunctionHelper::functionHelperBuilder(QList<QPair<QString, QString>> functionList)
{
    FunctionHelper *dialog = new FunctionHelper(functionList);
    for(auto i : functionList)
        dialog->ui->listWidget->addItem(i.first);
    return dialog;
}

void FunctionHelper::initScriptViewer()
{
    //实例化编辑器类
    te_script = new QsciScintilla(this);

    //设置词法分析器
    QsciLexerJavaScript *scriptLexer = new QsciLexerJavaScript();
    te_script->setLexer(scriptLexer);

    //设置颜色
    scriptLexer->setPaper(QColor(255, 255, 255));
    scriptLexer->setColor(QColor(0, 170, 0), QsciLexerJavaScript::Comment);

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

    te_script->setReadOnly(true);
    ui->horizontalLayout->addWidget(te_script);
}
