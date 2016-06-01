A First Level Header（一级标题）
====================
A Second Level Header（二级标题）
---------------------

#Paragraph（段落）

Now is the time for all good men to come to the aid of their country. This is just a regular paragraph.（一般的段落，不需要缩进）

The quick brown fox jumped over the lazy dog's back（空行划分段落）

# Header 1（一级标题）
## Header 2（二级标题）
### Header 3（三级标题）
#### Header 4（四级标题）
##### Header 5（五级标题）
###### Header 6（六级标题）

#Quote（引用，以>开头）

> This is a blockquote.
>
> This is the second paragraph in the blockquote.
> 
> > This is nested blockquote.（嵌套引用）
>
> Back to the first level.
>
> 1. List Item（引用内的列表项1）
> 2. List Item（引用内的列表项2）
>
> Some Shell Script:
>
> `return shell_exec("echo $input | $markdown_script");`（引用内的脚本代码）
>
> ## This is an H2 in a blockquote（引用内的二级标题）

#Emphasized

Some of these words *are emphasized*.（斜体）

Some of these words _are emphasized_.（斜体）

Use two asterisks for **strong emphasized**.（加粗）

Or, if you prefer, __use two underscores instead__.（加粗）

\*This text is surrounded by literal asterisks\*（将*号原样输出）

#List

* Candy.（无序列表）
+ Gum.（无序列表）
- Booze.（无序列表）


1. Red（有序列表）
2. Green（有序列表）
3. Blue（有序列表）


* A list item.（列表项1）

	With multiple paragraphs.（列表项之间加上空行，段落前加上4个空格或1个tab即可使用内容被<p>标签包住
	> This is a blockquote
	> inside a list item.（列表项内的引用）
	
	Some source code:
		int main() { return 0; }（列表项内的代码）
		
	2015\. What a great season.（将列表项标记--英文句号--进行转义）

* Another item in the list.（列表项2）


#Link

This is an [example link](http://example.com/).（行内链接）

This is an [example link](http://example.com/ "with a title");（链接加title）

I get 10 times more traffic from [Google][1] than from [Yahoo][2] or [MSN][3].（引用外部定义链接）

[1]: http://www.google.com/ "Google"
[2]: http://search.yahoo.com/ "Yahoo Search"
[3]: <http://search.msn.com/> (MSN Search)

I start my morning with a cup of coffee and [The New York Times][NY Times].（链接名称可以用数字、字母、空格，不分大小写）

[ny times]: http://www.nytimes.com/

<http://www.example.com/>

<address@example.com>

#Image

![alt text](http://www.google.cn/landing/cnexp/google-search.png "Title")（行内图片）

![alt text][id]（引用外部定义的图片）

[id]: http://www.google.cn/landing/cnexp/google-search.png "Title"

#Code

I strongly recommend against using any `<blink>` tags.（\`标记行内代码）

I wish SmartyPants used named entites like `&mdash;` instead of decimal-encoded entites like `&#8212;`.

``There is a literal backtick (`) here.``

If you want your page to validate under XHTML 1.0 Strict, you've got to put paragraph tags in your blockquotes:（如果是一段代码，则将每行都缩进4个空格或1个tab即可）

	<blockquote>
	<p>For example.</p>
	</blockquote>

Here is an example of AppleScript:

	tell application "Foo"
		beep
	end tell

	<div class="footer">
		$copy; 2004 Foo Corporation
	</div>

GitHub code blocks:

```bash
echo "Hello World"
```

GitHub Syntax highlighting:

Linguist: https://github.com/github/linguist/blob/master/lib/linguist/languages.yml

#Separater（分隔符）

* * *

***

*****

- - -

----------------

#Escape（转义）

\*literal asterisks\*

The following characters can be escaped:
* \\
* \`
* \*
* \_
* \{ \}
* \[ \]
* \( \)
* \#
* \+
* \-
* \.
* \!