# 阅境静态分析核心工具(base on llvm-clang)

## Linux编译

环境要求：

​	gcc > 5.9.0    python3    cmake > 3.13.4

支持多种类型的构建工具：

### Unix Makefiles (推荐)

```bash
cd Core
# git checkout your_branch
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release -DLLVM_ENABLE_PROJECTS="clang;clang-tools-extra" -G "Unix Makefiles" ../llvm
make -j8
```

### Ninja

```bash
cd Core
# git checkout your_branch
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release -DLLVM_ENABLE_PROJECTS="clang;clang-tools-extra" -G "Ninja" ../llvm
ninja -j8
```

* 注：编译该项目对硬件要求较高，请确保计算机内存至少为8GB

### 快速生成子模块

第一次全量编译比较耗时,当你对其中的某个子模块的代码修改后可能只需要生成这个被修改过的子模块就可以了，无需进行全量make去遍历所有子模块以节省编译时间。

```bash
cd build
make help #列出所有可独立生成的子模块
make CheckScopePlugin #仅快速生成CheckScopePlugin子模块
make clang-tidy #仅快速生成clang-tidy子模块
```

### 验证子模块是否有编译警告

```bash
cd build
#清理子模块
make -f ./tools/clang/lib/StaticAnalyzer/Checkers/CheckScope/Makefile clean 
#重新生成子模块并检查是否有编译警告
make -f ./tools/clang/lib/StaticAnalyzer/Checkers/CheckScope/Makefile -j8 2>&1 |grep "warn"
```

### Z3Prover

Clang可使用Z3Prover对复杂算术运算进行约束求解以减少clang默认的基于范围的约束求解器的误报。

参考：https://llvm.org/devmtg/2017-03//assets/slides/cross_translation_unit_analysis_in_clang_static_analyzer.pdf

Z3Prover github：https://github.com/Z3Prover/z3.git

Z3作为clang外部的约束求解器可以通过clang参数指定其参与静态分析的方式:

* -analyzer-constraints=z3    ----- 仅使用Z3约束求解器，禁用clang默认的范围约束求解器，分析效率会降低15-20倍
* crosscheck-with-z3=true    ----- 使用Z3约束求解器验证clang范围约束求解器检查出的BUG，降低误报，分析效率不会下降太多

以上中方式在CodeChecker中分别对应analyze参数的    --z3=on     和     --z3-refutation=on  

CodeChecker会自动检测clang是否在编译时集成了Z3Prover，如果编译clang时开启了 LLVM_ENABLE_Z3_SOLVER 才会有上面两个选项

受限于 C++17 ,我们选择了最后一个不以C++17实现的Z3Prover-4.8.10 

### 编译Z3Prover

1. 从github clong z3Prover 并切换到 z3-4.8.10   或从git@codeup.teambition.com:5dba5f29a612fe0001bac6ce/YueJing/Toolchains.git 获取源码

   ```bash
   git clone https://github.com/Z3Prover/z3.git
   cd z3
   git checkout z3-4.8.10
   ```

2. 编译生成

   ```bash
   python3 scripts/mk_make.py
   cd build
   make -j8
   ```

3. 安装

   ```bash
   make install PREFIX=/usr
   ldconfig
   ```

   DESTPREFIXDIR指定安装路径，如果你不想将Z3安装至系统目录(/usr)中,则令`PREFIX=install_path`即可,z3将会安装至 /install_path/include/    /install_path/lib/libz3.so  /install_path/bin/z3，如果是安装到系统目录中则下面的clang编译不需要指定LD_LIBRARY_PATH环境变量和LLVM_Z3_INSTALL_DIR编译选项
   
   注：要确保libz3.so被安装到了系统的默认库搜索目录中否则cmake会失败

### clang+Z3Prover 编译 (以Unix Makefiles为例)

```bash
cd Core
# git checkout your_branch
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release -DLLVM_ENABLE_PROJECTS="clang;clang-tools-extra" -DLLVM_ENABLE_Z3_SOLVER=ON -G "Unix Makefiles" ../llvm
make -j8
```

​	如果你的Z3Prover不是安装到系统目录(/usr)中，则你需要使用LD_LIBRARY_PATH指定libz3.so的路径，并使用 -DLLVM_Z3_INSTALL_DIR=Z3_install_dir指定Z3Prover的安装目录：

```bash
LD_LIBRARY_PATH=/z3_install_dir_/lib/ cmake -DCMAKE_BUILD_TYPE=Release -DLLVM_ENABLE_PROJECTS="clang;clang-tools-extra" -DLLVM_ENABLE_Z3_SOLVER=ON  -DLLVM_Z3_INSTALL_DIR=/z3_install_dir_ -G "Unix Makefiles" ../llvm
LD_LIBRARY_PATH=/z3_install_dir_/lib/ make -j8
```

## Windows+MinGW64编译

环境要求：

​	python3 (windows版 必须保证有python3.exe)

​	msys2-x86_64 

​	mingw-w64 

​	(msys2+mingw64环境搭建 https://hyzc.yuque.com/hyzc/gb82zg/rk7g3d )

​	cmake (Windows版 > 3.13.4)  安装后使用powershell运行cmake https://cmake.org/

### MinGW Makefiles（推荐）

* 启动Powershell  windows下编译需使用powershell环境

```powershell
cd Core
# git checkout your_branch
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release -DLLVM_ENABLE_PROJECTS="clang;clang-tools-extra" -G "MinGW Makefiles" ../llvm
mingw32-make.exe -j8
```



## 授权信息验证

最终给用户的发布版本需要开启授权信息验证编译选项，确保发布出去的版本都带有授权信息验证功能，防止用户调用未授权的检测能力。

可以在执行cmake命令时增加 `-DCLANG_ABILITY_AUTH=ON` 即可打开授权信息验证，一旦将其开启如果要关闭则必须手动指定`-DCLANG_ABILITY_AUTH=OFF`才可关闭，对于一个空的目录执行不带该参数的cmake默认为关闭。该参数开发人员无需理会，生成安装包时务必打开

Linux:

`cmake -DCMAKE_BUILD_TYPE=Release -DCLANG_ABILITY_AUTH=ON -DLLVM_ENABLE_PROJECTS="clang;clang-tools-extra" -DLLVM_ENABLE_Z3_SOLVER=ON -G "Unix Makefiles" ../llvm`

Windows:

`cmake -DCMAKE_BUILD_TYPE=Release -DCLANG_ABILITY_AUTH=ON -DLLVM_ENABLE_PROJECTS="clang;clang-tools-extra" -G "MinGW Makefiles" ../llvm`

* 注：发布软件包时一定要开启该选项！

