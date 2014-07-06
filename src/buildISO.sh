#!/bin/bash

scp -r isodir slayter@gnar.io:~/Code/OSLearning/
ssh slayter@gnar.io 'cd ~/Code/OSLearning; grub-mkrescue -o myos.iso isodir'
scp -r slayter@gnar.io:~/Code/OSLearning/myos.iso myos.iso