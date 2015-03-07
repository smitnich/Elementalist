import sys
import os

tileStr = "tile id=\""
endTag = "</tileset>"
replaceStr = "C:\\source\\elementalist\\Output\\"
emptyName = "gfx/iamerror.png"
fileName = "../newTemplate.tmx"
objCount = 1000

def makeStr(i):
    return "  <tile id=\"%d\">\r\n   <image width=\"48\" height=\"48\" source=\"gfx/iamerror.png\"/>\r\n  </tile>\r\n" % i;

def addDummyObjects(fileName):
    output = ""
    f = open(fileName,'r')
    for line in f.readlines():
        start = line.find(tileStr)
        end = line.rfind("\"")
        if (start != -1):
            start += len(tileStr)
            count = int(line[start:end])
        elif (line.find(endTag) != -1):
            for i in range(count+1,objCount):
                output += makeStr(i)
        output += line

    out = open("newTemplate.tmx","w")
    out.write(output)
    out.close()
    f.close()

def addObjects(imageName,tagName):
    findLine = True
    findTag = True
    f = open(fileName,'r')
    fout = open("../tmpFile.txt","w")
    for line in f.readlines():
        if (findTag and line.find(tagName) != -1):
            findTag = False
        elif (not findTag and findLine and line.find(emptyName) != -1):
            line = line.replace(emptyName,imageName.replace("\\","/"))
            findLine = False
        fout.write(line)
    f.close()
    fout.close()
    os.remove("../newTemplate.tmx");
    os.rename("../tmpFile.txt","../newTemplate.tmx");

addObjects(sys.argv[1].replace(replaceStr,""),sys.argv[2])
