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

* 标准对象

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
