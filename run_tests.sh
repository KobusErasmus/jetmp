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

rm jetmp
