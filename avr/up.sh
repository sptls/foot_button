#!/bin/bash
FILE=`ls *.hex`

avrdude -pm328p -cusbasp -Uflash:w:$FILE:a -U lfuse:w:0xff:m -U hfuse:w:0xd9:m -U efuse:w:0xff:m
