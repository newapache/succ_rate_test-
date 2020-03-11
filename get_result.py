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

	#line=0
	for add in adds:
		#1
		#print(add)
		#line += 1
		#if (line % 1000) != 0:
			#continue
		arr = add.strip().split('%20')
		send_request(arr, add.rsplit('%20', 1)[0])


def send_request(arr, str):
	#print(arr, str)
	url = 'PATH' + str + ''
	proc = subprocess.Popen(["curl", url], stdout=subprocess.PIPE)
	(out, err) = proc.communicate()
	results = out.decode('utf-8')
	#1
	# print(results)
	parse_response(arr, results)
	

def parse_response(arr, dict_result):
	dict = json.loads(dict_result)

	# request false인 경우 -> 즉시 종료 
	if not dict['result']:
		print('searching result : false', ' | ', arr)
		return
	else:
		flag=100
		try:
			# 구주소 지번 불일치 
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
	
			#도로명 불일치 
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
		except KeyError:
			print('No "adm" field in json', '|', arr)


def add_field_check(param, res):
	city=['시','군','구']
	dong=['읍','면','동','리']
	gil=['길','로','거리','가']

	res = res.strip().split(' ')
	#print(a[0:len(result)],'\t|\t',b)
	#print(param[1:-2], res[1:])
	param = null_check(param)

	if param[-1] == '1':
		if (param[1].endswith(tuple(city)) and res[1].endswith(tuple(city))) and (param[1] == res[1]):
			if (param[-3].endswith(tuple(gil)) and res[-1].endswith(tuple(gil))) and (param[-3] == res[-1]):
				#print('일치 | ',param, res)
				return
		print('신주소 주소 불일치 | ', param, res)
	
	elif param[-1] == '0':
		if (param[1].endswith(tuple(city)) and res[1].endswith(tuple(city))) and (param[1] == res[1]):
			if (param[-3].endswith(tuple(dong)) and res[-1].endswith(tuple(dong))) and (param[-3] == res[-1]):
				#print('일치 | ', param, res)
				return
			elif (param[-3].endswith(tuple(gil)) and res[-1].endswith(tuple(gil))) and (param[-3] == res[-1]):
				#print('일치 | ', param, res)
				return
		print('구주소 주소 불일치 | ', param, res)
	

def null_check(arr):
	temp=[]
	for item in arr:
		if item:
			temp.append(item)
	return temp


if __name__ == '__main__':
	open_adds()

