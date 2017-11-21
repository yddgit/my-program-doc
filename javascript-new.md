# JavaScript

## 基础

* JavaScript是ECMAScript标准的一种实现，ECMAScript 6标准（简称ES6）于2015年6月正式发布。

* `<script></script>`标签默认的type就是JavaScript，所以不必显式地把type指定为JavaScript。
  ```html
  <script>
      //js代码
  </script>
  ```

* 调试JavaScript代码，推荐[Chrome浏览器](https://www.google.cn/chrome/browser/desktop/index.html?system=true&standalone=1)

* JavaScript不强制要求在每个语句的结尾加`;`，浏览器中负责执行JavaScript代码的引擎会自动在每个语句的结尾补上`;`
  > 让JavaScript引擎自动加分号在某些情况下会改变程序的主义，导致运行结果与期望不一致，所以不要省略`;`

* JavaScript严格区分大小写

* JavaScript不区分整数和浮点数，统一用Number表示
  ```javascript
  123; // 整数
  0.456; // 浮点数
  1.2345e3; // 科学计数法
  -99; // 负数
  NaN; // Not a Number，无法计算结果时用NaN表示
  Infinity; // 表示无限大，当数值超过了Number所能表示的最大值时，就表示为Infinity
  2 / 0; // Infinity
  0 / 0; // NaN
  ```

* 比较运算符
  1. `==`会自动转换数据类型再比较
  2. `===`不会自动转换数据类型，如果数据类型不一致返回`false`，如果一致再比较
  3. 不要使用`==`比较，始终坚持使用`===`比较
  4. NaN与所有值都不相等，包括它自己，唯一能判断NaN的方法是`isNaN()`函数
     ```javascript
     NaN === NaN; // false
     isNaN(NaN); // true
     ```

* 对于`null`和`undefined`，大多数情况下都应该用`null`，`undefined`仅仅在判断函数参数是否传递的情况下有用

* 创建数组使用`[]`不要使用`new Array()`

* `strict`模式

  如果一个变量没有通过`var`声明，就自动变为全局变量；使用`var`声明的变量则不是全局变量，它的范围被限制在该变量被声明的**函数**体内。

  ECMA在后续规范中推出了`strict`模式，在`strict`模式下运行的JavaScript代码，强制使用`var`声明变量，未使用`var`声明变量就使用将导致运行错误。

  启用`strict`模式的方法是在JavaScript代码的第一行写上：
  ```javascript
  'use strict';
  ```
  这是一个字符串，不支持`strict`模式的浏览器会把它当做一个字符串语句，支持`strict`模式的浏览器将开启strict模式运行JavaScript
  ```javascript
  // 测试浏览器是否支持strict模式
  'use strict';
  abc = 'Hello World';
  console.log(abc);
  ```

* 字符串。ASCII字符可以用`\x##`形式的十六进制表示，可以用`\u####`表示一个Unicode字符
  ```javascript
  '\x41'; // 'A'
  '\u4e2d\u6587'; // '中文'
  ```

* 多行字符串。ES6标准新增了一种多行字符串的表示方式，用反引号`` `...` ``表示
  ```javascript
  `这是一个
  多行
  字符串`;
  ```

* 模板字符串。ES6标准新增了一种模板字符串，表示方法和多行字符串一样，但会自动替换字符串中的变量
  ```javascript
  var name = '小明';
  var age = 20;
  var message = `你好，${name}, 你今年${age}岁了！`;
  console.log(message);
  ```

* 字符串的常见操作
  ```javascript
  var s = 'Hello';
  s.length; // 5
  s[0]; // 'H'
  s[5]; // undefined，超出范围的索引不会报错，但一律返回undefined
  s[0] = 'X'; // 字符串是不可变的，对某个索引赋值，不会有任何错误，但也没有任何效果
  s.toUpperCase(); // 'HELLO'
  s.toLowerCase(); // 'hello'
  s.indexOf('e'); // 1
  s.substring(0, 3); // 从索引0开始到3（不包括3），返回'hel'
  s.substring(3); // 从索引3开始到结束，返回'lo'
  ```



