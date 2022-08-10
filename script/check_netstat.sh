#!/bin/bash

#处理特殊进程proxy
process_proxy()
{
    #配置文件中获取代理数量
    agent_num=2
    if [ "$1" -lt "$agent_num" ]
    then
        #后台进程小于数据库数量则为异常，重启proxy
        : #占位符
        #echo "proxy abnormal"
    else
        echo "proxy normal"
    fi
}

process_vsftpd()
{
     #查看vsftpd服务版本
    vsftpd_version=`/system/mrgcp/vsftpd -v`
    cho "客户端vsftpd的版本是:" $vsftpd_version
    #查看vsftpd配置文件中客户端连接的最大数量：
    vsftpd_max_client=`grep -r "max_clients" /system/mrgcp/vsftpd.conf`
    echo "客户端vsftpd允许的最大连接数是:" $vsftpd_max_client
}

#查询端口，需要输入进程名或端口号
query_port()
{
    echo "输入查询的进程名或端口号"
    read port
    echo "1" | sudo -S netstat -tunpl | grep "$port" # -S表示将echo输出的标准流读取，作为输入密码
}

#查询进程是否存在
query_process()
{
    echo "输入查询的进程名或服务名"
    read  process_name
    #echo $process_name
    #ps aux | grep "$process_name" | grep -v "grep"
    process_num=`ps aux | grep "$process_name" | grep -v "grep"| wc -l`
    if [ "$process_num" -le 0 ]
    then
        echo $process_name"的数量为"$process_num
        echo "no process, exec abnormal, do something"
    else
        echo $process_name"的数量为"$process_num
        echo "exec result normal,do something"
        #记录时间和服务名，重启进程或服务
    fi

    if [ "$process_name" == "proxy" ]
    then
        query_port
        process_proxy $process_num
    fi

    if [ "$process_name" == "vsftpd" ]
    then
        process_vsftpd
    fi
}



query_mount()
{
    echo "请输入关键字(目录名或IP):"
    read keyword
    mount -l | grep "$keyword"
    #查看目录是否挂载
    #未挂载手动挂载查看是否是挂载错误
    #手动挂载成功查看python脚本执行挂载命令是否失败
    :
}

function_menu()
{
    echo '---------1、查询端口---------'
    echo '---------2、查询进程---------'
    echo '---------3、查询挂载---------'
    echo '---------4、查询菜单---------'
    echo '---------5、  退出  ---------'
}

function_menu

while [ true ]
do
    echo '请输入你的选择'
    read i
   case "${i}" in
    1)
        echo "item = 1"
        query_port #查询端口
        ;;
    2)
        echo "item = 2 or item = 3"
        #query_process #查询进程
        ;;
    3)
        echo "item = 2 or item = 3"
        query_mount
        ;;
    4)
        function_menu
        ;;
    5)
        echo "End of query,goodbye"
        break
        ;;
    *)
        echo "输入错误，请重新输入"
        continue
        ;;
   esac
   
done

