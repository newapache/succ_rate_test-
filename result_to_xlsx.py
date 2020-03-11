#!/usr/bin/python
# -*- coding:utf-8 -*-


import os
import sys
import time
import shutil
from pandas import DataFrame


def save_xlsx():
    b1_param=[]
    b1_res=[]
    b2_param=[]
    b2_res=[]
    no_adm=[]
    res_fail=[]

    with open('result.txt', 'r') as f1:
        res = f1.readlines()

    for line in res:
    	# 주석 제거 후 순차 실행 
        # if line.split('|')[0].startswith('구주소 지번'):
        #     param_str = (line.split('|')[1]).replace('[','').replace(']','').replace("'",'').replace(',','')
        #     print('paramstr', param_str)
        #     a1_param.append(param_str[:-3])
        #     res_str = (line.split('|')[2]).replace('[','').replace(']','')
        #     print('res_str', res_str)
        #     a1_res.append(res_str)

        # if line.split('|')[0].startswith('구주소 주소'):
        #     param_str = line.split('|')[1].rsplit('[',1)[0].replace('[','').replace(']','').replace("'",'').replace(',','')
        #     #print('paramstr', param_str[:-2])
        #     res_str = line.rsplit('[',1)[1].replace(']','').replace("'",'').replace(',','')
        #     if '(' in res_str and ')' in res_str:
        #         #print('##### : ', res_str)
        #         continue
        #     a2_param.append(param_str[:-2])
        #     a2_res.append(res_str)

        # if line.split('|')[0].startswith('신주소 건번'):
        #     param_str = (line.split('|')[1]).replace('[','').replace(']','').replace("'",'').replace(',','')
        #     #print('paramstr', param_str[:-3])
        #     #b1_param.append(param_str[:-3])
        #     res_str = (line.split('|')[2]).replace('[','').replace(']','')
        #     #print('res_str', res_str)
        #     #b1_res.append(res_str)
        #
        # elif line.split('|')[0].startswith('신주소 주소'):
        #     param_str = line.split('|')[1].rsplit('[',1)[0].replace('[','').replace(']','').replace("'",'').replace(',','')
        #     #print('paramstr', param_str[:-2])
        #     res_str = line.rsplit('[',1)[1].replace(']','').replace("'",'').replace(',','')
        #     #print(res_str)
        #     if '세종' not in res_str:
        #         b2_param.append(param_str[:-2])
        #         b2_res.append(res_str)
            #     #print('##### : ', res_str)
            #     continue
            # a2_param.append(param_str[:-2])
            # a2_res.append(res_str)
        if line.split('|')[0].startswith('No'):
            param_str = line.split('|')[1].replace('[','').replace(']','').replace("'",'').replace(',','')
            #print(param_str[:-2])
            no_adm.append(param_str[:-2])

        elif line.split('|')[0].startswith('searching'):
            param_str = line.split('|')[1].replace('[','').replace(']','').replace("'",'').replace(',','')
            #print(param_str[:-2])
            res_fail.append(param_str[:-2])
            #print('res_str', res_str)

            # print('param : ', line.split('|')[1].rsplit('[',1)[0])
            # print(type(line.split('|')[1].rsplit('[',1)[0]))
            # print('res: ', line.rsplit('[',1)[1].replace(']',''))

        #     a2_param.append(line.split('|')[1].rsplit('[',1)[0])
        #     a2_res.append( line.rsplit('[',1)[1].replace(']','') )
        #
        # elif line.split('|')[0].startswith('신주소 주소'):
        #     b2_param.append(line.split('|')[1].rsplit('[',1)[0])
        #     b2_res.append( line.rsplit('[',1)[1].replace(']','') )


    # df1 = DataFrame({'adm 필드 없음 - param' : noadm_param})
    # df1.to_excel('te5.xlsx', sheet_name='sheet1', index=False)
    # df2 = DataFrame({'response fail - param' : false_param})
    # df2.to_excel('te6.xlsx', sheet_name='sheet1', index=False)

    df = DataFrame({'response fail' : res_fail})
    df.to_excel('te_6.xlsx', sheet_name='sheet1', index=False)


if __name__ == '__main__':
	print('Start')
	start = time.time()

	save_xlsx()

	end = time.time()
	elapsed = end - start
	print('\n\nCompleted')
	print('Elapsed Time: ' + str(elapsed))
