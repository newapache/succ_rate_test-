#!/usr/bin/python
# -*- coding:utf-8 -*-
import os
import sys
import time
import shutil

def poi_admin():
    with open('PoiData_All.txt', 'r') as f1:
        pois = f1.readlines()
    with open('AdminCode.txt', 'r') as f2:
        admins = f2.readlines()

    admin_arr = []
    for admin in admins:
        admin_arr.append(admin.strip().split('|'))

    for poi in pois:
        poi_arr = poi.strip().split('|')
        for admin in admin_arr:
            # 지번만 존재하는 poi
            if poi_arr[11] == admin[0] and (poi_arr[12] and not poi_arr[13]):
                print(admin[1], admin[2], admin[3], admin[4], poi_arr[12], 0)
                break
            # 지번, 도로명 , 건물번호 모두 존재하는 poi
            elif poi_arr[11] == admin[0] and (poi_arr[12] and poi_arr[13] and poi_arr[14]):
                print(admin[1], admin[2], admin[3], admin[4], poi_arr[12], 0)
                print(admin[1], admin[2], poi_arr[13], poi_arr[14], 1)
                break
            # 도로명, 건물번호만 존재하는 poi
            elif poi_arr[11] == admin[0] and (not poi_arr[12] and poi_arr[13]):
                print(admin[1], admin[2], poi_arr[13], poi_arr[14], 1)
                break

if __name__ == '__main__':
	print('Start')
	start = time.time()

	poi_admin()

	end = time.time()
	elapsed = end - start
	print('\n\nCompleted')
	print('Elapsed Time: ' + str(elapsed))
