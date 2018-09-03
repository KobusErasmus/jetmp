#!/bin/bash

cd test

# Test json_parser.c
gcc -o test_json_parser test_json_parser.c
./test_json_parser
rm test_json_parser

# Test jetmep.c
gcc -o jetmp ../lib/jetmp.c
filename="html_page.html"
json='{
  "name":"Tom Blue",
  "age": 32, "Gender" : Male,
  "height"   :67.0
}'
expected='<html>
  <head></head>
  <body>
    <h1>Hello Tom Blue<h1>
    <span class="the class">
      Gender: Male, height: 67.0
    </span>
    <div>
      <a>The {anchor}</a>
      <a>Tom Blue32</a>
    </div>
  </body>
</html>'
result=`./jetmp $filename "$json"`
if [[ $result == $expected ]]
then
  echo "All tests pass for: jetmp.c"
else
  echo "FAIL: jetmp.c incorrect interpolation:"
  printf "$result"
fi
rm jetmp

# gcc -o test_jetmp test_jetmp.c
# ./test_jetmp
# rm test_jetmp

