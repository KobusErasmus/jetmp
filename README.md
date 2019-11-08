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
jetmp template json
```
where *template* is a file and *json* is a JSON string.

### Variables
In your template, simply put the JSON key in between {{ and }}. For example, a template may look something like this:
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
jetmp template.html '{"name":"Jacobus"}'
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

### Escaping HTML
If you wish to escape the HTML in the JSON values, add the --escape-html flag
at the end of the command, e.g.:
```
jetmp template.html json.json --escape-html
```
This escapes the following five characters: &, <, >, ", and '.

Please note that this might not be sufficient for your security. For more info,
see:
https://wonko.com/post/html-escaping
https://www.owasp.org/index.php/XSS_(Cross_Site_Scripting)_Prevention_Cheat_Sheet

### Maximum Keys, Key Length, and Value Length
By default, the maximum number of keys allowed in the JSON is 100. To change
this, use the [--keys INT] flag. For example, to set the maximum keys to 20:
```
jetmp template.html json.json --escape-html --keys 20
```

By default, the maximum size of a key is 50. To change
this, use the [--key-length INT] flag. For example, to set the key size to 200:
```
jetmp template.html json.json --escape-html --key-length 200
```

By default, the maximum size of a value is 1000. To change
this, use the [--value-length INT] flag. For example, to set the value size to 2000:
```
jetmp template.html json.json --escape-html --value-length 2000
```

### Another Example
Consider the ERB example found here: https://ruby-doc.org/stdlib-2.5.1/libdoc/erb/rdoc/ERB.html

Our template may look as follows:
```
From:  James Edward Gray II <james@grayproductions.net>
To:  {{to}}
Subject:  Addressing Needs

{{to_name}}:

Just wanted to send a quick note assuring that your needs are being
addressed.

I want you to know that my team will keep working on the issues,
especially:

  * {{priority1}}
  * {{priority2}}
  * {{priority3}}

Thanks for your patience.

James Edward Gray II
```
Let the above template file be called "jetmp_template".

Let us now write a script called benchmark.sh:
```
#!/bin/bash

json='{
  "to":"Community Spokesman <spokesman@ruby_community.org>",
  "to_name":"Community",
  "priority1":"Run Ruby Quiz",
  "priority2": "Document Modules",
  "priority3":"Answer Questions on Ruby Talk"}'

puts "JETmp benchmark:"
time jetmp jetmp_template "$json"
```
Make this script executable and run it to run a simple benchmark.

## About Author
See www.JacobusErasmus.com
