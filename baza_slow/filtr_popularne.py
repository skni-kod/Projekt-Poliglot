#-*- coding: utf-8 -*-

import sys
import re
sys.stdout.reconfigure(encoding='utf-8')

f = open("popularne_zrodlo.txt", "r", encoding="utf-8")
newF = open("popularne.txt", "w", encoding="utf-8")

pattern = "^[a-pr-uwy-zA-PR-UWY-ZąęśżźćółńĄĘŚŻŹĆÓŁŃ]+$"

for line in f:
    line = line.split(" ")
    for rword in line:
        rword = rword.split("=")
        word = rword[0]
        if len(word) >= 3 and len(word) <= 8 and not word.isupper() and re.fullmatch(pattern, word):
            newF.write(word.lower() + "\n")

newF.close()