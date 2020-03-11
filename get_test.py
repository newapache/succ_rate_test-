#!/usr/bin/pythoni
# -*- coding:utf-8 -*-

import os
import sys
import time
import shutil
import json
import subprocess


def open_adds():
	fo = open("poi_to_add.txt",'r')
	adds=fo.readlines()
	fo.close()

	line=0
	for add in adds:
		#1print(add)
		line += 1
		if (line % 3000) != 0:
			continue
		arr = add.strip().split('%20')
		send_request(arr, add.rsplit('%20', 1)[0])


def send_request(arr, str):
	#print(arr, str)
	url = 'PATH' + str + ''
	proc = subprocess.Popen(["curl", url], stdout=subprocess.PIPE)
	(out, err) = proc.communicate()
	results = out.decode('utf-8')
	#1
	#print(results)
	parse_response(arr, results)
	

def parse_response(arr, dict_result):
	dict = json.loads(dict_result)

	# request false일 경우 -> 즉시 종료 
	if not dict['result']:
		return
	else:
		flag=100
		#지번 불일치 케이스 
		if arr[-1] == '0':
			for i in range(len(dict['adm'])):
				if dict['adm'][i]['jibun'] == arr[-2] :
					flag = i
			if flag == 100:
				print('구주소 지번 불일치', ' | ', arr, ' | ', dict['adm'])
				return
			else:
				#구주소 address filed check
				add_field_check(arr, dict['adm'][flag]['address'])
	
		#도로명 불일치 케이스 
		elif arr[-1] == '1':
			for i in range(len(dict['adm'])):
				if dict['adm'][i]['roadjibun'] == arr[-2]:
					flag = i
			if flag == 100:
				print('신주소 건번 불일치', ' | ', arr, ' | ', dict['adm'])
				return
			else:
				#신주소 address field check 
				add_field_check(arr, dict['adm'][flag]['roadname'])


def add_field_check(a, b):
	result = b.strip().split(' ')
	print(a[0:len(result)],'\t|\t',b)


if __name__ == '__main__':
	open_adds()
