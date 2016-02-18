A First Level Header
====================
A Second Level Header
---------------------

#Paragraph

Now is the time for all good men to come to the aid of their country. This is just a regular paragraph.

The quick brown fox jumped over the lazy dog's back

# Header 1
## Header 2
### Header 3
#### Header 4
##### Header 5
###### Header 6

#Quote

> This is a blockquote.
>
> This is the second paragraph in the blockquote.
> 
> > This is nested blockquote.
>
> Back to the first level.
>
> 1. List Item
> 2. List Item
>
> Some Shell Script:
>
> `return shell_exec("echo $input | $markdown_script");`
>
> ## This is an H2 in a blockquote

#Emphasized

Some of these words *are emphasized*.

Some of these words _are emphasized_.

Use two asterisks for **strong emphasized**.

Or, if you prefer, __use two underscores instead__.

\*This text is surrounded by literal asterisks\*

#List

* Candy.
+ Gum.
- Booze.


1. Red
2. Green
3. Blue


* A list item.

	With multiple paragraphs.
	> This is a blockquote
	> inside a list item.
	
	Some source code:
		int main() { return 0; }
		
	2015\. What a great season.

* Another item in the list.


#Link

This is an [example link](http://example.com/).

This is an [example link](http://example.com/ "with a title");

I get 10 times more traffic from [Google][1] than from [Yahoo][2] or [MSN][3].

[1]: http://www.google.com/ "Google"
[2]: http://search.yahoo.com/ "Yahoo Search"
[3]: <http://search.msn.com/> (MSN Search)

I start my morning with a cup of coffee and [The New York Times][NY Times].

[ny times]: http://www.nytimes.com/

<http://www.example.com/>

<address@example.com>

#Image

![alt text](http://www.google.cn/landing/cnexp/google-search.png "Title")

![alt text][id]

[id]: http://www.google.cn/landing/cnexp/google-search.png "Title"

#Code

I strongly recommend against using any `<blink>` tags.

I wish SmartyPants used named entites like `&mdash;` instead of decimal-encoded entites like `&#8212;`.

``There is a literal backtick (`) here.``

If you want your page to validate under XHTML 1.0 Strict, you've got to put paragraph tags in your blockquotes:

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

#Separater

* * *

***

*****

- - -

----------------

#Escape

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