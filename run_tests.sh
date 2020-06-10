#!/bin/bash

print_test_pass() {
  printf "."
}

print_test_fail() {
  printf "\nFAIL: $1\nExpected:\n"
  printf "$expected"
  printf "\nActual:\n"
  printf "$result\n"
}

evaluate_result() {
  if [[ $result == $expected ]]
  then
    print_test_pass
  else
    print_test_fail "$1"
  fi
}

cd test
gcc -o jetmp ../src/jetmp.c

#Test 1
filename="test3.html"
expected="Usage: jetmp FILENAME KEY1:'VALUE 1' KEY2:'VALUE 2' ..."
result=`./jetmp $filename`
evaluate_result 'Test 1'
result=`./jetmp name:'Tom'`
evaluate_result 'Test 1'

#Test 2
filename="test_does_not_exist.html"
expected="Cannot open file test_does_not_exist.html"
result=`./jetmp $filename name:'Tom'`
evaluate_result 'Test 2'

#Test 3
filename="test3.html"
expected="<h1>Hello {Tom}</h1>"
result=`./jetmp $filename name:'Tom'`
evaluate_result 'Test 3'

#Test 4
filename="test4.html"
expected="<h1>Hello Tom Blue, see you tomorrow</h1>"
result=`./jetmp $filename name:'Tom Blue' day:tomorrow`
evaluate_result 'Test 4'

#Test 5
filename="test5.html"
expected="<h1>Hello JETmp syntax error: missing closing braces"
result=`./jetmp $filename name:'Tom Blue' day:tomorrow`
evaluate_result 'Test 5'

#Test 6
filename="test6.html"
expected="JETmp syntax error: missing closing braces"
result=`./jetmp $filename name:'Tom Blue' day:tomorrow`
evaluate_result 'Test 6'

#Test 7
filename="test7.html"
expected="<h1>&amp; &lt; &gt; &quot; &#39;</h1>"
result=`./jetmp $filename heading:"& < > \" '"`
evaluate_result 'Test 7'

#Test 8
filename="test8.html"
expected="<h1>&amp; &lt; &gt; &quot; &#39; & < > \" '<span>test</span></h1>"
result=`./jetmp $filename heading1:"& < > \" '" heading2:"& < > \" '" heading3:'<span>test</span>'`
evaluate_result 'Test 8'

#Test 9
filename="test9.html"
expected="<div>
<h1>Hello Tom Blue, see you tomorrow</h1>

</div>"
result=`./jetmp $filename name:'Tom Blue' day:tomorrow`
evaluate_result 'Test 9'

#Test 10
filename="test10.html"
expected="<div>

  User:
  <p>Tom Blue tom@test.com</p><br><br><br>
  <i>1234</i><i>5678</i>

  User:
  <p>Jack Johnson jack@test.com</p><br><br><br>
  <i>9</i><i>0</i>

</div>"
result=`./jetmp $filename repeat:3 hidden:0 heading:'User:' users:2 name1:'Tom' surname1:'Blue' email1:'tom@test.com' name2:'Jack' surname2:'Johnson' email2:'jack@test.com' ids:2 id1_1:"1234" id1_2:5678 id2_1:9 id2_2:0`
evaluate_result 'Test 10'

#Test 11
filename="test11.html"
expected="Author: Jack
Books: book1 book2 book3 
Author: Jill
Books: book4 book5 book6 "
result=`./jetmp $filename authors:2 author1:Jack author2:Jill author3:John books:3 book1_1:book1 book1_2:book2 book1_3:book3 book2_1:book4 book2_2:book5 book2_3:book6`
evaluate_result 'Test 11'

printf "\n"
rm jetmp
