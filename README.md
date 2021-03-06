# JS Core ScienceCaculator——基于Qt JSEngine的可编程计算器的发布及使用说明

> 被实验报告繁琐而冗长的数据计算给整烦了，这个东西就诞生了。

虽然说我以前也发布过一款[计算器（命令行的）](https://paopaopaoge.xyz/archives/49.html)，但是这个计算器和以前的那款命令行计算器完全不一样。

以前那款计算器我是为了练一手栈容器的使用，了解一下计算器计算的原理而整出来的。

这款最初我是以把以前那款命令行计算器图形化为目的在写的，但是在写的途中，发现`Qt`的库里有一个`JavaScript`脚本引擎，可以做简单计算，还可以支持定义运行函数什么的，这不比我自己整的笨比计算器好用多了，之后我就开始围绕着`QScript`引擎开始整这款计算器了。

[Github仓库地址](https://github.com/AdamXuD/JSCoreCaculator)

废话不多说，我们先来看看主界面。

![image-20201224124238699.png](https://paopaopaoge.xyz/usr/uploads/2020/12/3180910111.png#vwid=411&vhei=708)

可以很容易看出，这款计算器是由**一个输入框控件**，**运算函数按钮组**，和**一个运算历史记录表控件**组成的。

## 基本用法

最基本的使用方法很简单，**在输入框里输入数学表达式，按下右边的Enter或者键盘上的回车，即可在输入框上获得结果，同时也能在下面的运算历史中得到看见结果。**

![image-20201224124624362.png](https://paopaopaoge.xyz/usr/uploads/2020/12/3913515808.png#vwid=829&vhei=712)

但是要是表达式输入不合法，你将会得到一个`nan`结果，同时结果也会在运算历史中显示出来。

![image-20201224124934786.png](https://paopaopaoge.xyz/usr/uploads/2020/12/86043598.png#vwid=829&vhei=712)

那么我们很容易可以看出来，运算成功的`1+1`和运算失败的`2+2+`的运算结果在历史记录中的颜色是不一样的。**运算成功的值显示是绿色（表示表达式没有错误），不成功的是红色（表达式有错误）**。

考虑到当**因为表达式有错误导致结果运算错误**，**你想修改表达式并重新计算**时，你可以双击运算历史中想要修改的一行，然后双击的那行的算式则会显示在输入框中，你可以在输入框中重新修改算式并重新计算。

![image-20201224125659837.png](https://paopaopaoge.xyz/usr/uploads/2020/12/4039382454.png#vwid=829&vhei=712)

作为一个计算器，它支持如下的运算符（也包括其他的`JavaScript`支持的运算符）。

| 运算符 | 描述 |
| :----- | :--- |
| +      | 加法 |
| -      | 减法 |
| *      | 乘法 |
| /      | 除法 |
| %      | 取余 |

**关于乘方，请用`pow(x, y)`函数替代。`x`为底数， `y`为指数。**

关于下面的按钮，点击后会在输入框最后插入一个**函数**，在函数内填入数值按回车即可算得值，以`sin(3.14 / 2)`为例。

![image-20201224130441984.png](https://paopaopaoge.xyz/usr/uploads/2020/12/1623363009.png#vwid=829&vhei=712)

显然函数内部可以支持填写新的表达式，函数本身也可以被当作操作数的一部分，比如像这样：

![image-20201224130735456.png](https://paopaopaoge.xyz/usr/uploads/2020/12/3604530932.png#vwid=638&vhei=708)

得到的结果是`-1`，与我们的预期一致。

**我还包装了其他的`JavaScript`的`Math`对象下其他的部分函数，可以在`菜单栏 > About > About define function`查看。**

![image-20201224131459270.png](https://paopaopaoge.xyz/usr/uploads/2020/12/170494229.png#vwid=638&vhei=708)

关于这一部分我们介绍到这，下面是该计算器的核心功能。

## 进阶用法

点开`Function Editor`我们会看见窗口右边扩展了一部分区域。

![image-20201224131831467.png](https://paopaopaoge.xyz/usr/uploads/2020/12/1607010610.png#vwid=1210&vhei=708)

从上往下是**函数编辑器**， **Console**，**确认键**。

**函数编辑器**是我们可以用来写函数的地方（废话），支持一定的**代码补全**，**代码高亮**，**代码缩进**等功能（不支持语法检查）。

**Console**是我们用来检查报错的地方，运算或代码执行错误都会在`Console`里看见报错。

**Enter键**按下后引擎会将代码编辑器中的代码执行。

但是**函数编辑器只支持`JavaScript`写函数**，不过关于`JavaScript`的基本语法（指计算器用得到的**函数定义**和算术运算这种）跟我们常见的`C/C++`，`Java`大同小异就是了。



今天刚好做了单摆测定重力加速度的实验，我们以这个公式为例：

![image-20201224185416696.png](https://paopaopaoge.xyz/usr/uploads/2020/12/488760986.png#vwid=214&vhei=96)

其中周期`T`需要用三条原始数据求得，单摆长度`l`可以直接测得。

我们可以用如下函数，输入三条原始数据和单摆长度即可快速求出重力加速度的测量值：

```javascript
function g(d1, d2, d3, l)
{
    var average = (d1 + d2 + d3) / 3; //求d1 d2 d3的平均数
    var T = average / 20 * pow(10, -6); //将平均数除20得到周期，并将周期（微妙） * 10^6得到周期（秒）
    return 4 * pow(pi, 2) * l / pow(T, 2); //计算4*(pi^2)*l/(T^2)
}
```

之后在输入框里输入函数与数据并调用，则可以得到所需要的值：

![image-20201224185801743.png](https://paopaopaoge.xyz/usr/uploads/2020/12/1871465961.png#vwid=1290&vhei=708)

不过如果输入值不符合语法会怎么样呢？

![image-20201224190230198.png](https://paopaopaoge.xyz/usr/uploads/2020/12/4072577691.png#vwid=1290&vhei=708)

在`Console`界面会报错误内容和行号，在运算历史记录里会有红色的`nan`结果出现。

## 附录

### 关于预先定义的函数的说明

| 函数或常量                     | 说明                                                         |
| ------------------------------ | ------------------------------------------------------------ |
| e                              | 同数学常量：自然对数e                                        |
| pi                             | 同数学常量：圆周率pi                                         |
| sin(x)                         | 同数学表达：正弦函数sin(x)                                   |
| cos(x)                         | 同数学表达：余弦函数sin(x)                                   |
| tan(x)                         | 同数学表达：正切函数sin(x)                                   |
| asin(x)                        | 同数学表达：反正弦函数sin(x)                                 |
| acos(x)                        | 同数学表达：反余弦函数sin(x)                                 |
| atan(x)                        | 同数学表达：反正切函数sin(x)                                 |
| pow(x, y)                      | 求幂：x为底数，y为指数                                       |
| integral(formula, max, min, n) | 定积分：formula为想要运算的公式的回调函数，max为上限，min为下限，n为精度（请取1000以上，n越大，精度越高，也越吃资源） |
| log(x, y)                      | 同数学表达：对数函数log，x为底数，y为真数                    |
| ln(x)                          | 同数学表达：对数函数ln(x)                                    |
| toRad(x)                       | 角度制转弧度值，弧度制=toRad(角度制)                         |

### 关于菜单栏 > File > Load .js File

它可以将.`js`文件中的脚本（函数或变量等）加载到引擎中。

如果你有一套自己的函数每次打开计算器都会使用到的话，对每次都在函数编辑器里输入并运行都感到过于繁琐，可以将这套函数或者变量保存在`.js`文件中，每次使用时可以通过`菜单栏 > File > Load .js File`加载并运行它，会非常方便。



# 后记

其实这套东西真没什么技术含量，还没有我之前写的命令行计算器有意思。其实是有点想自己写一个解释器来进行算式解析和函数解析的，但是很多东西大概没那么容易做得跟`QScript`引擎那么好，而且快期末了，搞不好某一天我心血来潮会整的吧。剩下的随缘维护吧。

希望这东西有用。

在`菜单栏 > About > About`还能看到这篇文章~
