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
  'toString' in xiaohong; // true，操作符in判断的属性可能是对象继承得到的
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
      x = 1; // x初始化为1
      y = x + 1; // y初始化为2
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
  ```