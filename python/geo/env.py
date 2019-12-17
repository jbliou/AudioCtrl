#! /usr/bin/python3
# -*- coding: utf-8 -*-
import os
from os import listdir
from os.path import isfile, isdir, join

mypath = os.getcwd()
mypath = mypath + '\\zip'

files = listdir(mypath)

for i in files:
    fullpath = join(mypath, i)
    if isfile(fullpath):
        print('檔案：', i)
    elif isdir(fullpath):
        print('目錄：', i)
