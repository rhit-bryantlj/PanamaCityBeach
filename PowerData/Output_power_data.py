import re


outputFile = open('fig-8-pwr-68-65-9.5-laps.csv','w')

with open('fig-8-pwr-68-65-9.5-laps.txt', 'r', errors='replace') as f:
    lines = f.readlines()
    for line in lines:
        # print(line[0])
        result = re.findall(r"[-+]?\d*\.\d+|\d+", line)
        for float in result:
            outputFile.write(float)
            outputFile.write(",")
        outputFile.write("\n")
        # print(result)