#!/bin/bash

cd test
gcc -o test_json_parser test_json_parser.c
./test_json_parser
rm test_json_parser

gcc -o test_jetmp test_jetmp.c
./test_jetmp
rm test_jetmp
