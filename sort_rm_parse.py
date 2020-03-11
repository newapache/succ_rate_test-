#!/usr/bin/python
# -*- coding:utf-8 -*-


import os
import sys
import time
import shutil
import json


def sorting():
    fo=open("result.txt",'r') 
    line=fo.readlines()
    mylist=sorted(set(line))
    fo.close()

    fo=open("result_sorted.txt",'w')
    fo.writelines(mylist)
    fo.close()


def remove_mt(): # mt : 불필요한 moutain 관련 주소 제거
    fo=open("result_sorted.txt",'r')
    lines=fo.readlines()
    fo.close()

    fo=open("result2.txt",'w')
    for line in lines:
        arr = line.split(' ')
        print(arr[0], arr[1], arr[2]) # output 파일 기록 
        # if not (arr[len(arr)-2]).startswith('산'):
        #     fo.writelines(line)
    fo.close()

def parse_result():
    fo=open("result.txt",'r')
    adds=fo.readlines()
    fo.close()

    fo=open("result_tmp.txt",'r')
    responses=fo.readlines()
    fo.close()

    total=0
    err=0
    adds_num=0

    for line in responses:
        try:
            dict = json.loads(line)
        except ValueError:
            print('Decoding JSON has failed', adds_num+1)
            err += 1
            continue
        if dict['result']:
            inner_dict = dict['adm'][0]
        else:
            print('result -> false ', adds_num+1, line)
            total += 1
            continue
        adds_num += 1
    print('not found : ', total + ( err / 5 ))


if __name__ == '__main__':
	#sorting()
	#remove_mt() // mountain관련 주소 제거 
	parse_result()
