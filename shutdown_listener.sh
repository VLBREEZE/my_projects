#!/bin/bash

# 아두이노 신호를 기다리기 위해 /dev/ttyACM0 포트(아두이노가 연결된 포트)를 모니터링
while true; do
    if sudo cat /dev/ttyACM0 | grep "SHUTDOWN"; then
        sudo shutdown -h now
        break
    fi
    sleep 1
done
