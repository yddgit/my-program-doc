# Node.js

Node.js = V8 + JavaScript

`io.js`和`Node.js`的关系：`io.js`先添加新的特性，如果测试没问题，就把新特性加入`Node.js`。`io.js`是“尝鲜版”，而`Node.js`是线上稳定版，相当于Fedora Linux和RHEL的关系

## 安装Node.js和npm

从[Node.js官网](https://nodejs.org/ "Node.js")下载安装包，安装时勾选全部组件，包括`Add to Path`

* 查看版本号：`node -v`
* 查看npm版本号：`npm -v`

## 第一个Node程序

```javascript
// hello.js
'use strict';
console.log('Hello world!');
```

使用如下命令执行`hello.js`（文件名必须以`.js`结尾，只能是英文字母、数字、下划线组合）

```bash
node hello.js
```

为了避免手动给每个文件都写上`'use strict';`，可以给`node`命令传递一个参数，直接为所有js文件开启严格模式

```bash
node --use_strict calc.js
```

## 搭建Node开发环境

使用[Visual Studio Code](https://code.visualstudio.com/ "Visual Studio Code")开发

调试js时需要创建一个`.vscode/launch.json`配置文件：

```json
{
    "version": "0.2.0",
    "configurations": [
        {
            "type": "node",
            "request": "launch",
            "name": "Launch Program",
            "program": "${workspaceFolder}/hello.js",
            "stopOnEntry": false,
            "args": [],
            "cwd": "${workspaceFolder}",
            "preLaunchTask": null,
            "runtimeExecutable": "node",
            "runtimeArgs": [
                "--nolazy",
                "--use_strict"
            ],
            "env": {
                "NODE_ENV": "development"
            },
            "sourceMaps": false
        }
    ]
}
```

## 模块

编写一个`hello.js`文件，这个`hello.js`文件就是一个模块，模块名字就是文件名（去掉`.js`后缀），所以`hello.js`文件就是名为`hello`的模块

```javascript
'use strict';

var s = 'Hello';
function greet(name) {
    console.log(s + ', ' + name + '!');
}
module.exports = greet; // 将函数greet作业模块的输出暴露出去
```

在另外一个js文件中使用模块

```javascript
'use strict';

var greet = require('./hello'); // 引入hello模块
var s = 'Alex';
greet(s); // Hello, Alex!
```

`require()`引入模块时，注意使用相对路径，如果只写模块名`hello`，则Node会在内置模块、全局模块和当前模块下查找`hello.js`

* CommonJS规范

  以上这种模块加载机制称为CommonJS规范，每个js文件都是一个模块，内部各自的变量名和函数名都互不冲突。一个模块想要对外暴露变量，可以用`module.exports = variable;`，一个模块要引用其他模块暴露的变量，用`var ref = require('module_name');`就拿到了引用模块的变量

## 基本模块

* fs

* stream

* http

* crypto

## Web开发

* koa

* mysql

* mocha

* WebSocket

* REST

* MVVM
