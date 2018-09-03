#!/bin/bash

gcc -O3 -o jetmp ./lib/jetmp.c
sudo mv jetmp /usr/bin/jetmp
