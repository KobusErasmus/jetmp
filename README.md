# JETmp - A simple templating system, written in C.

## About
JETmp is inspired by the Mustache templating system (https://mustache.github.io). However, since I have far simpler templating requirements, I wrote this tiny, fast templating system.

## Installation
To install, first clone this repository:
```
git clone https://github.com/KobusErasmus/jetmp
```
Then, cd into jetmp (this cloned repository) and run the following:
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

## Another Example
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
