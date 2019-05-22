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

因为Node.js是运行在服务器端的，没有浏览器的安全限制，而且服务器程序必须能够接收网络请求，读写文件，处理二进制内容，所以Node.js内置的常用模块就是为了实现基本的服务器功能。这些模块在浏览器环境中是无法被执行的，因为它们的底层代码是用C/C++在Node.js运行环境中实现的

1. global

   `global`是Node.js中唯一的全局变量（类似浏览器中的window），进入Node.js交互环境，直接输入

   ```javascript
   > global.console
   Console {
     log: [Function: bound consoleCall],
     info: [Function: bound consoleCall],
     warn: [Function: bound consoleCall],
     error: [Function: bound consoleCall],
     dir: [Function: bound consoleCall],
     ...
   }
   ```

2. process

   `process`也是Node.js提供的一个对象，它代表当前Node.js进程

   ```javascript
   > process === global.process
   true
   > process.version
   'v8.9.4'
   > process.platform
   'win32'
   > process.arch
   'x64'
   > process.cwd() // 返回当前工作目录
   'E:\\path\\to\\current'
   > process.chdir('/tmp'); // 切换当前工作目录
   undefined
   > process.cwd()
   'E:\\tmp'
   ```

   JavaScript程序是事件驱动执行的单线程模型，Node.js也是，Node.js不断执行响应事件的JavaScript函数，直到没有任何响应事件的函数可以执行时，Node.js就退出了。如果想要在下一次事件响应中执行代码，可以调用`process.nextTick()`

   ```javascript
   // test.js
   process.nextTick(function() {
       console.log('nextTick callback!');
   });
   console.log('nextTick was set!');
   ```

   用Node执行上面的代码`node test.js`，输出：

   ```
   nextTick was set!
   nextTick callback!
   ```

   这说明传入`process.nextTick()`的函数不是立刻执行，而是要等到下一次事件循环。Node.js进程本身的事件就是由`process`对象来处理。如果我们响应`exit`事件，就可以在程序即将退出时执行某个回调函数

   ```javascript
   // 程序即将退出时的回调函数
   process.on('exit', function(code) {
       console.log('about to exit with code: ' + code);
   });
   ```

3. 判断JavaScript执行环境

   有很多JavaScript代码能在浏览器中执行，也能在Node.js环境执行，程序本身需要判断自己到底是在什么环境下执行的，常用的方式就是根据浏览器和Node环境提供的全局变量名来判断

   ```javascript
   if(typeof(window) === 'undefined') {
       console.log('node.js');
   } else {
       console.log('browser');
   }
   ```

* fs

  Node.js内置的`fs`模块就是文件系统模块，负责读写文件。它同时提供了异步和同步的方法

  ```javascript
  'use strict';

  var fs = require('fs');

  // 异步读取文本文件
  fs.readFile('sample.txt', 'utf-8', function(err, data) {
    // 正常读取时，err参数为null，data为读取到的String
    // 读取错误时，err参数为一个错误对象，data为undefined
    // 这也是Node.js标准的回调函数：第一个参数代表错误信息，第二个参数代表结果
    if(err) {
        console.log(err);
    } else {
        console.log(data);
    }
  });

  // 异步读取二进制文件
  fs.readFile('sample.jpg', function(err, data) {
    if(err) {
        console.log(err);
    } else {
        // 读取二进制文件不传入编码时，回调函数的data参数将返回一个Buffer对象
        // 在Node.js中，Buffer对象就是一个包含零个或任意个字节的数组（但和Array不同）
        console.log(data);
        console.log(data.length + ' bytes');
    }
  });
  ```

  `Buffer`对象可以和String作转换

  ```javascript
  // Buffer --> String
  var text = data.toString('utf-8');
  console.log(text);
  // String --> Buffer
  var buf = Buffer.from(text, 'utf-8');
  console.log(buf);
  ```

  同步读取函数与异步函数相比，多了一个`Sync`后缀，并且不接收回调函数，直接返回结果

  ```javascript
  var data = fs.readFileSync('sample.txt', 'utf-8');
  // 如果发生错误需要用try...catch...捕获
  try {
    fs.readFileSync('no-file.txt', 'utf-8');
  } catch(err) {
    console.log(err);
  }
  ```

  写文件同样分同步和异步两种情况

  ```javascript
  // 异步写文件
  var data = 'Hello, Node.js'
  // 如果data是String，默认按UTF-8编码写入，如果是Buffer，则写入的是二进制文件
  fs.writeFile('output.txt', data, function(err) {
    console.log('=============异步写文本文件=============');
    if(err) {
      console.log(err);
    } else {
      console.log('ok');
    }
  });

  // 同步写文件
  fs.writeFileSync('output.txt', data);
  ```

  `stat`获取文件大小、创建时间等基本信息

  ```javascript
  // 异步
  fs.stat('sample.txt', function(err, stat) {
    console.log('=============异步查看文件信息=============');
    if(err) {
      console.log(err);
    } else {
      console.log('isFile: ' + stat.isFile());
      console.log('isDirectory: ' + stat.isDirectory());
      if(stat.isFile()) {
        console.log('size: ' + stat.size);
        console.log('birth time: ' + stat.birthtime);
        console.log('modified time: ' + stat.mtime);
      }
    }
  });

  // 同步
  var stat = fs.statSync('sample.txt');
  ```

  除了服务器启动时读取配置文件、结束时写入状态文件时可以使用同步代码，其他情况下必须使用异步代码。因为JavaScript只有一个执行线程，同步代码执行期间，服务器将停止响应

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
