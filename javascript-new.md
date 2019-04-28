# JavaScript

## 基础

* JavaScript是ECMAScript标准的一种实现，ECMAScript 6标准（简称**ES6**）于2015年6月正式发布。

* `<script></script>`标签默认的type就是JavaScript，所以不必显式地把type指定为JavaScript。
  ```html
  <script>
      //js代码
  </script>
  ```

* 调试JavaScript代码，推荐[Chrome浏览器](https://www.google.cn/chrome/browser/desktop/index.html?system=true&standalone=1)

* JavaScript不强制要求在每个语句的结尾加`;`，浏览器中负责执行JavaScript代码的引擎会自动在每个语句的结尾补上`;`

  让JavaScript引擎自动加分号在某些情况下会改变程序的语义，导致运行结果与期望不一致，所以不要省略`;`
  ```javascript
  function foo() {
    return
      {name: 'foo'};
  }
  foo(); // undefined

  // 由于JavaScript引擎在行末自动添加分号的机制，以上代码实际上变成了：
  function foo() {
    return; // 自动添加了分号，相当于return undefined;
      {name: 'foo'}; // 这句代码永远不会执行到
  }

  // 正确的多行写法是：
  function foo() {
    return { // 这里不会自动加分号，因为{表示语句尚未结束
      name: 'foo'
    }
  }
  ```

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
  5. JavaScript把`null`、`undefined`、`0`、`NaN`和空字符串`''`视为`false`，其他值一概视为`true`

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

* 多行字符串。**ES6**标准新增了一种多行字符串的表示方式，用反引号`` `...` ``表示

  ```javascript
  `这是一个
  多行
  字符串`;
  ```

* 模板字符串。**ES6**标准新增了一种模板字符串，表示方法和多行字符串一样，但会自动替换字符串中的变量

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

* Array的常见操作

  ```javascript
  var arr = [1, 2, 3]; // 直接给Array的length赋值会导致Array大小的变化
  arr.length; // 3
  arr.length = 6; // arr变为[1, 2, 3, undefined, undefined, undefined]
  arr.length = 2; // arr变为[1, 2]

  var arr = [1, 2, 3]; // 通过索引赋值时，索引超出了范围也会引起Array大小的变化
  arr[5] = 'x'; // arr变为[1, 2, 3, undefined, undefined, 'x']
  arr.indexOf(1); // 0
  arr.indexOf(3); // 2
  arr.indexOf('3'); // 没有找到元素时返回-1

  var arr = ['A', 'B', 'C', 'D', 'E', 'F', 'G']; // slice()对应String的substring()方法
  arr.slice(0, 3); // 从索引0开始到3结束，但不包括索引3：['A', 'B', 'C']
  arr.slice(3); // 从索引3开始到结束：['D', 'E', 'F', 'G']
  var aCopy = arr.slice(); // slice()不传参数会从头到尾截取所有元素，可利用这一点复制一个Array
  aCopy; // ['A', 'B', 'C', 'D', 'E', 'F', 'G']
  aCopy === arr; // false

  var arr = [1, 2];
  arr.push('A', 'B'); // 向Array末尾添加元素[1, 2, 'A', 'B']，返回Array新的长度：4
  arr.pop(); //把最后一个元素删除掉[1, 2, 'A']，返回删除的元素：'B'
  arr.pop();arr.pop();arr.pop(); // 所有元素都pop出来：[]
  arr.pop(); //空数组继续pop不会报错，而是返回undefined

  var arr = [1, 2];
  arr.unshift('A', 'B'); //向Array头部添加元素['A', 'B', 1, 2]，返回Array新的长度：4
  arr.shift(); //把第一个元素删掉['B', 1, 2]，返回删除的元素：'B'
  arr.shift();arr.shift();arr.shift(); //所有元素都shift出来：[]
  arr.shift(); //空数组继续shift不会报错，而是返回undefined

  var arr = ['B', 'C', 'A'];
  arr.sort(); // 排序：['A', 'B', 'C']

  var arr = ['one', 'two', 'three'];
  arr.reverse(); // 反转：['three', 'two', 'one']

  //splice()方法是修改Array的万能方法，可以从指定索引开始删除若干个元素，然后再从该位置添加若干个元素
  var arr = ['Microsoft', 'Apple', 'Yahoo', 'AOL', 'Excite', 'Oracle'];
  arr.splice(2, 3, 'Google', 'Facebook'); // 从索引2开始删除3个元素，然后再添加两个元素['Microsoft', 'Apple', 'Google', 'Facebook', 'Oracle']，返回删除的元素['Yahoo', 'AOL', 'Excite']
  arr.splice(2, 2); // 只删除不添加['Microsoft', 'Apple', 'Oracle']，返回删除的元素['Google', 'Facebook']
  arr.splice(2, 0, 'Google', 'Facebook'); // 只添加不删除['Microsoft', 'Apple', 'Google', 'Facebook', 'Oracle']，返回[]，因为没有删除任何元素

  var arr = ['A', 'B', 'C'];
  var added = arr.concat([1, 2, 3]); // 把当前Array和另一个Array连接起来，返回一个新的Array：['A', 'B', 'C', 1, 2, 3]，原arr仍为['A', 'B', 'C']
  arr.concat(1, 2, [3, 4]); // concat()方法可以接收任意个元素和Array，并自动把Array拆开，然后全部添加到新的Array里：['A', 'B', 'C', 1, 2, 3, 4]

  var arr = ['A', 'B', 'C', 1, 2, 3];
  arr.join('-'); // join()把Array元素用指定字符串连接起来：'A-B-C-1-2-3'
  ```

* 对象

  ```javascript
  var xiaohong = {
    name: '小红',
    'middle-school': 'No.1 Middle School'
  };
  xiaohong['middle-school']; // 'No.1 Middle School'，如果属性名称不是有效变量名，需要用''括起来
  xiaohong.age; // undefined，访问不存在的属性返回undefined
  'name' in xiaohong; // true，检测对象是否拥有某一属性用in操作符
  'grade' in xiaohong; // false
  'toString' in xiaohong; // true，操作符in判断的属性可以是对象继承得到的
  xiaohong.hasOwnProperty('name'); // true，判断属性是对象本身拥有而不是继承得到的，可以用hasOwnProperty()方法
  xiaohong.hasOwnProperty('toString'); // false
  ```

* 循环语句

  ```javascript
  // for ... in
  var o = {
    name: 'Jack',
    age: 20,
    city: 'Beijing'
  };
  for(var key in o) {
    if(o.hasOwnProperty(key)) {
      console.log(key + '=' + o[key]);
    }
  }
  ```

* `Map`和`Set`（**ES6**规范）

  JavaScript对象的key必须是字符串，但实际上Number或其他数据类型也是合理的，为此**ES6**引入新的数据类型`Map`。`Map`是一组键值对，具有极快的查找速度。

  ```javascript
  var m = new Map([['Michael', 95], ['Bob', 75], ['Tracy', 85]]); // 使用二维数组初始Map
  m.get('Michael'); // 95
  var m = new Map();
  m.set('Adam', 67); // 使用set方法添加key-value
  m.set('Bob', 59);
  m.has('Adam'); // 是否存在key='Adam'：true
  m.get('Adam'); // 67
  m.delete('Adam'); // 删除key='Adam'的键值对
  m.get('Adam'); // undefined
  m.set('Jerry', 67);
  m.set('Jerry', 88);
  m.get('Jerry'); // 88，多次设置一个key的value，后设的值会覆盖前面的值
  ```

  `Set`是一组key的集合，由于key不能重复，所以`Set`中没有重复key

  ```javascript
  var s1 = new Set(); // 空Set
  s1.add(1);
  s1.add(4); // {1, 4}
  s1.add(4); // {1, 4}，add(key)可以添加元素，重复添加没有效果
  var s2 = new Set([1, 2, 3]); // {1，2，3}
  var s = new Set([1, 2, 3, 3, '3']); // {1, 2, 3, '3'}，重复元素在Set中自动被过滤
  s.delete(3); // {1, 2, '3'}，delete(key)可以删除元素
  ```

* `iterable`（**ES6**规范）

  为了统一集合类型，**ES6**标准引入了新的`iterable`类型，`Array`、`Map`和`Set`都属于`iterable`类型。具有`iterable`类型的集合可以通过新的`for ... of`循环来遍历。
  ```javascript
  'use strict';

  var a = ['A', 'B', 'C'];
  var s = new Set(['A', 'B', 'C']);
  var m = new Map([[1, 'x'], [2, 'y'], [3, 'z']]);
  for(var x of a) { // 遍历Array
    console.log(x);
  }
  for(var x of s) { // 遍历Set
    console.log(x);
  }
  for(var x of m) { // 遍历Map
    console.log(x[0] + '=' + x[1]);
  }
  ```

  为什么要引入`for ... of`循环：
  1. `for ... in`循环由于历史遗留问题，遍历的实际上是对象的属性名称，对于Array每个元素的索引被视为一个属性。

     当手动给Array对象添加额外的属性后，`for ... in`循环将带来意想不到的效果：
     ```javascript
     var a = ['A', 'B', 'C'];
     a.name = 'Hello';
     for(var x in a) {
       console.log(x); // '0', '1', '2', 'name'
     }
     // for ... in循环把name包括在内，但Array的length属性却不包括在内
     ```
  2. `for ... of`循环修复了以上问题，只循环集合本身的元素：

     ```javascript
     var a = ['A', 'B', 'C'];
     a.name = 'Hello';
     for(var x of a) {
       console.log(x); // 'A', 'B', 'C'
     }
     ```

  更好的方式是直接使用`iterable`内置的`forEach()`方法（**ES5.1**标准引入）：

  ```javascript
  'use strict';

  var a = ['A', 'B', 'C'];
  a.forEach(function (element, index, array) {
    // element: 指向当前元素的值
    // index: 指向当前索引
    // array: 指向array对象本身
    console.log(element + ', index=' + index);
  });

  var s = new Set(['A', 'B', 'C']);
  s.forEach(function (element, sameElement, set){
    // Set没有索引，因此前两个参数都是元素本身
    console.log(element);
  });

  var m = new Map([[1, 'x'], [2, 'y'], [3, 'z']]);
  m.forEach(function(value, key, map){
    console.log(value);
  });
  ```

## 函数

* 函数参数

  未传入的参数，函数内将收到`undefined`，为此可以对参数进行检查：
  ```javascript
  function abx(x) {
    if(typeof x !== 'number') {
      throw 'Not a number';
    }
    if(x >= 0) {
      return x;
    } else {
      return -x;
    }
  }
  ```

  JavaScript关键字arguments，只在函数内部起作用，并且永远指向当前函数的调用者传入的所有参数，类似Array但不是Array：
  ```javascript
  'use strict';

  function foo(x) {
    console.log('x = ' + x); // 10
    for(var i=0; i<arguments.length; i++) {
      console.log('arg' + i + ' = ' + arguments[i]); // 10, 20, 30
    }
  }
  foo(10, 20, 30);
  
  // arguments最常用于判断传入参数的个数，如下：
  // foo(a[, b], c)
  // 接收2~3个参数，b是可选参数，如果只传2个，b默认为null
  function foo(a, b, c) {
    if(arguments.length === 2) {
      // 实际拿到的参数是a和b，c为undefined
      c = b; // 把b赋给c
      b = null; // b变为默认值
    }
    // ...
  }
  ```

  **ES6**标准引入了`rest`参数，用于获取除了已定义参数之外的参数：
  ```javascript
  // rest参数只能写在最后，前面用...标识
  function foo(a, b, ...rest) {
    console.log('a = ' + a);
    console.log('b = ' + b);
    console.log(rest);
  }
  foo(1, 2, 3, 4, 5); // a = 1, b = 2, Array[3, 4, 5]
  // 如果传入的参数少于正常定义的参数，rest参数会接收一个空数组（不是undefined）
  foo(1); // a = 1, b = undefined, Array[]
  ```

* 变量作用域

  用`var`声明的变量是有作用域的，函数体内的变量作用域为整个函数体。

  内部函数可以访问外部函数定义的变量，反过来则不行。如果内部函数定义了与外部函数重名的变量，则内部函数的变量将“屏蔽”外部函数的变量。

  **不在任何函数内定义的变量就具有全局作用域**，全局作用域的变量实际上被绑定为`window`对象的一个属性：
  ```javascript
  'use strict';

  var course = 'Learn JavaScript';
  alert(course); // 'Learn JavaScript'
  alert(window.course); // 'Learn JavaScript'
  ```

  顶层函数的定义也被视为一个全局变量，并绑定到`window`对象：
  ```javascript
  'use strict';

  function foo() {
    alert('foo');
  }
  foo(); // 直接调用foo()
  window.foo(); // 通过window.foo()调用
  // 因此alert()函数也是window的一个变量
  window.alert('alert');
  ```

  这说明JavaScript实际上只有一个全局作用域，任何变量如果没有在当前函数作用域中找到，就会继续往上查找，最后如果在全局作用域中也没有找到，则报`ReferenceError`错误

* 名字空间。为了减少命名冲突，可以把自己的所有变量和函数全部绑定到一个全局变量中。
  ```javascript
  // 唯一的全局变量MYAPP
  var MYAPP = {};
  // 其他变量
  MYAPP.name = 'myapp';
  MYAPP.version = 1.0;
  // 其他函数
  MYAPP.foo = function() {
    return 'foo';
  };
  ```

* 局部作用域

  由于JavaScript的变量作用域实际是函数内部，在`for`循环等语句块中是无法定义局部作用域的变量的：
  ```javascript
  'use strict';

  function foo() {
    for(var i=0; i<100; i++) {
      // ...
    }
    i += 100; // 仍然可以引用变量
  }
  ```

  为此**ES6**引入了新的关键字`let`，用`let`替代`var`可以声明一个块级作用域的变量
  ```javascript
  'use strict';

  function foo() {
    var sum = 0;
    for(let i=0; i<100; i++) {
      sum += i;
    }
    i += 1; // SyntaxError
  }
  ```

* 常量。**ES6**标准引入新的关键字`const`来定义常量，`const`与`let`都具有块级作用域

  ```javascript
  'use strict';

  const PI = 3.14;
  PI = 3; // 某些浏览器不报错，但无效果
  console.log(PI); // 3.14
  ```

* 变量提升

  JavaScript函数定义会扫描整个函数体的语句，把所有申明的变量“提升”到函数顶部，称为**变量提升**。
  ```javascript
  'use strict';

  function foo() {
    var x = 'Hello, ' + y;
    console.log(x);
    var y = 'Bob';
  }
  foo(); // Hello, undefined
  ```
  虽然是strict模式，但语句`var x = 'Hello, ' + y;`并不报错，原因是变量y在后面声明了。但`console.log`显示`Hello, undefined`，说明`y`的值为`undefined`。

  这正是因为JavaScript引擎自动提升了变量y的声明，但不会提升y的赋值。JavaScript引擎看到的代码相当于：
  ```javascript
  function foo() {
    var y; // 提升变量y的声明，此时y为undefined
    var x = 'Hello, ' + y;
    console.log(x);
    y = 'Bob';
  }
  ```

  因此，在函数内部定义变量时，请严格遵守“**在函数内部首先声明所有变量**”这一规则。最常见的做法是用一个var声明函数内部用到的所有变量：
  ```javascript
  function foo() {
    var 
      x = 1, // x初始化为1
      y = x + 1, // y初始化为2
      z, i; // z和i为undefined
    // ...
    for(i=0; i<100; i++) {
      ...
    }
  }
  ```

* 解构赋值（**ES6**规范引入）

  把一个数组的元素分别赋值给几个变量：
  ```javascript
  // 传统做法
  var array = ['hello', 'JavaScript', 'ES6'];
  var x = array[0];
  var y = array[1];
  var z = array[2];

  // 使用解构赋值
  'use strict';

  // 多个变量用[...]括起来
  var [x, y, z] = ['hello', 'JavaScript', 'ES6']; // x='hello', y='JavaScript', z='ES6'
  // 如果数组本身有嵌套，也可以进行解构赋值，嵌套层次和位置和保持一致
  var [x, [y, z]] = ['hello', ['JavaScript', 'ES6']];
  // 还可以忽略某些元素
  var [, , z] = ['hello', 'JavaScript', 'ES6'];
  ```

  使用解构赋值快速获取对象指定属性：
  ```javascript
  var person = {
    name: '小明',
    age: 20,
    gender: 'male',
    passport: 'G-12345678',
    school: 'No.4 middle school',
    address: {
      city: 'Beijing',
      street: 'No.1 Road',
      zipcode: '100001'
    }
  };
  // name, age, passport分别被赋值为对应属性
  var {name, age, passport} = person;
  // 对嵌套的对象属性进行赋值
  // 1. 如果对应的属性不存在，变量将被赋值为undefined，因此zip=undefined
  // 2. address不是变量，而是为了让city和zip获得嵌套的address对象的属性
  var {name, address: {city, zip}} = person;
  // 如果变量名与属性名不一致
  var {name, passport:id} = person; // 这里passport不是变量，而是为了让变量id获得passport属性的值
  // 解构赋值时使用默认值，避免不存在的属性返回undefined的问题
  var {name, single=true} = person; // 如果person对象没有single属性，默认赋值为true
  ```

  如果变量已经被声明，再次赋值时会报语法错误
  ```javascript
  var x, y;
  // 解构赋值，报SyntaxError
  {x, y} = {name: '小明', x: 100, y: 200};
  // 解决方法是用小括号括起来
  ({x, y} = {name: '小明', x: 100, y: 200});
  ```

  解构赋值的使用场景
  ```javascript
  // 交换两个变量x和y的值
  var x=1, y=2;
  [x, y] = [y, x];
  // 快速获取当前页面的域名和路径
  var {hostname:domain, pathname:path} = location;
  // 如果一个函数接收一个对象作为参数，那可以使用解构赋值把属性绑定到变量中
  // 如下：快速创建一个Date对象
  function buildDate({year, month, day, hour=0, minute=0, second=0}) {
    return new Date(year + '-' + month + '-' + day + ' ' + hour + ':' + minute + ':' + second);
  }
  buildDate({year:2017, month:1, day:1});
  buildDate({year:2017, month:1, day:1, hour:20, minute:15});
  ```

* this

  在一个方法内部，`this`是一个特殊变量，它始终指向调用该方法的当前对象。单独调用函数时`this`指向全局对象，也就是`window`。要保证`this`指向正确必须用`obj.xxx()`的形式调用。
  ```javascript
  function getAge() {
    var y = new Date().getFullYear();
    return y - this.birth;
  }

  var xiaoming = {
    name: '小明',
    birth: 1990,
    age: getAge
  };

  xiaoming.age(); // 27，正常结果
  getAge(); // NaN
  ```

  在`strict`模式下单独调用函数时，`this`指向`undefined`。但这只是让错误及时暴露出来。

  如下，`this`只在age方法的函数内指向对象`xiaoming`，在函数内部定义的函数，`this`又指向了`undefined`（在非`strict`模式下，它重新指向全局对象`window`）
  ```javascript
  'use strict';

  var xiaoming = {
    name: '小明',
    birth: 1990,
    age: function() {
      function getAgeFromBirth() {
        var y = new Date().getFullYear();
        return y - this.birth;
      }
      return getAgeFromBirth();
    }
  };
  xiaoming.age(); // Uncaught TypeError: Cannot read property 'birth' of undefined
  ```

  解决办法是先用一个变量捕获`this`：
  ```javascript
  'use strict';

  var xiaoming = {
    name: '小明',
    birth: 1990,
    age: function() {
      var that = this; // 在方法内部一开始就捕获this
      function getAgeFromBirth() {
        var y = new Date().getFullYear();
        return y - that.birth; // 用that而不是this
      }
      return getAgeFromBirth();
    }
  };
  xiaoming.age(); // 27
  ```

* `apply()`

  `apply`方法可以指定`this`指向哪个对象，`apply`方法接收两个参数，第一个是需要绑定`this`的变量，第二个参数是`Array`，表示函数本身的参数
  ```javascript
  function getAge() {
    var y = new Date().getFullYear();
    return y - this.birth;
  }

  var xiaoming = {
    name: '小明',
    birth: 1990,
    age: getAge
  };

  xiaoming.age(); // 27
  getAge.apply(xiaoming, []); // 27，this指向xiaoming，参数为空
  ```

  `call()`方法与`apply()`方法类似，唯一区别是：`apply()`把参数打包成`Array`再传入，`call()`把参数按顺序传入
  ```javascript
  // 对普通函数调用，通常把this绑定为null
  Math.max.apply(null, [3, 5, 4]); // 5
  Math.max.call(null, 3, 5, 4); // 5
  ```

  利用`apply()`可以动态改变函数行为。如要统计代码中`parseInt()`的调用次数：
  ```javascript
  'use strict';

  var count = 0;
  var oldParseInt = parseInt; //保存原函数

  window.parseInt = function() {
    count += 1;
    return oldParseInt.apply(null, arguments); //调用原函数
  };

  parseInt('10');
  parseInt('20');
  parseInt('30');
  console.log('count = ' + count); // 3
  ```

* map/reduce

  ```javascript
  'use strict';

  function pow(x) {
    return x * x;
  }

  var arr = [1, 2, 3, 4, 5, 6, 7, 8, 9];
  // 把函数f(x)=x^2作用在数组的每个元素上
  arr.map(pow); // [1, 4, 9, 16, 25, 36, 49, 64, 81]
  // 把数组的所有元素转为字符串
  arr.map(String); // ['1', '2', '3', '4', '5', '6', '7', '8', '9']
  ```

  Array的reduce()把一个函数作用到这个Array上，这个函数必须接收两个参数，reduce把结果继续和序列的下一个元素做累积计算

  ```javascript
  // 求和
  var arr = [1, 3, 5, 7, 9];
  arr.reduce(function(x, y) {
    return x + y;
  }); // 25

  // string转int
  function string2int(s) {
    return s.split("").map(function(val) {
      return '0123456789'.indexOf(val);
    }).reduce(function(x, y) {
      return x * 10 + y;
    });
  }
  ```

* filter

  filter用于把Array的某些元素过滤掉，然后返回剩下的元素。Array的filter接收一个函数做为参数，然后把传入的函数依次作用于每个元素，然后根据返回值是`true`还是`false`决定保留还是丢弃该元素

  ```javascript
  // 删除偶数，保留奇数
  var arr = [1,2,3,4,5,6,7,8,9,10];
  var r = arr.filter(function(x) {
    return x % 2 !== 0;
  });

  // 数组去重
  var
    r,
    arr = ['apple', 'strawberry', 'banana', 'pear', 'apple', 'orange', 'orange', 'strawberry'];
  r = arr.filter(function(element, index, self) {
    // indexOf总是返回第一个元素的位置
    return self.indexOf(element) === index;
  });

  // 筛选质数
  function get_primes(arr) {
    return arr.filter(function(val){
      if(val === 1) {
        return false;
      }
      var i, half = val/2;
      for(i=2; i<=half; i++) {
        if(val % i === 0) break;
      }
      return i > half;
    });
  }
  get_primes([1,2,3,4,5,6,7,8,9,10]);
  ```

* sort

  Array的sort()方法是用于排序的，默认把所有元素先转换为String再排序

  ```javascript
  [10, 20, 1, 2].sort(); // [1, 10, 2, 20]
  ```

  sort()方法可以接收一个比较函数来实现自定义排序。**注意：sort()方法会直接对Array进行修改，返回的结果仍是当前Array**

  ```javascript
  // 数组排序
  var arr = [10, 20, 1, 2];
  arr.sort(function(x, y) {
    if(x < y) {
      return -1;
    }
    if(x > y) {
      return 1;
    }
    return 0;
  });

  // 忽略大小写的排序
  var arr = ['Google', 'apple', 'Microsoft'];
  arr.sort(function(s1, s2) {
    x1 = s1.toUpperCase();
    x2 = s2.toUpperCase();
    if(x1 < x2) {
      return -1;
    }
    if(x1 > x2) {
      return 1;
    }
    return 0;
  });
  ```

* 闭包

  高阶函数除了可以接受函数作为参数外，还可以把函数作为结果返回。

  ```javascript
  // 返回求和函数
  function lazy_sum(arr) {
    var sum = function() {
      return arr.reduce(function(x, y) {
        return x + y;
      });
    }
    return sum;
  }
  // 调用lazy_sum()时，返回的并不是求和结果，而是求和函数
  var f = lazy_sum([1, 2, 3, 4, 5]); // function sum()
  f(); // 15
  // 调用lazy_sum()时，每次调用都会返回一个新的函数
  var f1 = lazy_sum([1, 2, 3, 4, 5]);
  var f2 = lazy_sum([1, 2, 3, 4, 5]);
  f1 === f2; // false
  ```

  注意到返回的函数在其内部引用了局部变量arr，所以，当一个函数返回了一个函数后，其内部的局部变量还被新函数引用，因此，闭包用起来简单，实现起来不容易。另外，返回的函数并没有立刻执行，而是直到调用了f()才执行。

  ```javascript
  // 如下，每次循环都创建一个函数，然后将3个函数添加到一个数组中返回
  function count() {
    var arr = [];
    for(var i=1; i<=3; i++) {
      arr.push(function() {
        return i * i;
      });
    }
    return arr;
  }
  var results = count();
  var f1 = results[0];
  var f2 = results[1];
  var f3 = results[2];
  // f1()、f2()、f3()结果应该是1、4、9，但实际是：
  f1(); // 16
  f2(); // 16
  f3(); // 16
  // 原因就在于返回的函数引用了变量i，但并非立刻执行，等到3个函数都返回时，它们所引用的变量i已经变成了4，因此最终结果为16
  ```

  返回闭包时牢记一点：**返回函数不要引用任何循环变量，或者后续会发生变化的变量**

  如果一定要引用循环变量，就再创建一个函数，用该函数的参数绑定循环变量当前的值，无论该循环变量后续如何更改，已绑定到函数参数的值不变

  ```javascript
  function count() {
    var arr = [];
    for(var i=1; i<=3; i++) {
      arr.push(
        // 创建一个匿名函数并立刻执行
        (function(n){
          return function() {
            return n * n;
          }
        })(i)
      );
    }
    return arr;
  }
  var results = count();
  var f1 = results[0];
  var f2 = results[1];
  var f3 = results[2];
  f1(); // 1
  f2(); // 4
  f3(); // 9
  ```

  闭包的作用：在Java或C++里，可以用private修饰一个成员变量，实现在对象内部封装一个私有变量。对于没有class机制，只有函数的语言里，借助闭包同样可以封装一个私有变量。

  ```javascript
  'use strict';
  // 创建一个计数器
  // 返回的对象中，实现了一个闭包，该闭包携带了局部变量x，并且，从外部代码根本无法访问到变量x。闭包就是携带状态的函数，并且它的状态可以完全对外隐藏起来
  function create_counter(initial) {
    var x = initial || 0;
    return {
      inc: function() {
        x += 1;
        return x;
      }
    }
  }
  // 使用
  var c1 = create_counter();
  c1.inc(); // 1
  c1.inc(); // 2
  c1.inc(); // 3
  var c2 = create_counter(10);
  c2.inc(); // 11
  c2.inc(); // 12
  c2.inc(); // 13
  ```

  闭包还可以把多参数的函数变成单参数的函数，如：计算x^y可以用Math.pow(x, y)，不过考虑到经常计算x^2或x^3，可以利用闭包创建新的函数pow2和pow3

  ```javascript
  'use strict';

  function make_pow(n) {
    return function(x) {
      return Math.pow(x, n);
    }
  }
  // 创建两个新函数
  var pow2 = make_pow(2);
  var pow3 = make_pow(3);
  console.log(pow2(7)); // 49
  console.log(pow3(5)); // 125
  ```

* 箭头函数（**ES6**规范）

  ES6标准新增了一种新的函数：Arrow Function（箭头函数）

  ```javascript
  // 箭头函数的定义用的就是一个箭头
  x => x * x
  // 上面的箭头函数相当于
  function (x) {
    return x * x;
  }
  // 箭头函数相当于匿名函数，并且简化了函数定义。还有一种包含多条语句
  x => {
    if(x > 0) {
      return x * x;
    } else {
      return - x * x;
    }
  }

  // 如果参数不是一个，就需要用括号()括起来
  
  // 两个参数
  (x, y) => x * x + y * y;
  // 无参数
  () => 3.14
  // 可变参数
  (x, y, ...rest) => {
    var i, sum = x + y;
    for(i=0; i<rest.length; i++) {
      sum += rest[i];
    }
    return sum;
  }
  // 如果要返回一个对象，就要注意，如果是单表达式，如下写法会报错
  x => { foo: x } // SyntaxError
  // 因为函数体的{ ... }有语法冲突，所以要改为
  x => ({ foo: x }) // ok
  ```

  箭头函数中的`this`

  箭头函数看上去是匿名函数的一种简写，但实际上，两者有明显区别：箭头函数内部的`this`是词法作用域，由上下文确定

  ```javascript
  // 回顾前面的例子，由于JavaScript函数对this绑定的错误处理，下面的代码无法得到预期结果
  var obj = {
    birth: 1990,
    getAge: function() {
      var b = this.birth; // 1990
      var fn = function() {
        return new Date().getFullYear() - this.birth; // this指向window或undefined
      };
      return fn();
    }
  };

  // 箭头函数完全修复了this的指向，this总是指向词法作用域，也就是外层调用者obj
  var obj = {
    birth: 1990,
    getAge: function() {
      var b = this.birth; // 1990
      var fn = () => new Date().getFullYear() - this.birth; // this指向obj对象
      return fn();
    }
  };
  obj.getAge();

  // 使用箭头函数，以前的这种hack写法就不再需要了
  var that = this;

  // 由于this在箭头函数中已经按照词法作用域绑定了，所以用call()或apply()调用箭头函数时，无法对this进行绑定，即传入的第一个参数被忽略
  var obj = {
    birth: 1990,
    getAge: function(year) {
      var b = this.birth; // 1990
      var fn = (y) => y - this.birth; // this.birth仍是1990
      return fn.call({birth:2000}, year);
    }
  };
  obj.getAge(2015); // 25
  ```

* generator（**ES6**规范）

  ES6的generator标准借鉴了Python的概念和语法，与函数很像，定义如下

  ```javascript
  function* foo(x) {
    yield x + 1;
    yield x + 2;
    return x + 3;
  }
  ```

  generator与函数不同的是，generator由`function*`定义，并且除了`return`语句，还可以用`yield`返回多次

  以斐波那契数列为例，它由`0`，`1`开头：`0 1 1 2 3 5 8 13 21 34 ...`，要编写一个产生斐波那契数列的函数，可以这么写：

  ```javascript
  function fib(max) {
    var
      t,
      a = 0,
      b = 1,
      arr = [0, 1];
    while(arr.length < max) {
      [a, b] = [b, a+b];
      arr.push(b);
    }
    return arr;
  }

  fib(10); // [0, 1, 1, 2, 3, 5, 8, 13, 21, 34]
  ```

  函数只能返回一次，所以必须返回一个`Array`，但是generator可以一次返回一个数，不断返回多次：

  ```javascript
  function* fib(max) {
    var
      t,
      a = 0,
      b = 1,
      n = 0;
    while(n < max) {
      yield a;
      [a, b] = [b, a+b];
      n++;
    }
    return;
  }

  fib(5); // fib {[[GeneratorStatus]]: "suspended", [[GeneratorReceiver]]: Window}
  ```

  直接调用一个generator和调用函数不一样，`fib(5)`仅仅是创建了一个generator对象，还没有去执行它
  
  调用generator对象有两个方法：
  
  1. 不断地调用next()方法

     ```javascript
     var f = fib(5);
     f.next(); // {value: 0, done: false}
     f.next(); // {value: 1, done: false}
     f.next(); // {value: 1, done: false}
     f.next(); // {value: 2, done: false}
     f.next(); // {value: 3, done: false}
     f.next(); // {value: undefined, done: true} 
     ```
  
     `next()`方法会执行generator代码，当遇到`yield x;`，就返回一个对象`{value: x, done: true/false}`，然后“暂停”，返回的`value`就是`yield`的返回值，`done`表示这个generator是否已经执行结束了。

     如果`done`为true，则`value`就是`return`的返回值。这时generator对象就已经全部执行完毕了，不要再继续调用next()了。

  2. 用`for ... of`循环迭代generator对象，这种方法不需要自己判断done

     ```javascript
     for( var x of fib(10)) {
       console.log(x); // 0, 1, 1, 2, 3, ...
     }
     ```

  generator可以在执行过程中多次返回，看上去像一个可以记住执行状态的函数，利用这一点就可以实现需要用面向对象才能实现的功能。如，用一个对象来保存状态，就得这么写：

  ```javascript
  var fib = {
    a: 0,
    b: 1,
    n: 0,
    max: 5,
    next: function() {
      var
        r = this.a,
        t = this.a + this.b;
      this.a = this.b;
      this.b = t;
      if(this.n < this.max) {
        this.n++;
        return r;
      } else {
        return undefined;
      }
    }
  }
  ```

  使用generator实现一个计数器

  ```javascript
  'use strict';

  function* next_id() {
    var x = 1;
    while(true) yield x++;
  }

  var g = next_id();
  g.next(); // 1
  g.next(); // 2
  ```

## 标准对象

  1. 不要使用`new Number()`、`new Boolean`、`new String`创建包装对象
     ```javascript
     // number, boolean, string都有包装对象
     var n = new Number(123);
     var b = new Boolean(true);
     var s = new String('str');
     // 包装对象的类型是object，与原始值用===比较会返回false
     typeof new Number(123); // 'object'
     new Number(123) === 123; // false
     typeof new Boolean(true); // 'object'
     new Boolean(true) === true; // false
     typeof new String('str'); // 'object'
     new String('str') === 'str'; // false
     // 如果不写new，Number()、Boolean()和String()被当做普通函数，把任何类型的数据转换为number、boolean和string类型（注意不是其包装类型）：
     typeof Number('123'); // 'number', 相当于parseInt()或parseFloat()
     typeof Boolean('true'); // 'boolean' true
     var b1 = Boolean('false'); // true
     var b2 = Boolean(''); // false
     typeof String(123.45); // 'string'
     ```
  2. 用`parseInt`或`parseFloat`来转换任意类型到`number`
  3. 用`String()`来转换任意类型到`string`，或者直接调用某个对象的`toString()`方法
     * 不是任何对象都有`toString()`方法，`null`和`undefined`就没有
     * `number`对象调用`toString()`会报语法错误
       ```javascript
       123.toString(); // SyntaxError
       // 此时需要特殊处理一下
       123..toString(); // '123', 注意是两个点
       (123).toString(); // '123'
       ```
  4. 通常不必把任意类型转换为boolean再判断，因为可以直接写`if (myVar) {...}`
  5. `typeof`操作符可以判断出`number`、`boolean`、`string`、`function`和`undefined`
     ```javascript
     typeof 123; // 'number'
     typeof NaN; // 'number'
     typeof 'str'; // 'string'
     typeof true; // 'boolean'
     typeof undefined; // 'undefined'
     typeof Math.abs; // 'function'
     typeof null; // 'object'
     typeof []; // 'object'
     typeof {}; // 'object'
     ```
  6. 判断`Array`要使用`Array.isArray(arr)`
  7. 判断`null`请使用`myVar === null`
  8. 判断某个全局变量是否存在用`typeof window.myVar === 'undefined'`
  9. 函数内部判断某个变量是否存在用`typeof myVar === 'undefined'`

* Date

  ```javascript
  // 获取系统当前时间(本机操作系统时间)
  var now = new Date();
  now; // Thu Jan 10 2019 16:34:07 GMT+0800 (China Standard Time)
  now.getFullYear(); // 2019, 年份
  now.getMonth(); // 0, 月份0~11, 0表示1月
  now.getDate(); // 10, 表示10号
  now.getDay(); // 4, 表示星期四
  now.getHours(); // 16, 24小时制
  now.getMinutes(); // 34, 分钟
  now.getSeconds(); // 7, 秒
  now.getMilliseconds(); // 522, 毫秒数
  now.getTime(); // 1547109247522, 以number形式表示的时间戳

  // 创建一个指定日期和时间的Date对象
  var d = new Date(2015, 5, 19, 20, 15, 30, 123);
  d; // Fri Jun 19 2015 20:15:30 GMT+0800 (China Standard Time)
  
  // 通过解析一个符合ISO 8601格式的字符串来创建Date对象
  var d = Date.parse('2015-06-24T19:49:22.875+08:00');
  d; // 1435146562875
  // 不过它返回的是一个时间戳，可以很容易地转换为一个Date
  var d = new Date(1435146562875);
  d; // Wed Jun 24 2015 19:49:22 GMT+0800 (China Standard Time)
  d.getMonth(); // 5, 使用Date.parse()时传入的字符串使用实际月份01~12, 转换为Date对象后getMonth()为0~11

  // 使用时间戳创建Date()就不需要关心时区转换的问题
  var d = new Date(1435146562875);
  d.toLocaleString(); // '6/24/2015, 7:49:22 PM'
  d.toUTCString(); // 'Wed, 24 Jun 2015 11:49:22 GMT'

  // 获取当前时间戳
  if(Date.now) {
    console.log(Date.now()); // 老版本IE没有now()方法
  } else {
    console.log(new Date().getTime());
  }
  ```

* RegExp

  JavaScript有两种方式创建一个正则表达式：

  1. 直接通过`/正则表达式/`写出来
  2. 通过`new RegExp('正则表达式')`创建RegExp对象

  ```javascript
  var re1 = /ABC\-001/;
  var re2 = new RegExp('ABC\\-001'); // 注意字符串转义

  // 测试给定的字符串是否匹配：test()
  var re = /^\d{3}\-\d{3,8}$/;
  re.test('010-12345'); // true
  re.test('010-1234x'); // false
  re.test('010 12345'); // false

  // 切分字符串
  'a b   c'.split(' '); // ['a', 'b', '', '', 'c']
  'a b   c'.split(/\s+/); // ['a', 'b', 'c']
  'a,b, c  d'.split(/[\s\,]+/); // ['a', 'b', 'c', 'd']
  'a,b;; c  d'.split(/[\s\,\;]+/); // ['a', 'b', 'c', 'd']
  ```

  提取子串，用`()`表示要提取的分组（Group）

  ```javascript
  var re = /^(\d{3})-(\d{3,8})$/;
  re.exec('010-12345'); // ['010-12345', '010', '12345']
  re.exec('010 12345'); // null
  ```

  如果正则表达式定义了组，可以在RegExp对象上用exec()方法提取出子串来
  * exec()方法匹配成功后，会返回一个Array，第一个元素是正则表达式匹配到的整个字符串，后面的字符串表示匹配成功的子串
  * exec()方法匹配失败时返回null

  ```javascript
  var re = /^(0[0-9]|1[0-9]|2[0-3]|[0-9])\:(0[0-9]|1[0-9]|2[0-9]|3[0-9]|4[0-9]|5[0-9]|[0-9])\:(0[0-9]|1[0-9]|2[0-9]|3[0-9]|4[0-9]|5[0-9]|[0-9])$/;
  re.exec('19:05:30'); // ['19:05:30', '19', '05', '30']
  ```

  正则匹配默认是贪婪匹配，加个`?`就可以采用非贪婪匹配

  ```javascript
  // 贪婪匹配
  var re = /^(\d+)(0*)$/;
  re.exec('102300'); // ['102300', '102300', '']
  // 非贪婪匹配
  var re = /^(\d+?)(0*)$/;
  re.exec('102300'); // ['102300', '1023', '00']
  ```

  JavaScript的正则表达式还有几个特殊的标志，最常用的是`g`，表示全局匹配

  ```javascript
  var r1 = /test/g;
  // 等价于
  var r2 = new RegExp('test', 'g');
  ```

  全局匹配可以多次执行`exec()`方法来搜索一个字符串，当指定了`g`标志后，每次执行`exec()`，正则表达式本身会更新`lastIndex`属性，表示上次匹配到的最后索引

  ```javascript
  var s = 'JavaScript, VBScript, JScript and ECMAScript';
  var re = /[a-zA-Z]+Script/g;

  re.exec(s); // ['JavaScript']
  re.lastIndex; // 10
  re.exec(s); // ['VBScript']
  re.lastIndex; // 20
  re.exec(s); // ['JScript']
  re.lastIndex; // 29
  re.exec(s); // ['ECMAScript']
  re.lastIndex; // 44
  re.exec(s); // null
  ```

  全局匹配类似搜索，因此不能使用`/^...$/`，那样只会最多匹配一次。正则表达式还可以指定`i`标志，表示忽略大小写，`m`标志，表示执行多行匹配

* JSON

  JSON - JavaScript Object Notation

  JSON中的数据类型：

  - number：即JavaScript中的`number`
  - boolean：即JavaScript中的`true`或`false`
  - string：即JavaScript中的`string`
  - null：即JavaScript中的`null`
  - array：JavaScript中`Array`的表示方式`[]`
  - object：即JavaScript中的`{...}`表示方式

  JSON字符集必须是`UTF-8`，字符串必须用双引号`""`，Object的key也必须用双引号`""`

  **序列化**

  ```javascript
  'use strict';
  var xiaoming = {
    name: '小明',
    age: 14,
    gender: true,
    height: 1.65,
    grade: null,
    'middle-school': '\"W3C\" Middle School',
    skills: ['JavaScript', 'Java', 'Python', 'Lisp']
  };
  var s = JSON.stringify(xiaoming);
  console.log(s);
  // {"name":"小明","age":14,"gender":true,"height":1.65,"grade":null,"middle-school":"\"W3C\" Middle School","skills":["JavaScript","Java","Python","Lisp"]}

  // 缩进输出
  s = JSON.stringify(xiaoming, null, '  ');
  console.log(s);
  /*
  {
    "name": "小明",
    "age": 14,
    "gender": true,
    "height": 1.65,
    "grade": null,
    "middle-school": "\"W3C\" Middle School",
    "skills": [
      "JavaScript",
      "Java",
      "Python",
      "Lisp"
    ]
  }
  */

  // 第二个参数用于控制如何筛选对象的key，如：只输出指定的属性
  s = JSON.stringify(xiaoming, ['name', 'skills'], '  ');
  console.log(s);
  /*
  {
    "name": "小明",
    "skills": [
      "JavaScript",
      "Java",
      "Python",
      "Lisp"
    ]
  }
  */
  // 还可以传入一个函数，这样对象的每个key-value都会被函数先处理(递归的)
  // 如：把所有属性值都变成大写
  function convert(key, value) {
    if(typeof value === 'string') {
      return value.toUpperCase();
    }
    return value;
  }
  s = JSON.stringify(xiaoming, convert, '  ');
  console.log(s);
  /*
  {
    "name": "小明",
    "age": 14,
    "gender": true,
    "height": 1.65,
    "grade": null,
    "middle-school": "\"W3C\" MIDDLE SCHOOL",
    "skills": [
      "JAVASCRIPT",
      "JAVA",
      "PYTHON",
      "LISP"
    ]
  }
  */

  // 可以定义toJSON()方法精确控制序列化
  xiaoming.toJSON = function() {
    return {
      'Name': this.name,
      'Age': this.age
    }
  }
  s = JSON.stringify(xiaoming);
  console.log(s);
  // {"Name":"小明","Age":14}
  ```

  **反序列化**

  使用`JSON.parse()`将JSON格式的字符串变成一个JavaScript对象

  ```javascript
  JSON.parse('[1,2,3,true]'); // [1, 2, 3, true]
  JSON.parse('{"name":"小明","age":14}'); // {name: "小明", age: 14}
  JSON.parse('true'); // true
  JSON.parse('123.45'); // 123.45

  // JSON.parse()还可以接收一个函数用来转换解析出的属性
  var obj = JSON.parse('{"name":"小明","age":14}', function(key, value) {
    if(key === 'name') {
      return value + '同学';
    }
    return value;
  });
  console.log(JSON.stringify(obj));
  // {"name":"小明同学","age":14}
  ```

## 面向对象编程

与`Java`、`C#`等面向对象语言中的类和实例的概念不同，JavaScript通过原型(prototype)来实现面向对象编程。原型是指当我们要创建`xiaoming`这个具体的学生时，并没有一个`Student`类型可用。但有一个现成的对象：

```javascript
var robot = {
  name: 'Robot',
  height: 1.6,
  run: function() {
    console.log(this.name + ' is running...');
  }
};
```

这个`robot`对象有名字、身高，还会跑，有点像小明，就用它来“创建”小明吧

```javascript
var Student = {
  name: 'Robot',
  height: 1.2,
  run: function() {
    console.log(this.name + ' is running...');
  }
}

var xiaoming = {
  name: '小明'
};

// 把xiaoming的原型指向对象Student
xiaoming.__proto__ = Student;
xiaoming.name; // '小明'
xiaoming.run(); // 小明 is running...
```

`xiaoming`有自己的`name`属性，但并没有定义`run()`方法，但由于是从`Student`继承而来，只要`Student`有`run()`方法，`xiaoming`也可以调用

JavaScript的原型链和Java的Class区别就在，它没有“Class”的概念，所有对象都是实例，所谓继承关系不过是把一个对象的原型指向另一个对象而已

如果把`xiaoming`的原型指向其他对象：

```javascript
var Bird = {
  fly: function() {
    console.log(this.name + ' is flying...');
  }
};

xiaoming.__proto__ = Bird;

// 现在xiaoming无法run()了
xiaoming.fly(); // 小明 is flying...
```

在JavaScript代码运行时期，可以把xiaoming从Student变成Bird，或者变成任何对象

**注意**：不要直接用`obj.__proto__`去改变一个对象的原型，而且低版本的IE也无法使用`__proto__`。`Object.create()`方法可以传入一个原型对象，并创建一个基于该原型的新对象，但新对象什么属性都没有

```javascript
var Student = {
  name: 'Robot',
  height: 1.2,
  run: function() {
    console.log(this.name + ' is running...');
  }
};

function createStudent(name) {
  var s = Object.create(Student);
  s.name = name;
  return s;
}

var xiaoming = createStudent('小明');
xiaoming.run(); // 小明 is running...
xiaoming.__proto__ === Student; // true
```

* 创建对象

  当用`obj.xxx`访问一个对象属性时，JavaScript引擎先在当前对象上查找该属性，如果没有找到，就到其原型对象上找，如果还没有找到，就一直上溯到`Object.prototype`对象，最后，如果还没有找到，就只能返回`undefined`

  ```javascript
  // 创建一个Array对象
  var arr = [1, 2, 3];
  // 原型链：arr ----> Array.prototype ----> Object.prototype ----> null
  // Array.prototype定义了indexOf()、shift()等方法，因此可以在所有的Array对象上直接调用这些方法

  // 创建一个函数
  function foo() {
    return 0;
  }
  // 原型链：foo ----> Function.prototype ----> Object.prototype ----> null
  // Function.prototype定义了apply()方法，因此所有函数都可以调用apply()方法
  ```

  因此，如果原型链很长，那么访问一个对象的属性就会因为花更多时间查找而变得更慢，因此不要把原型链搞得太长

* 构造函数

  除了直接用`{...}`创建一个对象外，JavaScript还可以用一种构造函数的方法来创建对象。它的用法是，先定义一个构造函数：

  ```javascript
  // 看起来就是一个普通函数
  function Student(name) {
    this.name = name;
    this.hello = function() {
      alert('Hello, ' + this.name + '!');
    }
  }
  // 但在JavaScript中，可以用关键字new来调用这个函数，并返回一个对象
  var xiaoming = new Student('小明');
  xiaoming.name; // 小明
  xiaoming.hello(); // Hello, 小明
  ```

  注意：如果不写`new`，它就是一个普通函数，它返回`undefined`。但是，如果写了`new`，它就变成了一个构造函数，它绑定的`this`指向新创建的对象，并默认返回`this`，也就是说，不需要在最后写`return this;`

  新创建的`xiaoming`的原型链：`xiaoming ----> Student.prototype ----> Object.prototype ----> null`

  如果又创建了`xiaohong`、`xiaojun`，那么其原型与xiaoming是一样的

  用`new Student()`创建的对象还从原型上获得了一个`constructor`属性，它指向函数Student本身：

  ```javascript
  xiaoming.constructor === Student.prototype.constructor; // true
  Student.prototype.constructor === Student; // true
  Object.getPrototypeOf(xiaoming) === Student.prototype; // true
  xiaoming instanceof Student; // true
  ```

  用一张图来表示上面的关系就是：

  ![javascript-prototype-constructor-1](https://github.com/yddgit/my-program-doc/raw/master/images/javascript-prototype-constructor-1.png "javascript-prototype-constructor-1")

  红色箭头是原型链。注意，`Student.prototype`指向的对象就是`xiaoming`、`xiaohong`的原型对象，这个原型对象自己还有个属性`constructor`，指向`Student`函数本身。但是`xiaoming`、`xiaohong`这些对象可没有`prototype`这个属性，不过可以用`__proto__`这个非标准用法来查看。现在我们就认为`xiaoming`、`xiaohong`这些对象“继承”自`Student`

  不过有个问题：

  ```javascript
  xiaoming.name; // 小明
  xiaohong.name; // 小红
  xiaoming.hello; // function: Student.hello()
  xioahong.hello; // function: Student.hello()
  xiaoming.hello === xiaohong.hello; // false
  ```

  `xiaoming`和`xiaohong`各自的name不同，这是对的，它们各自的`hello`是一个函数，但它们是两个不同的函数，虽然函数名和代码都相同。如果通过`new Student`创建了很多对象，这些对象的`hello`函数实际上只需要共享同一个函数就可以了，这样可以节省很多内存

  要让创建的对象共享一个`hello`函数，根据对象的属性查找原则，我们只要把`hello`函数移到`xiaoming`、`xiaohong`这些对象共同的原型上就可以了，也就是`Student.prototype`

  ![javascript-prototype-constructor-2](https://github.com/yddgit/my-program-doc/raw/master/images/javascript-prototype-constructor-2.png "javascript-prototype-constructor-2")

  ```javascript
  function Student(name) {
    this.name = name;
  }

  Student.prototype.hello = function() {
    alert('Hello, ' + this.name + '!');
  }
  ```

* 忘记写`new`怎么办

  如果一个函数被定义为用地创建对象的构造函数，但是调用时忘记了写`new`会出现什么问题？

  在strict模式下，`this.name = name`将报错，因为`this`绑定为`undefined`，在非strict模式下，`this.name = name`不报错，因为`this`绑定为`window`，于是无意间创建了全局变量`name`，并且返回`undefined`，这个结果更糟糕。所以调用构造函数千万不要忘记写`new`。为了区分普通函数和构造函数，按照约定，构造函数首字母应当大写，而普通函数首字母应当小写

  一个常用的编程模式是：

  ```javascript
  function Student(props) {
    this.name = props.name || '匿名';
    this.grade = props.grade || 1;
  }

  Student.prototype.hello = function() {
    alert('Hello, ' + this.name + '!');
  };

  //这个函数的优点：1.不需要new来调用，2.参数非常灵活
  function createStudent(props) {
    return new Student(props || {});
  }

  // 如果创建的对象有很多属性，我们只需要传递需要的某些属性，剩下的属性可以用默认值
  // 由于参数是Object，无需记忆参数的顺序，如果恰好从JSON拿到了一个对象，就可以直接创建出xiaoming
  var xiaoming = createStudent({
    name: '小明'
  });
  xiaoming.grade; // 1
  ```

* 原型继承

  `Student`构造函数

  ```javascript
  function Student(props) {
    this.name = props.name || 'Unnamed';
  }

  Student.prototype.hello = function () {
    alert('Hello, ' + this.name + '!');
  }
  ```

  `Student`原型链

  ![javascript-prototype-constructor-2](https://github.com/yddgit/my-program-doc/raw/master/images/javascript-prototype-constructor-2.png "javascript-prototype-constructor-2")

  现在，要基于`Student`扩展出`PrimaryStudent`，可以先定义出`PrimaryStudent`：

  ```javascript
  function PrimaryStudent(props) {
    Student.call(this, props); // 调用Student构造函数，绑定this变量
    this.grade = props.grade || 1;
  }
  ```

  但是，调用`Student`构造函数不等于继承了`Student`，`PrimaryStudent`创建的对象的原型是：

  `new PrimaryStudent ----> PrimaryStudent.prototype ----> Object.prototype ----> null`

  必须想办法把原型链修改为：

  `new PrimaryStudent ----> PrimaryStudent.prototype ----> Student.prototype ----> Object.prototype ----> null`

  如下的做法是不行的：

  ```javascript
  PrimaryStudent.prototype = Student.prototype;
  ```

  此时，`PrimaryStudent`和`Student`共享一个原型对象，定义`PrimaryStudent`就没意义了

  必须借助一个中间对象来实现正确的原型链，这个中间对象的原型要指向`Student.prototype`

  ```javascript
  // PrimaryStudent构造函数
  function PrimaryStudent(props) {
    Student.call(this, props);
    this.grade = props.grade || 1;
  }
  // 空函数
  function F() {}
  // 把F的原型指向Student.prototype
  F.prototype = Student.prototype
  // 把PrimaryStudent的原型指向一个新的F对象，F对象的原型正好指向Student.prototype
  PrimaryStudent.prototype = new F();
  // 把PrimaryStudent原型的构造函数恢复为PrimaryStudent
  PrimaryStudent.prototype.constructor = PrimaryStudent;
  // 继续在PrimaryStudent原型(即new F()对象)上定义方法
  PrimaryStudent.prototype.getGrade = function() {
    return this.grade;
  }

  var xiaoming = new PrimaryStudent({
    name: '小明',
    grade: 2
  });
  xiaoming.name; // 小明
  xiaoming.grade; // 2

  xiaoming.__proto__ === PrimaryStudent.prototype; // true
  xiaoming.__proto__.__proto__ === Student.prototype; // true
  xiaoming instanceof PrimaryStudent; // true
  xiaoming instanceof Student; // true
  ```

  新的原型链：

  ![javascript-inherits](https://github.com/yddgit/my-program-doc/raw/master/images/javascript-inherits.png "javascript-inherits")

  注意：函数`F`仅用于桥接，仅创建了一个`new F()`实例，而没有改变原有的`Student`定义的原型链。可以把继承这个动作用一个函数`inherits()`封装起来：

  ```javascript
  function inherits(Child, Parent) {
    var F = function() {};
    F.prototype = Parent.prototype;
    Child.prototype = new F();
    Child.prototype.constructor = Child;
  }

  // 这个inherits()函数可以复用
  function Student(props) {
    this.name = props.name || 'Unnamed';
  }
  Student.prototype.hello = function() {
    alert('Hello, ' + this.name + '!');
  }
  function PrimaryStudent(props) {
    Student.call(this, props);
    this.grade = props.grade || 1;
  }
  inherits(PrimaryStudent, Student);
  PrimaryStudent.prototype.getGrade = function() {
    return this.grade;
  };
  ```

  总结一下原型继承实现方式：

  * 定义新的构造函数，并在内部用`call()`调用希望“继承”的构造函数，并绑定`this`
  * 借助中间函数`F`实现原型链继承，最好通过封装的`inherits`函数完成
  * 继续在新的构造函数的原型上定义新方法

* class继承

  新的关键字`class`从**ES6**开始正式被引入到JavaScript中，目的就是定义类更简单

  上文用函数实现的`Student`，用新的`class`关键字来编写：

  ```javascript
  class Student {
    constructor(name) {
      this.name = name;
    }
    // 注意没有function关键字
    hello() {
      alert('Hello, ' + this.name + '!');
    }
  }
  var xiaoming = new Student('小明');
  xiaoming.hello();
  ```

  用`class`定义对象的另一个巨大的好处是继承更方便了，直接用`extends`来实现：

  ```javascript
  // PrimaryStudent的定义也是class关键字实现的，而extends则表示原型链对象来自Student
  class PrimaryStudent extends Student {
    // 子类的构造函数可能与父类不太相同，并且需要通过super(name)来调用父类的构造函数，否则父类的name属性无法正常初始化
    constructor(name, grade) {
      super(name);
      this.grade = grade;
    }
    // 自动获得父类的hello方法，定义新的myGrade方法
    myGrade() {
      alert('I am at grade ' + this.grade);
    }
  }
  ```

  `class`和原有的JavaScript原型继承没有任何区别，其作用就是让JavaScript引擎去实现原来需要我们自己编写的原型链代码。但并不是所有浏览器都支持`class`，如果一定要用，需要用工具把`class`代码转换为传统的`prototype`代码，如[Babel](https://babeljs.io/ "Babel")

## 浏览器

目前主流的浏览器：

1. IE6~11，历来对W3C标准支持最差，从IE10开始支持ES6标准
2. Chrome，Google出品的基于Webkit内核浏览器，内置JavaScript引擎-V8，支持ES6
3. Safari，Mac系统自带的基于Webkit内核的浏览器，从OS X 10.7 Lion自带6.1版本开始支持ES6
4. Firefox，Mozilla自己研制的Gecko内核和JavaScript引擎OdinMonkey
5. 移动设备Apple的Safari和Google的Chrome，均支持ES6

* 浏览器对象

  * `window`，不但充当全局作用域，而且表示浏览器窗口
    
    `window`对象的`innerWidth`和`innerHeight`属性，可以获取浏览器窗口的内部宽度和高度；`outerWidth`和`outerHeight`属性可以获取浏览器窗口的整个宽高。

  * `navigator`对象表示浏览器的信息，常用属性：

    * `navigator.appName`：浏览器名称
    * `navigator.appVersion`：浏览器版本
    * `navigator.language`：浏览器设置的语言
    * `navigator.platform`：操作系统类型
    * `navigator.userAgent`：浏览器设定的`User-Agent`字符串

    注意：`navigator`的信息可以很容易被用户修改，所以JavaScript读取的值不一定正确。所以不要用`if`判断浏览器版本，正确的方法是充分利用JavaScript对不存在的属性返回`undefined`的特性，直接用`||`计算：

    ```javascript
    // 不建议
    var width;
    if(getIEVersion(navigator.userAgent) < 9) {
      width = document.body.clientWidth;
    } else {
      width = window.innerWidth;
    }
    // 推荐
    var width = window.innerWidth || document.body.clientWidth;
    ```

  * `screen`对象表示屏幕信息，常用属性：

    * `screen.width`：屏幕宽度，以像素为单位
    * `screen.height`：屏幕高度，以像素为单位
    * `screen.colorDepth`：返回颜色位数，如8、16、24
  
  * `location`对象表示当前页面的URL信息

    如：`http://www.example.com:8080/path/index.html?a=1&b=2#TOP`

    ```javascript
    location.href; // http://www.example.com:8080/path/index.html?a=1&b=2#TOP
    location.protocol; // http
    location.host; // www.example.com
    location.port; // 8080
    location.pathname; // /path/index.html
    location.search; // ?a=1&b=2
    location.hash; // #TOP
    location.assign(); // 加载一个新页面
    location.reload(); // 重新加载当前页面
    ```

  * `document`对象表示当前页面，由于HTML以DOM形式表示为树形结构，`document`就是整个DOM树的根节点

    `document`有一个`cookie`属性，可以获取当前页面的`cookie`。

    ```javascript
    document.cookie; // v=123; remember=true; prefer=zh
    ```

    HTTP协议是无状态的，服务器使用Cookie来区分到底是哪个用户发过来的请求。JavaScript能够读取页面Cookie，而用户登录信息通常也存在Cookie中，这就存在巨大的安全隐患。如果页面引入的第三方JavaScript存在恶意代码，那用户登录信息将会泄漏。

    为了解决这个问题，服务器在设置Cookie时可以使用`httpOnly`，设定了`httpOnly`的Cookie将不能被JavaScript读取，这个行为由浏览器实现，主流浏览器均支持，IE6从SP1开始支持。服务器端应该始终坚持使用`httpOnly`

  * `history`对象保存了浏览器的历史记录。`back()`或`forward()`相当于用户点击了“后退”或“前进”按钮

* 操作DOM

  ```javascript
  var test = document.getElementById('test');
  var trs = document.getElementById('test-table').getElementsByTagName('tr');
  var reds = document.getElementById('test-div').getElementsByClassName('red');
  var cs = test.children; // 获取所有直属子节点
  var first = test.firstElementChild;
  var last = test.lastElementChild;
  var q1 = document.querySelector('#q1');
  var ps = q1.querySelectorAll('div.highlighted > p');
  // IE<8不支持querySelector和querySelectorAll，IE8仅有限支持

  // 更新DOM

  var p = document.getElementById('p-id');
  p.innerHTML = 'ABC';
  p.innerHTML = 'ABC <span style="color:red">RED</span> XYZ';
  p.innerText = '<script>alert("Hi");</script>'; // 自动进行HTML编码
  p.textContent; // innerText不返回隐藏元素的文本，textContent返回所有文本，IE<9不支持textContent
  // 修改CSS
  p.style.color = '#ff0000';
  p.style.fontSize = '20px';
  p.style.paddingTop = '2em';

  // 插入DOM

  p.innerHTML = '<span>child</span>'; // 相当于插入，但会直接替换掉原来的所有子节点

  // HTML结构：
  // <p id="js">JavaScript</p>
  // <div id="list">
  //   <p id="java">Java</p>
  //   <p id="python">Python</p>
  //   <p id="scheme">Scheme</p>
  // </div>
  var js = document.getElementById('js');
  var list = document.getElementById('list');
  list.appendChild(js);
  // HTML结构变成了：
  // <div id="list">
  //   <p id="java">Java</p>
  //   <p id="python">Python</p>
  //   <p id="scheme">Scheme</p>
  //   <p id="js">JavaScript</p>
  // </div>
  // 因为我们插入的js节点已经存在于当前的文档树，因此这个节点首先会从原先的位置删除，再插入到新的位置
  
  // 更多的时候会创建一个新的结点插入到指定位置
  var haskell = document.createElement('p');
  haskell.id = 'haskell';
  haskell.innerText = 'Haskell';
  list.appendChild(haskell);
  // <div id="list">
  //   <p id="java">Java</p>
  //   <p id="python">Python</p>
  //   <p id="scheme">Scheme</p>
  //   <p id="haskell">Haskell</p>
  // </div>

  // 动态创建节点可以实现很多功能，如：动态创建一个style节点，然后添加到head节点末尾，就动态的给文档添加了新的CSS样式
  var d = document.createElement('style');
  d.setAttribute('type', 'text/css');
  d.innerHTML = 'p { color: red; }';
  document.getElementsByTagName('head')[0].appendChild(d);

  // insertBefore
  parentElement.insertBefore(newElement, referenceElement); // 子节点会插入到referenceElement之前

  // 删除DOM

  var self = document.getElementById('to-be-removed');
  var parent = self.parentElement;
  var removed = parent.removeChild(self);
  removed === self; // true
  ```

* 操作表单

  * 文本框：`<input type="text">`
  * 密码框：`<input type="password">`
  * 单选框：`<input type="radio">`
  * 复选框：`<input type="checkbox">`
  * 下拉框：`<select>`
  * 隐藏域：`<input type="hidden">`

  ```javascript
  // 获取值
  // <input type="text" id="email">
  var input = document.getElementById('email');
  input.value; // 用户输入的值
  // 以上方式可用于text、password、hidden、select
  // 对于单选框和复选框，要用checked判断
  // <label><input type="radio" name="weekday" id="monday" value="1">Monday</label>
  // <label><input type="radio" name="weekday" id="tuesday" value="2">Tuesday</label>
  var mon = document.getElementById("monday");
  var tue = document getElementById("tuesday");
  mon.value; // 1
  tue.value; // 2
  mon.checked; // true或false
  tue.checked; // true或false

  // 设置值
  // <input type="text" id="email">
  var input = document.getElementById("email");
  input.value = 'test@example.com';
  // 对于单选框和复选框，设置checked为true或false即可

  // HTML5
  // date、datetime、datetime-local、color
  // <input type="date" value="2019-04-28">
  // <input type="datetime-local" value="2019-04-28T19:56:04">
  // <input type="color" value="#ff0000">
  // 不支持HTML5的浏览器会将其做为text显示

  // 提交表单的两种方式

  // 1. 通过<form>元素的submit()方法提交，缺点是扰乱了浏览器对form的正常提交
  // <form id="test-form">
  //   <input type="text" name="test">
  //   <button type="button" onclick="doSubmitForm()">Submit</button>
  // </form>
  function doSubmitForm() {
    var form = document.getElementById('test-form');
    // 修改form的input...
    form.submit();
  }

  // 2. 响应<form>的onsubmit事件
  // <form id="test-form" onsubmit="return checkForm()">
  //   <input type="text" name="test">
  //   <button type="submit">Submit</button>
  // </form>
  function checkForm() {
    var form = document.getElementById('test-form');
    // 修改校验...
    return true; // true继续提交，false终止提交
  }

  // 检查和修改<input>时要充分利用<input type="hidden">来传递数据
  // <form>
  //   <input type="text" id="username" name="username">
  //   <input type="password" id="input-password">
  //   <input type="hidden" id="md5-password" name="password">
  //   <button type="submit">Submit</button>
  // </form>
  function checkForm() {
    var input_pwd = document.getElementById('input-password');
    var md5_pwd = document.getElementById('md5-password');
    md5_pwd.value = toMD5(input_pwd.value);
    return true
  }

  // 注意到id为md5-password的<input>标记了name="password"，而用户输入的id为input-password的<input>没有name属性。没有name属性的<input>的数据不会被提交
  ```

* 操作文件

  HTML表单中唯一可以上传文件的控制`<input type="file">`，`method`必须为`post`，`enctype`必须为`multipart/form-data`

  ```html
  <form id="test-form" method="post" enctype="multipart/form-data">
    <input type="file" name="avatar">
  </form>
  ```

  只允许点击选择本地文件，对`<input type="file">`的`value`赋值没有任何效果，JavaScript也无法获得该文件的真实路径

  ```javascript
  var f = document.getElementById('test-file-upload');
  var filename = f.value; // C:\fakepath\test.png
  if(!filename || !(filename.endsWith('.jpg') || filename.endsWith('.png') || filename.endsWith('.gif'))) {
    alert('Can only upload image file');
    return false;
  }
  ```

  HTML5新增`File API`允许JavaScript读取文件内容

  ```javascript
  var
    fileInput = document.getElementById('test-image-file'),
    info = document.getElementById('test-file-info'),
    preview = document.getElementById('test-image-preview');
  // 监听change事件
  fileInput.addEventListener('change', function() {
    // 清除背景图片
    preview.style.backgroundImage = '';
    // 检查文件是否选择
    if(!fileInput.value) {
      info.innerHTML = '没有选择文件';
      return;
    }
    // 获取File引用
    var file = fileInput.files[0];
    // 获取File信息
    info.innerHTML = '文件：' + file.name + '<br>' +
                     '大小：' + file.size + '<br>' +
                     '修改：' + file.lastModifiedDate;
    if(file.type !== 'image/jpeg' && file.type !== 'image/png' && file.type !== 'image/gif') {
      alert('不是有效的图片文件');
      return;
    }
    // 读取文件
    var reader = new FileReader();
    reader.onload = function(e) {
      var
        data = e.target.result; // data:image/png;base64,iVBORw0KGgoAAAAN...
      preview.style.backgroundImage = 'url(' + data + ')';
    };
    // 以DataURL的形式读取文件，读到的文件是一个字符串，类似data:image/jpeg;base64,iVBOR...常用于设置图像
    reader.readAsDataURL(file);
  });

  // 上面的代码还演示了JavaScript一个重要特性：单线程执行模式
  // 在JavaScript中执行多任务实际上都是异步调用，所以对于读取文件内容这种异步操作需要先设置一个回调函数
  reader.onload = function(e) {
    // 文件读取完成后自动调用此函数...
  };
  ```

* AJAX

  ```javascript
  'use strict';
  function success(text) {
    var textarea = document.getElementById('test-response-text');
    textarea.value = text;
  }
  function fail(code) {
    var textarea = document.getElementById('text-response-text');
    textarea.value = 'Error code: ' + code;
  }

  // 新建XMLHttpRequest对象
  var request;
  if(window.XMLHttpRequest) {
    request = new XMLHttpRequest;
  } else {
    request = new ActiveXObject('Microsoft.XMLHTTP'); // 兼容低版本的IE
  }

  request.onreadystatechange = function() { // 状态发生变化时，函数被回调
    if(request.readyState === 4) { // 成功完成
      // 判断响应结果
      if(request.status === 200) {
        return success(request.responseText);
      } else {
        return fail(request.status);
      }
    } else {
      // HTTP请求还在继续
    }
  }
  // 发送请求
  request.open('GET', '/api/categories'); // open()还有第3个参数指定是否异步，默认true
  request.send();
  alert('请求已发送，请等待响应...');
  ```

  AJAX不能跨域请求，跨域请求实现方式：

  * 通过Flash插件发送HTTP请求，但必须安装Flash
  * 在同源域名下架设一个代理服务器来转发，但需要服务端额外做开发
  * JSONP，只能用GET请求，并且要求返回JavaScript。实际是利用了浏览器允许跨域引用JavaScript资源，如：返回JavaScript内容为`foo('data')`，我们在页面上先准备好`foo()`函数，然后给页面动态加个`<script>`节点，相当于动态读取外域的JavaScript资源，最后就等着接收回调了

  HTML5则有新的跨域策略：CORS(Cross-Origin Resource Sharing)

  Origin表示本域，当JavaScript向外域发起请求后，浏览器收到响应后，首先检查`Access-Control-Allow-Origin`是否包含本域，如果是，则此次跨域请求成功，如果不是，则请求失败，JavaScript将无法获取到响应的任何数据。`Access-Control-Allow-Origin: *`表示接受所有跨域请求

  以上这种跨域称为“简单请求”：`GET`、`HEAD`、`POST`(`Content-Type`仅限`application/x-www-form-urlencoded`、`multipart/form-data`和`text/plain`)，且不能出现任何自定义头(如：`X-Custom: 12345`)

  对于`PUT`、`DELETE`以及其他类型如`application/json`的POST请求，发送AJAX请求之前，浏览器会先送一个`OPTIONS`请求(称为preflignted请求)到这个URL上，询问目标服务器是否接受

  ```
  OPTIONS /path/to/resource HTTP/1.1
  Host: bar.com
  Origin: http://my.com
  Access-Control-Request-Method: POST
  ```

  服务器必须响应并明确指出允许的Method

  ```
  HTTP/1.1 200 OK
  Access-Control-Allow-Origin: http://my.com
  Access-Control-Allow-Methods: POST, GET, PUT, OPTIONS
  Access-Control-Max-Age: 86400
  ```

  浏览器确认服务器响应的`Access-Control-Allow-Methods`确实包含要发送的AJAX请求后的Method才会继续发送请求

