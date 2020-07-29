#!/bin/bash

if zenity --warning --title="Warning High CPU usage" --text="Do you want to kill the process $2" --width=300 --height=100; then
    kill -9 $1
fi
