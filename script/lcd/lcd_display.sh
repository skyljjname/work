#!/bin/bash

#清除内外防火墙
/root/clear_iptables.sh
/system/src/wss/pym_shell.py /root/clear_iptables.sh
#内网IP
#in_ip=(IAN1 IAN2 IAN3 IAN4 MAN)
in_ip=(IAN1 IAN2 IAN3 IAN4)
out_ip=(EAN1 EAN2 EAN3 EAN4)

#更换IP
chage_ip()
{
    num_ip=6 #默认IP：1(MAN)、2、3、4、5网段，从6开始更换不影响其他网口
    #截取
    recv_ip=$2 #$2直接传进去截取不允许，不知道为什么
    intercept=${recv_ip##*.} #截取IP地址192.168.160.128最后一个.后面的数值 128
    while [ $num_ip -le 10 ]
    do
        replace_ip=192.168.$num_ip.$intercept #替换IP
        ifconfig $1 $replace_ip netmask 255.255.255.0 #修改IP
        sleep 25 #从修改IP到液晶屏完全显示的时间
        chage_after=`ifconfig $1 |grep "inet addr" | awk '{print $2}' | tr -d "addr:"` #再次获取
        echo "修改后获取的IP""$1:"$chage_after
        let num_ip+=1 #网段自加1
    done
    num_ip=6 #默认IP：1、2、3、4、5
    #恢复原始IP
    echo "恢复原来的IP $1:$2"
    ifconfig $1 $2 netmask 255.255.255.0
}

#获取ip
get_ip()
{
    #读取文件中的值判断内外网版,获取IP、更换IP、显示IP
    for role in `cat "/etc/role"` #通过命令获取的值有‘\n’无法判断，去除后一样不能判断
    do
        echo $role
        if [ "$role" == 1 ]
        then
            #获取外网IP
            echo "外"
            for net_card_name in "${out_ip[@]}"
            do
                net_card_ip=`ifconfig $net_card_name |grep "inet addr" | awk '{print $2}' | tr -d "addr:"`
                echo "修改前的IP "$net_card_name":"$net_card_ip
                #更换外网IP
                chage_ip $net_card_name $net_card_ip
            done
        else
            #获取内网IP
            echo "内"
            for net_card_name in "${in_ip[@]}"
            do
                net_card_ip=`ifconfig $net_card_name |grep "inet addr" | awk '{print $2}' | tr -d "addr:"`
                echo "修改前的IP "$net_card_name":"$net_card_ip
                #更换内网IP
                chage_ip $net_card_name $net_card_ip
                #查看lcd显示
            done
        fi
    done
}

#给每个网卡速度
make_speed()
{
    #读取文件中的值判断内外网版,获取IP、更换IP、显示IP
    for role in `cat "/etc/role"`
    do
        echo $role
        if [ $role == 0 ]
        then
            echo “请输入外网IP 例如：IP:192.168.1.81,只需输入81即可”
            read server_ip
        else
            echo “请输入内网IP 例如：IP:192.168.1.81,只需输入81即可”
            read server_ip
        fi

        if [ "$role" == 1 ]
        then
            #获取外网IP
            /root/iperf -s &>/dev/null & #作为服务器启动
            for speed_ip_name in "${out_ip[@]}"
            do
                speed_ip=`ifconfig $speed_ip_name |grep "inet addr" | awk '{print $2}' | tr -d "addr:"`
                #循环给各网卡制造速度
                echo "${speed_ip_name}"":""${speed_ip}"
                #截取网段后连接同网段IP
                speed_vlan_intercept_1=${speed_ip#*"168."}
                speed_vlan_intercept_2=${speed_vlan_intercept_1%%.*}
                echo "开始连接内网板服务器:""192.168."$speed_vlan_intercept_2"."$server_ip
                /root/iperf -c "192.168."$speed_vlan_intercept_2"."$server_ip -t 30
            done
        else
            #获取内网IP
            /root/iperf -s &>/dev/null &
            ssh root@159.201.31.102 "/root/monitor.sh"
            for speed_ip_name in "${in_ip[@]}"
            do
                speed_ip=`ifconfig $speed_ip_name |grep "inet addr" | awk '{print $2}' | tr -d "addr:"`
                #循环给各网卡制造速度
                echo "${speed_ip_name}"":""${speed_ip}"
                #截取网段后连接同网段IP
                speed_vlan_intercept_1=${speed_ip#*"168."}
                speed_vlan_intercept_2=${speed_vlan_intercept_1%%.*}
                echo "开始连接外网板服务器:""192.168."$speed_vlan_intercept_2"."$server_ip
                /root/iperf -c "192.168."$speed_vlan_intercept_2"."$server_ip -t 30
            done
        fi
    done
}

ps aux | grep iperf | grep -v "grep"
echo $?
if [ $? != 0 ]
then
    killall -9 iperf
fi

make_speed #循环给各网卡速度
get_ip #获取网卡IP并循环修改
