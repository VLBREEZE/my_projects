import serial
import os

def main():
    ser = serial.Serial('/dev/ttyACM0', 115200) # 젯슨 나노의 시리얼 포트 설정
    while True:
        if ser.in_waiting > 0:
            line = ser.readline().decode('utf-8').strip()
            if line == "SHUTDOWN":
                os.system("sudo shutdown now")

if __name__ == "__main__":
    main()
