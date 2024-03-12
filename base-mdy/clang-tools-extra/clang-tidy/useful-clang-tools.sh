#! /bin/bash

SOURCE=$0
while [ -h "$SOURCE" ]; do
    DIR="$(cd -P "$(dirname $"$SOURCE")"&&pwd)"
    SOURCE="$(readlink $"$SOURCE")"
    [[ $SOURCE != /*  ]] && SOURCE="${DIR}/${SOURCE}"
done
#脚本文件名
SCRIPT_NAME=`basename $"$SOURCE"`
#脚本所在目录 非'/'结尾
SCRIPT_DIR="$(cd -P "$(dirname $"$SOURCE")"&&pwd)"
#脚本绝对路径
SCRIPT_PATH=${SCRIPT_DIR}/${SCRIPT_NAME}
echo $SCRIPT_DIR
CORE_PATH=`realpath $SCRIPT_DIR/../../`

case "${1}" in
    "-icheck")
        # install & check
        cd ${CORE_PATH}/build
        sudo ninja install
        python3 ${CORE_PATH}/clang-tools-extra/test/clang-tidy/check_clang_tidy.py ${CORE_PATH}/clang-tools-extra/test/clang-tidy/checkers/${2}.cpp ${2} /tmp/${2}.cpp.tmp
        ;;
    "-check")
        # check
        clang-tidy  --list-checks --checks="-*,${2}*" |grep -w ${2}
        # RESULT=`useful-clang-tools -list gjb8114 |grep -w "${2}"`
        if [ $? -ne 0 ]; then
            echo "The checker name is wrong or you need install firstly"
            exit 1
        fi
        
        python3 ${CORE_PATH}/clang-tools-extra/test/clang-tidy/check_clang_tidy.py ${CORE_PATH}/clang-tools-extra/test/clang-tidy/checkers/${2}.cpp ${2} /tmp/${2}.cpp.tmp
        ;;
    "-new")
        #new
        python3 ${CORE_PATH}/clang-tools-extra/clang-tidy/add_new_check.py ${2} ${3}
        ;;

    "-dump")
        # dump
        if [ ${3} ="" ]; then
            clang-check --ast-dump ${CORE_PATH}/clang-tools-extra/test/clang-tidy/checkers/${2}.cpp --
        else
            clang-check --ast-dump  --ast-dump-filter=${3} ${CORE_PATH}/clang-tools-extra/test/clang-tidy/checkers/${2}.cpp --
        fi
        ;;

    "-query")
        # query
        clang-query ${CORE_PATH}/clang-tools-extra/test/clang-tidy/checkers/${2}.cpp --
        ;;

    "-delete")
        # delete
        python3 ${CORE_PATH}/clang-tools-extra/clang-tidy/delete_check.py ${2} ${3}
        ;;

    "-list")
        # list
        clang-tidy  --list-checks --checks="-*,${2}*" |grep ${2}
        ;;

    "-check-clang-tools")
        #check-clang-tools
        cd ${CORE_PATH}/build
        ninja check-clang-tools
        ;;
    "-install")
        #install
        cd ${CORE_PATH}/build
        sudo ninja install
        ;;
    "-ninja")
        #build
        cd ${CORE_PATH}/build
        ninja
        ;;
    "-h"|"--help")
        echo "
Usage:
    useful-clang-tool.sh

    -icheck checker_name             e.g. gjb8114-if-elseif-with-else
    -check checker_name              e.g. gjb8114-if-elseif-with-else
    -dump checker_name filter        e.g. gjb8114-if-elseif-with-else main_3
    -query checker_name              e.g. gjb8114-if-elseif-with-else
    -new module_name checker         e.g. gjb8114 if-elseif-with-else
    -delete module_name checker      e.g. gjb8114 if-elseif-with-else
    -list module_name                e.g. gjb8114
    -check-clang-tools
    -install
    -ninja
    -h|--help
"
        ;;

    *)
        echo "Usage: useful-clang-tools.sh [-h|--help]"
        exit 1
        ;;
esac
exit 0
