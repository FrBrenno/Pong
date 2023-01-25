#! /bin/bash

TMP_FILE="/tmp/ECS_Modification"

if [ -f $TMP_FILE ]; then
    NB=$(cat $TMP_FILE | uniq | wc -l)
    if [ $NB -gt 0 ]; then
        echo "Component files has been changed => Recompiling everything..."
        rm $TMP_FILE
        make build
    else
        echo "No component file has been changed."
        make main
    fi
else
    inotifywait -m -e modify ./ECS/* > $TMP_FILE &
    NB_WATCHERS=$(pidof inotifywait | wc -w)

    if [ $NB_WATCHERS -gt 1 ]; then
        echo "Too many watchers, killing some..."
        kill $(pidof inotifywait)
        NB_WATCHERS=0
    fi

    if [ $NB_WATCHERS -eq 0 ]; then
        echo "Creating a single watcher..."
        inotifywait -m -e modify ./ECS/* > $TMP_FILE &
    fi
fi