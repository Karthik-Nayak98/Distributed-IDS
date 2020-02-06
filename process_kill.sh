#!/bin/bash


if zenity --question  --title="Warning" --text="High CPU usage. Do you want to kill this Process"; then
    echo $1
    echo $2
    #zenity --text="you press YES!"
fi