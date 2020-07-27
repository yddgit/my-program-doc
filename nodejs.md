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

  流也是一个对象，我们只需要响应流的事件就可以了：`data`事件表示流的数据已经可以读取了，`end`事件表示这个流已经到末尾了，没有数据可以读取了，`error`事件表示出错了

  ```javascript
  'use strict'
  
  var fs = require('fs');
  
  // 创建read流
  var rs = fs.createReadStream('sample.txt', 'utf-8');
  
  // 响应流事件
  rs.on('data', function(chunk) {
      console.log('DATA');
      console.log(chunk);
  });
  rs.on('end', function() {
      console.log('END');
  });
  rs.on('error', function(err) {
      console.log('ERROR' + err)
  });
  ```

  以流的形式写入文件，只需要不断调用`write()`方法，最后以`end()`结束

  ```javascript
  // 创建write流
  var ws1 = fs.createWriteStream('output1.txt', 'utf-8');
  ws1.write('使用Stream写入文本数据...\n');
  ws1.write('END');
  ws1.end();
  var ws2 = fs.createWriteStream('output2.txt');
  ws2.write(new Buffer('使用Stream写入二进制数据...\n', 'utf-8'));
  ws2.write(new Buffer('END', 'utf-8'));
  ws2.end();
  ```

  所有读取数据的流都继承自`stream.Readable`，所有可以写入的流都继承自`stream.Writable`。两个流可以串起来，数据自动从Readable流进入Writable流，这个操作可以用Readable中的`pipe()`方法实现

  ```javascript
  // 如：复制一个文件
  var rs = fs.createReadStream('sample.txt');
  var ws = fs.createWriteStream('copied.txt');
  rs.pipe(ws);
  ```

  默认情况下，当Readable流的数据读取完毕，`end`事件触发后，将自动关闭Writable流。如果不希望自动关闭Writable流，需要传入参数：`rs.pipe(ws, {end: false});`

* http

  Node.js自带`http`模块实现了HTTP协议，提供了`request`和`response`对象：

  * `request`对象封装了HTTP请求，我们调用`request`对象的属性和方法就可以拿到所有HTTP请求的信息
  * `response`对象封装了HTTP响应，我们操作`response`对象的方法，就可以把HTTP响应返回给浏览器

  Node.js实现一个HTTP服务器：

  ```javascript
  'use strict';

  var http = require('http');
  // 创建http server，并传入回调函数
  var server = http.createServer(function (request, response) {
    // 获得HTTP请求的method和url
    console.log(request.method + ': ' + request.url);
    // 将HTTP响应200写和response，同时设置Content-Type: text/html
    response.writeHead(200, {'Content-Type': 'text/html'});
    // 将HTTP响应的HTML内容写入response
    response.end('<h1>Hello World!</h1>');
  });

  // 让服务器监听8080端口
  server.listen(8080);
  console.log('Server is running at http://localhost:8080/');
  ```

* crypto

  crypto模块的目的是为了提供通用的加密和哈希算法，用C/C++实现这些算法，通过crypto模块暴露为JavaScript接口，这样用起来方便，运行速度也快。常用的有：

  * MD5/SHA1

    ```javascript
    'use strict';
    const crypto = require('crypto');

    const hash = crypto.createHash('md5'); // md5/sha1/sha256/sha512
    // 可任意多次调用update()，默认字符编码为UTF-8，也可以传入Buffer
    hash.update('Hello world!');
    hash.update('Hello nodejs!');
    console.log(hash.digest('hex')); // e7ccbe87d78713fb925afbb15a1f9485
    ```

  * Hmac

    ```javascript
    const hmac = crypto.createHmac('sha256', 'secret-key');
    hmac.update('Hello world!');
    hmac.update('Hello nodejs!');
    console.log(hmac.digest('hex')); // 1b71dc8ab352f594ca1c6050976d4ab90677d377dee855d704c7b87cac871923
    ```

  * AES

    ```javascript
    function aesEcrypt(data, key) {
        const cipher = crypto.createCipher('aes192', key);
        var crypted = cipher.update(data, 'utf8', 'hex');
        crypted += cipher.final('hex');
        return crypted;
    }

    function aesDecrypt(encrypted, key) {
        const decipher = crypto.createDecipher('aes192', key);
        var decrypted = decipher.update(encrypted, 'hex', 'utf8');
        decrypted += decipher.final('utf8');
        return decrypted;
    }

    var data = 'Hello, this is a secret message';
    var key = 'Password!';
    var encrypted = aesEcrypt(data, key);
    var decrypted = aesDecrypt(encrypted, key);
    console.log('Plain text: ' + data);
    console.log('Encrypted text: ' + encrypted);
    console.log('Decrypted text: ' + decrypted);
    ```

  * Diffie-Hellman

    ```javascript
    var ming = crypto.createDiffieHellman(512);
    var ming_keys = ming.generateKeys();

    var prime = ming.getPrime();
    var generator = ming.getGenerator();

    console.log('Prime: ' + prime.toString('hex'));
    console.log('Generator: ' + generator.toString('hex'));

    var hong = crypto.createDiffieHellman(prime, generator);
    var hong_keys = hong.generateKeys();

    var ming_secret = ming.computeSecret(hong_keys);
    var hong_secret = hong.computeSecret(ming_keys);

    console.log('Secret of xiao ming: ' + ming_secret.toString('hex'));
    console.log('Secret of xiao hong: ' + hong_secret.toString('hex'));
    ```

  * RSA

    ```javascript
    const fs = require('fs');

    // 从文件加载key
    function loadKey(file) {
        // key实际上就是PEM编码的字符串
        return fs.readFileSync(file, 'utf8');
    }

    let
        prvKey = loadKey('rsa-prv.pem'),
        pubKey = loadKey('rsa-pub.pem'),
        message = 'Hello, world';

    // 使用私钥加密
    let enc_by_prv = crypto.privateEncrypt(prvKey, Buffer.from(message, 'utf8'));
    console.log('encrypted by private key: ' + enc_by_prv.toString('hex'));
    let dec_by_pub = crypto.publicDecrypt(pubKey, enc_by_prv);
    console.log('decrypted by public key: ' + dec_by_pub.toString('utf8'));

    // 使用公钥加密
    let enc_by_pub = crypto.publicEncrypt(pubKey, Buffer.from(message, 'utf8'));
    console.log('encrypted by public key: ' + enc_by_pub.toString('hex'));
    let dec_by_prv = crypto.privateDecrypt(prvKey, enc_by_pub);
    console.log('decrypted by private key: ' + dec_by_prv.toString('utf8'));
    ```

    如果把message字符串长度增加到很长，如1M，RSA加密会报错：`data too large for key size`，这是因为RSA加密的原始信息必须小于Key的长度。
    
    RSA并不适合加密大数据，而是先成一个随机的AES密码，用AES密码加密原始信息，然后用RSA加密AES口令。这样实际使用RSA时，给对方传的密文分两部分，一部分是AES加密的密文，另一部分是RSA加密的AES口令。对方用RSA先解密出AES口令，再用AES解密密文，即可获得明文

  * 证书

    crypto模块也可以处理数字证书，数字证书通常用在SSL连接，即Web的https连接。一般情况下，https连接只需要处理服务器端的单向认证，如无特殊需求（例如自己作为Root给客户发认证证书），建议用反向代理服务器如Nginx等Web服务器去处理证书

## Web开发

* koa

  koa是Express的下一代基于Node.js的web框架。Express是第一代最流行的web框架，它对Node.js的http进行了封装，用起来如下：

  ```javascript
  var express = require('express');
  var app = express();

  app.get('/', function(req, res) {
    res.send('Hello World');
  });

  app.listen(3000, function() {
    console.log('Example app listening on port 3000!');
  });
  ```

  虽然Express的API很简单，但是它是基于ES5的语法，要实现异步，只有一个方法：回调。如果异步嵌套层次很多，代码写起来就非常难看：

  ```javascript
  app.get('/test', function(req, res) {
    fs.readFile('/file1', function(err, data) {
      if(err) {
        res.status(500).send('read file1 error!');
      }
      fs.readFile('/file2', function(err, data) {
        if(err) {
          res.status(500).send('read file2 error!');
        }
        res.type('text/plain');
        res.send(data);
      });
    });
  });
  ```

  随着ES6的标准得到Node.js的支持，Express团队又基于ES6的generator重新编写了下一代web框架koa 1.0

  ```javascript
  var koa = require('koa');
  var app = koa();

  app.use('/test', function *() {
    yield doReadFile();
    var data = yield doReadFile2();
    this.body = data;
  });

  app.listen(3000);
  ```

  但是generator的本意并不是异步，Promise才是为异步设计的，但是Promise的写法太复杂。koa团队又基于ES7开发koa2，完全使用Promise并配合`async`来实现异步。

  ```javascript
  app.use(async(ctx, next) => {
    await next();
    var data = await doReadFile();
    ctx.response.type = 'text/plain';
    ctx.response.body = data;
  });
  ```

  示例：https://github.com/yddgit/hello-koa.git

* mysql

  https://github.com/yddgit/hello-sequelize.git

* mocha

  mocha是javascript的一种单元测试框架，既可以在浏览器环境下运行，也可以在Node.js环境下运行

  * 既可以测试简单的函数，又可以测试异步代码
  * 可以自动运行所有测试，也可以只运行特定的测试
  * 可以支持before、after、beforeEach和afterEach来编写初始化代码

  https://github.com/yddgit/hello-mocha.git

* WebSocket

* REST

* MVVM
