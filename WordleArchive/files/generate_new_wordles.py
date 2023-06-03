import os
import random
import requests
import time

from tqdm import tqdm

def get_words(startswith):
    response = None
    timeout = 10        # timeout after 10 seconds
    sleep_time = 0.5
    for _ in range(round(timeout / sleep_time)):
        try:
            response = requests.get('https://api.datamuse.com/words?sp='+startswith+'????&md=f&max=1000').json()
            break
        except:
            time.sleep(sleep_time)
            continue
    if len(response) == 0: return []
    else:
        return [(w["word"].lower(), float(w["tags"][0][2:])) for w in response \
            if len(w["word"]) == 5 and all(c in "abcdefghijklmnopqrstuvwxyz" for c in w["word"].lower())]

dictionary = []
for c in "abcdefghijklmnopqrstuvwxyz":
    dictionary += get_words(c)

print(f"Got {len(dictionary)} words")

with open("wordles.txt", "rt") as wordle_file:
    wordles = [line.strip().split(' ')[-1].lower() for line in wordle_file.readlines()]
    wordles = set(word for word in wordles if len(word) == 5)
    
dictionary = [word for word in dictionary if word[0] not in wordles]
del wordles

dictionary.sort(key=lambda dict_entry: dict_entry[1], reverse=True)

dictionary = dictionary[:3000]#[i for i in dictionary if i[1] > 0.25]
print(f"Output list size: {len(dictionary)} words")
random.shuffle(dictionary)
with open("new_word_list.txt", "wt") as new_word_list:
    for word in dictionary: print(word[0], file=new_word_list)
