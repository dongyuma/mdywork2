from __future__ import print_function

import argparse
import os
import re
import sys


#删除CMakeLists.txt文件中的相关信息(信息格式为大驼峰命名+"Check.cpp")
def adapt_cmake(module_path, check_name_camel):
  print("--------------------------修改CMakeLists.txt文件-----------------------")
  #定义文件寻找标志
  found=False
  #获取CMakeLists.txt的具体路径
  filename = os.path.join(module_path, 'CMakeLists.txt')
  print("CMakeLists.txt文件的具体路径为",filename)
  #读取CMakeLists.txt文件
  with open(filename, 'r') as f:
    lines = f.readlines()
  #获取要删除的文件信息
  cpp_file = check_name_camel + '.cpp'
  print("CMakeLists.txt文件中要删除的信息是",cpp_file)
  print('正在更新%s...' % filename)
  with open(filename, 'w') as f:
    for line in lines:
      #在每一行中寻找要删除的文本
      match=re.search(cpp_file,line)
      #如果查找到，就不要写入了(相当于删除)
      if match:
        found=True
        print("CMakeLists.txt文件更新完成")
        continue
      #将不符合的文本写入
      f.write(line)
  if found==False:
    print("要删除的checker文件不存在，请检查输入!")
    return False
  return True

#删除checker创建过程中的.h文件(位于用户指定的文件夹中，如mpi文件夹下)
def delete_header(module_path, module, namespace, check_name, check_name_camel):
  print("--------------------------删除checker创建过程中的.h文件---------------------------")
  #获取module名+checker名变量
  check_name_dashes = module + '-' + check_name
  #获取对应的.h文件名称
  filename = os.path.join(module_path, check_name_camel) + '.h'
  print('正在删除%s...' % filename)
  #移除指定文件
  os.remove(filename)
  print("已经成功删除",filename)


#删除checker创建过程中的.cpp文件(位于用户指定的文件夹中，如mpi文件夹下)
def delete_implementation(module_path, check_name_camel):
  print("--------------------删除checker创建过程中的.cpp文件-------------------------------")
  #获取要删除的文件路径
  filename = os.path.join(module_path, check_name_camel) + '.cpp'
  print('正在删除%s...' % filename)
  os.remove(filename)
  print("成功删除",filename)

#更新MPITidyModule.cpp文件(主要更新两个部分的内容，一个是删除include头文件，一个是删除函数中的引用添加)
def adapt_module(module_path, module, check_name, check_name_camel):
  print("--------------------更新MPITidyModule.cpp文件----------------------------")
  #获取MPITidyModule.cpp文件名
  modulecpp = list(filter(
      lambda p: p.lower() == module.lower() + 'tidymodule.cpp',
      os.listdir(module_path)))[0]
  #获取MPITidyModule.cpp文件路径
  filename = os.path.join(module_path, modulecpp)
  #读取文件内容
  with open(filename, 'r') as f:
    lines = f.readlines()

  print('正在修改%s...' % filename)
  with open(filename, 'w') as f:
    #获取模块名-checker名称
    check_fq_name = module + '-' + check_name
    #构造完整字符串(仅做提示，下文没有使用)
    check_decl = ('    CheckFactories.registerCheck<' + check_name_camel +
                  '>(\n        "' + check_fq_name + '");\n')
    #第一个要删除的字符串(include部分要删除的字符串)
    delete1='#include "'+check_name_camel+'.h"'
    #第二个要删除的字符串(第二部分的第一行，文件名用的是驼峰命名)
    delete2='    CheckFactories.registerCheck<' + check_name_camel +'>\('
    #第三个要删除的字符串(第二部分的第二行，文件名用的是原始命名)
    delete3='        "'+check_fq_name+'"\);'
    lines = iter(lines)
    try:
      while True:
        line = next(lines)
        #寻找第一个要删除的字符串
        match = re.search(delete1, line)
        if match:
          continue
        #寻找第二个要删除的字符串
        match = re.search(delete2,line)
        if match:
          continue
        #寻找第三个要删除的字符串
        match=re.search(delete3,line)
        if match:
          continue
        f.write(line)
    except StopIteration:
      print("MPITidyModule.cpp文件更新完成")
      pass


#更新ReleaseNotes.rst文件
def adapt_release_notes(module_path, module, check_name):
  print("------------------------更新ReleaseNotes.rst文件--------------------------------")
  #获取module名+checker名变量(原始命名)
  check_name_dashes = module + '-' + check_name
  #获取ReleaseNotes.rst文件目录
  filename = os.path.normpath(os.path.join(module_path,
                                           '../../docs/ReleaseNotes.rst'))
  #读取ReleaseNotes.rst文件
  with open(filename, 'r') as f:
    lines = f.readlines()

  print('正在修改 %s...' % filename)
  #构造匹配字符串
  test='- New :doc:`'
  #构造第一行要删除的数据
  delete1='- New :doc:`'+check_name_dashes
  #构造第二行要删除的数据
  delete2='  <clang-tidy/checks/'+check_name_dashes+'>` check.'
  #由于后面的数据没有特异性，因此构造删除标志flag,标志为0表示不删除
  flag=0
  with open(filename, 'w') as f:
    for line in lines:
      #查找第一行数据
      match=re.search(delete1,line)
      if match:
        print("正在删除",line)
        continue
      #查找第二行数据
      match=re.search(delete2,line)
      if match:
        print("正在删除",line)
        #删除标志置为1，表示开始删除后面的数据
        flag=1
        continue
      #检测删除标志决定是否删除该行数据
      if flag==1:
        #检测是否删除到了末尾
        match=re.search(test,line)
        #检测已经删除到末尾
        if match:
          #删除标志置0
          flag=0
        #如果删除标志为1
        if flag==1:
          print("正在删除",line)
          #删除该行
          continue
      f.write(line)
  print("ReleaseNotes.rst文件更新完成")


#删除clang-tools-extra/test/clang-tidy/checkers下的cpp文件
def delete_test(module_path, module, check_name, test_extension):
  print("----------------删除clang-tools-extra/test/clang-tidy/checkers下的cpp文件--------------------------")
  check_name_dashes = module + '-' + check_name
  #获取cpp文件路径
  filename = os.path.normpath(os.path.join(module_path, '../../test/clang-tidy/checkers',
                                           check_name_dashes + '.' + test_extension))
  print('正在删除%s...' % filename)
  os.remove(filename)
  print("成功删除",filename)

#好像没用到，先留着吧
def get_actual_filename(dirname, filename):
  if not os.path.isdir(dirname): 
    return ""
  name = os.path.join(dirname, filename)
  if (os.path.isfile(name)):
    return name
  caselessname = filename.lower()
  for file in os.listdir(dirname):
    if (file.lower() == caselessname):
      return os.path.join(dirname, file)
  return ""


#更新clang-tools-extra/docs/clang-tidy/checks下的list.rst文件
def update_checks_list(clang_tidy_path, module, check_name):
  is_deleted=False
  print("---------------------------更新list.rst文件--------------------------------")
  #形成非驼峰名称
  check_name_dashes = module + '-' + check_name
  #获取文件目录
  docs_dir = os.path.join(clang_tidy_path, '../docs/clang-tidy/checks')
  #获取要打开的文件名
  filename = os.path.normpath(os.path.join(docs_dir, 'list.rst'))
  #读取list.rst文件
  with open(filename, 'r') as f:
    lines = f.readlines()
  print('正在更新%s...' % filename)
  #要删除的字符串(这个部分在生成函数中还是挺复杂的，看了一下list.rst文件，里面应该是存储了多种不同的类型记录，包括下面的Yes，也是指示其有没有自动修复功能)
  delete1='   `'+check_name_dashes+' <'+check_name_dashes+'.html>`_,'
  with open(filename, 'w') as f:
    for line in lines:
      #匹配字符串
      match=re.search(delete1,line)
      if match:
        is_deleted=True
        continue
      f.write(line)
  if is_deleted==True:
    print("list.rst文件更新完成")
  if is_deleted==False:
    print("list.rst文件更新失败，请检查list.rst文件中的checker记录是否具有上述字符串变量的形式，如果不具有可选择手动删除！")



# 删除clang-tools-extra/docs/clang-tidy/checks下的rst文件
def delete_docs(module_path, module, check_name):
  print("-------------------------删除.rst文件--------------------------------")
  check_name_dashes = module + '-' + check_name
  #获取文件目录
  filename = os.path.normpath(os.path.join(
      module_path, '../../docs/clang-tidy/checks/', check_name_dashes + '.rst'))
  print('正在删除%s...' % filename)
  os.remove(filename)
  print("成功删除",filename)

#获取名称的驼峰命名字符串并给字符串加上Check，例如用户输入的是"test-first",该函数会将该字符串转换为"TestFffffCheck"
def get_camel_name(check_name):
  return ''.join(map(lambda elem: elem.capitalize(),
                     check_name.split('-'))) + 'Check'

#主函数
def main():
  language_to_extension = {
      'c': 'c',
      'c++': 'cpp',
      'objc': 'm',
      'objc++': 'mm',
  }
  #编写命令行接口用于获取用户输入的参数
  parser = argparse.ArgumentParser()
  #列出文件列表，随后退出，不执行删除操作
  parser.add_argument(
      '--update-docs',
      action='store_true',
      help='just update the list of documentation files, then exit')
  #定义语言变量，默认为C++语言
  parser.add_argument(
      '--language',
      help='language to use for check (defaults to c++)',
      choices=language_to_extension.keys(),
      default='c++',
      metavar='LANG')
  #定义要删除checker所在目录
  parser.add_argument(
      'module',
      nargs='?',
      help='module directory to delete the useless checker (e.g., misc)')
  #定义要删除的checker名称
  parser.add_argument(
      'check',
      nargs='?',
      help='name of the useless check to delete (e.g. foo-do-the-stuff instead of FooDoTheStuffCheck),')
  #对用户输入进行解析
  args = parser.parse_args()
  #如果用户选择的是列出文件列表功能，显示错误提示
  if args.update_docs:
    print("程序暂不支持列出文件列表功能")
    return
  #如果用户没有正确指出checker的位置和名称，应返回错误提示
  if not args.module or not args.check:
    print('请正确输入checker所在的文件夹(如mpi)和定义checker时使用的名称')
    parser.print_usage()
    return

  #获取用户输入的checker目录名称
  module = args.module
  #获取用户输入的checker名称
  check_name = args.check
  #把用户输入的checker名称转换为大驼峰命名并在末尾添加"Check"
  check_name_camel = get_camel_name(check_name)
  #获取本文件的目录(由于文件放置在clang-tidy目录下，因此本文件的目录也就是clang-tidy的目录)
  clang_tidy_path = os.path.dirname(sys.argv[0])
  #连接形成checker的目录
  module_path = os.path.join(clang_tidy_path, module)
  #如果给定的checker不存在，结束程序
  if not adapt_cmake(module_path, check_name_camel):
    return
  #将模块名称映射到与广泛使用的顶级命名空间不冲突的命名空间名称(该操作是add_new_check.py所规定，为保证删除过程中能够正确寻找到checker，特地保留，一般无用)
  if module == 'llvm':
    namespace = module + '_check'
  else:
    namespace = module
  #删除checker创建过程中的.h文件
  delete_header(module_path, module, namespace, check_name, check_name_camel)
  #删除checker创建过程中的.cpp文件
  delete_implementation(module_path, check_name_camel)
  #更新MPITidyModule.cpp文件
  adapt_module(module_path, module, check_name, check_name_camel)
  #更新ReleaseNotes.rst文件
  adapt_release_notes(module_path, module, check_name)
  #获取语言类型，这里是c++
  test_extension = language_to_extension.get(args.language)
  #删除clang-tools-extra/test/clang-tidy/checkers下的cpp文件
  delete_test(module_path, module, check_name, test_extension)
  #删除clang-tools-extra/docs/clang-tidy/checks下的rst文件
  delete_docs(module_path, module, check_name)
  #更新clang-tools-extra/docs/clang-tidy/checks下的list.rst文件
  update_checks_list(clang_tidy_path, module, check_name)
  print('checker删除成功！')


if __name__ == '__main__':
  main()
