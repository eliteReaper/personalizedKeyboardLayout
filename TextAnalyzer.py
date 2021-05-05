from collections import defaultdict
import re

def getCharFreqFromLines(lines):
    char_freq = defaultdict(int)
    for line in lines:
        for char in line:
            char_freq[char] += 1
    return char_freq

def getCharPairFreqFromLines(lines):
    char_pair_freq = defaultdict(int)
    for line in lines:
        sz = len(line)
        for idx, char in enumerate(line):
            if idx == sz - 1: 
                break
            char1 = line[idx]
            char2 = line[idx + 1]
            if char1 == ' ' or char1 == '\n' or char1 == '\t' or char2 == ' ' or char2 == '\n' or char2 == '\t':
                continue
            char_pair_freq[(line[idx], line[idx + 1])] += 1
    return char_pair_freq

# def getWordFrequencyFromLines(lines):
#     word_freq = defaultdict(int)
#     for line in lines:
#         for word in re.split(',| |(|)|\n|\t|;|', line):
#             word_freq[word] += 1
#     return word_freq

def getSortedListByFreqFromDict(dictionary):
    ls = []
    total = 0
    for key, value in dictionary.items():
        total += value
    for key, value in dictionary.items():
        ls.append((key, value, (value / total) * 100))
    ls.sort(key=lambda a : a[1], reverse=True)
    return ls

no_of_templated_solutions = 15   # Close to 19 actually

full_charset = ['1', '!', 'q','Q', 'a', 'A', 'z', 'Z', '2', '@', 'w', 'W', 's', 'S', 'x', 'X', '3', '#', 'e', 'E', 'd', 'D', 'c', 'C', '4', '$', 'r', 'R', 'f', 'F', 'v', 'V', '5', '%', 't', 'T', 'g', 'G', 'b', 'B', '6', '^','y', 'Y', 'h', 'H', 'n', 'N', '7', '&', 'u', 'U', 'j', 'J', 'm', 'M', '8', '*', 'i', 'I', 'k', 'K', ',', '<', '9', '(', 'o', 'O', 'l', 'L', '.', '>', '0', ')', 'p', 'P',';', ':','/', '?','-', '_','[', '{', '\'', '"', '=', '+', ']','}']

with open("./cf_scrapper/partha_paul.txt") as rd:
    lines = rd.readlines()
    char_freq = getCharFreqFromLines(lines)
    # word_freq = getWordFrequencyFromLines(lines)
    
    template_lines = []
    with open("./cf_scrapper/cpp_template.txt") as rd_template:
        template_lines = rd_template.readlines()
    
    char_freq_template = getCharFreqFromLines(template_lines)

    for key, value in char_freq_template.items():
        char_freq[key] = max(0, char_freq[key] - no_of_templated_solutions * value)

    char_freq.pop('\n') 
    char_freq.pop(' ') 
    char_freq.pop('\\')
    char_freq.pop('|')
    ls = getSortedListByFreqFromDict(char_freq)

    total = 0
    # for key, value in char_freq.items():
    #     total += value
    # for char in full_charset:
    #     print("{:.5f}".format(char_freq[char] / total * 100), end=', ')

    char_pair_freq = getCharPairFreqFromLines(lines)
    lscp = getSortedListByFreqFromDict(char_pair_freq)
    for e in lscp:
        print(e)
    # for key, value in char_pair_freq.items():
    #     total += value
    
    # for c1 in full_charset:
    #     for c2 in full_charset:
    #         print("{:.5f}".format(char_pair_freq[(c1, c2)] / total * 100), end=', ')