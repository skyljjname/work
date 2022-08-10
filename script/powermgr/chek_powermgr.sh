#!/bin/bash

powermgr=`ps aux | grep "powermgr" |grep -v "grep" | wc -l`
echo $powermgr