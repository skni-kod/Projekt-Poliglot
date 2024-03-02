#-*- coding: utf-8 -*-

import sys
import re
sys.stdout.reconfigure(encoding='utf-8')

f = open("baza-wszystkie.txt", "r", encoding="utf-8")
newF = open("wszystkie.txt", "w", encoding="utf-8")

for line in f:
    line = line.split(", ")
    word = line[0]
    #ok = re.search("^[a-zA-Ząęśżźćółń]$", word)
    pattern = "^[a-pr-uwy-zA-PR-UWY-ZąęśżźćółńĄĘŚŻŹĆÓŁŃ]+$"
    #if len(word) >= 3 and len(word) <= 8 and not word.isupper() and word.find(" ") == -1 and word.find(".") == -1 and word.find("v") == -1 and word.find("x") == -1:
    #    newF.write(word + "\n")

    if len(word) >= 3 and len(word) <= 8 and not word.isupper() and re.fullmatch(pattern, word) and word[0].islower():
        newF.write(word.lower() + "\n")

newF.close()
