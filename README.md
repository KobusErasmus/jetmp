# JETmp - A simple templating system, written in C.

## About
JETmp is inspired by the Mustache templating system (https://mustache.github.io). However, since I have far simpler templating requirements, I wrote this tiny, fast templating system.

## Installation
If you have *wget*, copy and paste this in your terminal (all commands on one line):
```
wget www.jacobuserasmus.com/jetmp-0.1.tar.gz ; tar -xzvf jetmp-0.1.tar.gz ; cd jetmp-0.1 ; ./configure ; make ; sudo make install ; make clean
```
Or, you may follow the steps manually. First, download the tar file here: www.jacobuserasmus.com/jetmp-0.1.tar.gz

Then, unzip the file and cd into the unzipped file. Next, run the following:
```
./configure
make
sudo make install
make clean
```
If you wish to uninstall JETmp, run:
```
sudo make uninstall
```

## Usage
To run the program, use the following format:
```
jetmp template key1:"value 1" key2:"value 2" ...
```
where *template* is a file followed by some set of key/value pairs.

### Variables
In your template, simply put the keys in between {{ and }}. For example, a template may look something like this:
```
<html>
  <head></head>
  <body>
    <h1>Hello {{name}}<h1>
  </body>
</html>
```
Suppose that this template file is called "template.html". Then, you may run the command as follows:
```
jetmp template.html name:Jacobus
```
This will return:
```
<html>
  <head></head>
  <body>
    <h1>Hello Jacobus<h1>
  </body>
</html>
```

### Partials
To render another file or partial, put the file name in between {{> and }}. For example, your template.html file may look like this:
```
<html>
  <head></head>
  <body>
    {{>partial.html}}
  </body>
</html>
```
Your partial.html file may look like this:
```
<h1>Hello {{name}}<h1>
```
Your output will then look like this:
```
<html>
  <head></head>
  <body>
    <h1>Hello Jacobus<h1>
  </body>
</html>
```

### Loops

To create a loop, open the loop with the {{#KEY}} tag, where KEY
is a key whose value is an integer (or integer string) denoting
the loop count (i.e., how many times to loop). Close the loop
with the {{-}} tag. Tags within the loop that end with "#" will
try to be inserted by finding a key that resembles the tag with
the current loop index appended to the end.

For example, suppose your file.txt looks as follows:
```
{{heading}}
{{#names}}
Name: {{name#}} {{surname#}}
{{-}}
```
Then, if you run the following command:
```
jetmp file.txt heading:"Test loop" names:3 name1:"Jack" name2:"Jill" name3:"John" surname1:"Back" surname2:"Bill" surname3:"Bohn"
```
the output would be:
```
Test loop

Name: Jack Back

Name: Jill Bill

Name: John Bohn

```

Currently, you can only have nested loops of depth one. In other
words, you can have a second loop within an initial loop, but you
cannot place a third loop within the second loop. Things will
break. If you require deep nested loops, you can easily adjust
the code of JETmp accordingly.

An example of a nested loop: Suppose your file.txt contains the
following:
```
{{#authors}} Author: {{author#}}
Books: {{#books}}{{book#}} {{-}}
{{-}}
```
Then, if you run:
```
jetmp file.txt authors:2 author1:Jack author2:Jill books:3 book1_1:book1 book1_2:book2 book1_3:book3 book2_1:book4 book2_2:book5 book2_3:book6
```
the output will be:
```
Author: Jack
Books: book1 book2 book3
Author: Jill
Books: book4 book5 book6
```

### Escaping HTML

By default, the normal tag {{KEY}} escapes HTML, that is to say, when the
tag's value is inserted, the following five characters are
escaped: &, <, >, ", and '.

Please note that this might not be sufficient for your security.
For more info, see: https://wonko.com/post/html-escaping
https://www.owasp.org/index.php/XSS_(Cross_Site_Scripting)_Prevention_Cheat_Sheet

However, if you do not wish to escape HTML, then use the {{/KEY}}
tag and not the {{KEY}} tag.

## About Author
See www.JacobusErasmus.com
