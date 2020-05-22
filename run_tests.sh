#!/bin/bash

cd test

# Test json_parser.c
gcc -o test_json_parser test_json_parser.c
./test_json_parser
rm test_json_parser

# Test jetmep.c
gcc -o jetmp ../src/jetmp.c
filename="html_page.html"
json='{
  "garbage":"",
  "\quote\"":"\double\ quote: \"",
  "name":"Tom Blue",
  "age": 32, "Gender" : Male,
  "height"   :67.0
}'
expected='<html>
  <head></head>
  <body>
    <h1>Hello Tom Blue</h1>
    <span> Height: 67.0 </span>

    <span class="the class">
      Gender: Male, height: 67.0
      quote: \\double\\ quote: "
    </span>
    <div>
      <a>The {anchor}</a>
      <a>Tom Blue32</a>
    </div>
  </body>
</html>'
result=`./jetmp $filename "$json" --keys 10 --key-length 20 value-length 20`
if [[ $result == $expected ]]
then
  echo "All tests pass for: jetmp.c normal test"
else
  echo "FAIL: jetmp.c incorrect interpolation:"
  printf "$result"
fi

# Test escape html
filename="html_page.html"
json='{
  "garbage":"",
  "\quote\"":"\double\ quote: \"",
  "name":"Tom Blue",
  "age": "& < > \" '"'"'", "Gender" : Male,
  "height"   :67.0
}'
expected='<html>
  <head></head>
  <body>
    <h1>Hello Tom Blue</h1>
    <span> Height: 67.0 </span>

    <span class="the class">
      Gender: Male, height: 67.0
      quote: \\double\\ quote: &quot;
    </span>
    <div>
      <a>The {anchor}</a>
      <a>Tom Blue&amp; &lt; &gt; &quot; &#39;</a>
    </div>
  </body>
</html>'
result=`./jetmp $filename "$json" --escape-html`
if [[ $result == $expected ]]
then
  echo "All tests pass for: jetmp.c escape HTML test"
else
  echo "FAIL: jetmp.c HTML test incorrect interpolation:"
  printf "$result"
  printf "\n\n\n$expected"
fi

# Test long partial filename
filename="html_page2.html"
json='{"@_heading":"The Heading"}'
expected='
The Heading'
result=`./jetmp $filename "$json" --escape-html --keys 1 --key-length 9 --value-length 11`
if [[ $result == $expected ]]
then
  echo "All tests pass for: jetmp.c long partial filename test"
else
  echo "FAIL: jetmp.c long partial filename test incorrect interpolation:"
  printf "$result"
  printf "\n\n\n$expected"
fi

# Test loop
filename="loop.html"
json='{"heading":"Test looping", "names0":"John", "names1":"Jack", "names2":"Jill"}'
expected='<h1>Test looping</h1>

<h2>John</h2>
<h2>Jack</h2>
<h2>Jill</h2>
'
result=`./jetmp $filename "$json" --escape-html`
if [[ $result == $expected ]]
then
  echo "All tests pass for: jetmp.c loop test"
else
  echo "FAIL: jetmp.c loop test incorrect interpolation:"
  printf "$result"
  printf "\n\n\n$expected"
fi

rm jetmp
