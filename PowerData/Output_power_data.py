import re
from xml.etree.ElementTree import tostring


outputFile = open('CompetitionOutData/oval-ccw.csv','w')

outputFile.write("Time (s),shunt V (mV),bus V ,current mA,power mW,load V \n")
timeNum = 0.5

with open('CompetitionInData/oval-ccw.txt', 'r', errors='replace') as f:
    lines = f.readlines()
    for line in lines:
        # print(line)
        result = re.findall(r"[-+]?\d*\.\d+|\d+", line)
        timeins = str(timeNum)
        outputFile.write(timeins)
        outputFile.write(",")
        for floatString in result:
            outputFile.write(floatString)
            outputFile.write(",")
        timeNum += 0.5
        outputFile.write("\n")
        # print(result)
    outputFile.write("\n")
    outputFile.write(" ,Average, Max \n")
    outputFile.write("shunt V (mV),=AVERAGE(B2:B239),=MAX(B2:B239) \n")
    outputFile.write("current mA ,=AVERAGE(D2:D239),=MAX(D2:D239) \n")
    outputFile.write("power mW ,=AVERAGE(E2:E239),=MAX(E2:E239), ,Joules,=SUM(E2:E239)/(2*1000)\n")
    
