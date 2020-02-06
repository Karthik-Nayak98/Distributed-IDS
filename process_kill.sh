#!/bin/bash

if zenity --warning  --title="Warning" --text="High CPU usage. Do you want to kill this Process" --width=300 --height=100; then
    echo $1
    echo $2
fi