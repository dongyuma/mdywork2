#!/usr/bin/python3
# -*- coding: utf-8 -*- 

#内置编译时使用该脚本创建临时Checker注册头文件

import sys
import os
import re
import os.path

INC_FILE_START="""
#ifndef _CHECK_SCOPE_CHECKERS_H_

void registCheckScopeBuiltinCheckers(void *arg);
"""

INC_FILE_MID="""
static inline void checkScopeCheckersShouldRegist()
{
"""

INC_FILE_END="""
}
#endif
"""


def getShouldRegisterFun(src_file):
    res=""
    pattern=re.compile(r'SCOPE_REGISTER_CLASS\((.*?)\)')
    if os.path.exists(src_file):
        fin = open(src_file,'r',encoding='utf-8')
        try:
            lines=fin.readlines()
        except:
            fin.close()
            print("[Error] CheckScopeBuiltinInclude.py at file: [%s] Please check whether the file code is UTF-8"%(src_file))
            return res
        fin.close()
        for line in lines:
            if len(line) == 0:
                continue
            mstr=pattern.search(line)
            if mstr:
                reg_class_name=mstr.group(1)
                if (len(reg_class_name) > 0 and reg_class_name != "ClassName"):
                    res="shouldRegister{CLASS_NAME}();".format(CLASS_NAME=reg_class_name)
    return res

def main(argc,argv):
    if argc < 4:
        exit(1)
    src_dir=os.path.abspath(argv[1])
    src_file_list=argv[2].split(";")
    dst_inc_dir=os.path.abspath(argv[3])
    dst_inc_file="{DST_INC_DIR}/CheckScopeCheckers.h".format(DST_INC_DIR=dst_inc_dir)
    reg_fun_list=[]

    for src_file in src_file_list:
        src_file_abl="{SRC_DIR}/{SRC_FILE}".format(SRC_DIR=src_dir,SRC_FILE=src_file)
        new_fun=getShouldRegisterFun(src_file_abl)
        if len(new_fun) > 0:
            reg_fun_list.append(new_fun)

    o_file=open(dst_inc_file,"w+",encoding='utf-8')
    o_file.write(INC_FILE_START)

    for fun in reg_fun_list:
        decl_str="bool {FUN}".format(FUN=fun)
        o_file.write(decl_str)
        o_file.write("\n")

    o_file.write(INC_FILE_MID)

    for fun in reg_fun_list:
        o_file.write(fun)
        o_file.write("\n")
    
    o_file.write(INC_FILE_END)
    o_file.close()

if __name__ == "__main__":
    main(len(sys.argv),sys.argv)
