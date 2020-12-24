#pragma once

#include <QString>
#include <QPair>
#include <QAction>
#include <QtScript/QScriptEngine>
#include <QScriptEngineDebugger>
#include <QDebug>
#include <QFile>

class Caculator : public QObject{
    QScriptEngine *se;
    QScriptEngineDebugger dbg;

    QList<QPair<QString, QString>> predefinedList;

public:
    enum EvaluateState{RunningState, SuspendedState};
    using Result = QPair<double, EvaluateState>;

    Caculator()
    {
        predefinedList.push_back(QPair<QString, QString>("e", "var e = Math.E;"));
        predefinedList.push_back(QPair<QString, QString>("pi", "var pi = Math.PI;"));

        predefinedList.push_back(QPair<QString, QString>("sin(x)", "function sin(x)\r\n{\r\n    return Math.sin(x);\r\n}"));
        predefinedList.push_back(QPair<QString, QString>("cos(x)", "function cos(x)\r\n{\r\n    return Math.cos(x);\r\n}"));
        predefinedList.push_back(QPair<QString, QString>("tan(x)", "function tan(x)\r\n{\r\n    return Math.tan(x);\r\n}"));
        predefinedList.push_back(QPair<QString, QString>("asin(x)", "function asin(x)\r\n{\r\n    return Math.asin(x);\r\n}"));
        predefinedList.push_back(QPair<QString, QString>("acos(x)", "function acos(x)\r\n{\r\n    return Math.acos(x);\r\n}"));
        predefinedList.push_back(QPair<QString, QString>("atan(x)", "function atan(x)\r\n{\r\n    return Math.atan(x);\r\n}"));
        predefinedList.push_back(QPair<QString, QString>("pow(x, y)", "function pow(x, y)\r\n{\r\n    return Math.pow(x, y);\r\n}"));
        predefinedList.push_back(QPair<QString, QString>("integral(formula, max, min, n)", "function integral(formula, max, min, n)\r\n{\r\n    var delta = (max - min) / n;\r\n    var result = 0;\r\n\r\n    for(var i = min + delta; i < max; i+=delta)\r\n        result += formula(i) * delta;\r\n\r\n    return result;\r\n}"));
        predefinedList.push_back(QPair<QString, QString>("log(x, y)", "function log(x, y)\r\n{\r\n    return Math.log(y) / Math.log(x);\r\n}"));
        predefinedList.push_back(QPair<QString, QString>("ln(x)", "function ln(x)\r\n{\r\n    return Math.log(x);\r\n}"));
        predefinedList.push_back(QPair<QString, QString>("toRad(x))", "function toRad(x)\r\n{\r\n    return x/180 * Math.PI;\r\n}"));

        newScriptEngine();
        connect(&dbg, &QScriptEngineDebugger::evaluationSuspended, [this](){
            dbg.action(QScriptEngineDebugger::DebuggerAction::ContinueAction)->trigger();
        });
    }

    void resetEngine()
    {
        deleteScriptEngine();
        newScriptEngine();
    }

    Result evaluateScript(QString script)
    {
        QScriptValue value = se->evaluate(script);
        return Result(value.toNumber(), fetchState());
    }

    Result evaluateFile(QString fileName)
    {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly))
            return Result(NAN, EvaluateState::RunningState);
        QString contents = QTextStream(&file).readAll();

        QScriptValue value = se->evaluate(contents, file.fileName());
        return Result(value.toNumber(), fetchState());
    }

    QWidget *getScriptEngineDebuggerConsoleWidget()
    {
        return dbg.widget(QScriptEngineDebugger::ConsoleWidget);
    }

    QList<QPair<QString, QString>> getPredefinedList()
    {
        return predefinedList;
    }

    QScriptEngine *getEngine()
    {
        return se;
    }

    ~Caculator()
    {
        deleteScriptEngine();
    }

private:
    void deleteScriptEngine()
    {
        if(se != nullptr)
        {
            dbg.detach();
            delete se;
            se = nullptr;
        }
    }

    void newScriptEngine()
    {
        se = new QScriptEngine();
        dbg.attachTo(se);
        dbg.setAutoShowStandardWindow(false);

        for(auto i : predefinedList)
            se->evaluate(i.second);

        for(auto i : QMap<QString, QString>())
            qDebug();
    }

    EvaluateState fetchState()
    {
        EvaluateState state = EvaluateState::RunningState;
        if(dbg.state() == QScriptEngineDebugger::RunningState)
            state = EvaluateState::RunningState;
        else if(dbg.state() == QScriptEngineDebugger::SuspendedState)
            state = EvaluateState::SuspendedState;
        return state;
    }

protected:
};
