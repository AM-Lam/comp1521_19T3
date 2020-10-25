#!/bin/sh

for i in 0 1 2 3 4 5 6 7 
do
    for j in 0 1 2 3 4 5 6 7
    do
        for k in 0 1 2 3 4 5 6 7
        do
            for l in 0 1 2 3 4 5 6 7
            do
                for m in 0 1 2 3 4 5 6 7
                do
                    for n in 0 1 2 3 4 5 6 7
                    do
                        for o in 0 1 2 3 4 5 6 7
                        do
                            for p in 0 1 2 3 4 5 6 7
                            do
                                echo "1521 emu -p" $i$j$k$l$m$n$o$p
                            done
                        done
                    done
                done
            done
        done
    done
done