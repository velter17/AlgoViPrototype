#!/usr/bin/python

import os
import argparse
import datetime


def main():
    parser = argparse.ArgumentParser(description = "AlgoVi problem visualizer class generator")
    parser.add_argument("-p", "--path", help="path for header file")
    parser.add_argument("-n", "--name", help="name of class")
    parser.add_argument("-a", "--author", default="Dmytro Sadovyi", help="author of code")
    parser.add_argument("-s", "--namespace", default="none", help="Wrap into namespace, default is none")
    args = parser.parse_args()
    header = open("CProblemViz.h.gen")
    classHeader = open(args.path + args.name + ".h", 'w')
    replaceList = [
            ["$CLASS_NAME$", args.name],
            ["$FILE_NAME$", args.name + ".h"],
            ["$AUTHOR$", args.author],
            ["$DATE$", datetime.date.today().strftime("%d.%m.%Y")]
            ]
    if args.namespace != "none":
        replaceList.append(["$NAMESPACE$", args.namespace])
    for line in header:
        for replaceItem in replaceList:
            line = line.replace(replaceItem[0], replaceItem[1])
        if args.namespace == "none" and line.startswith("$N"):
            continue
        elif line[:2] == "$N":
            line = line[2:]
        classHeader.write(line)

    source = open("CProblemViz.cpp.gen")
    if not os.path.exists(args.path + "private/"):
        os.makedirs(args.path + "private/")
    classSource = open(args.path + "private/" + args.name + ".cpp", "w")
    replaceList[1][1] = args.name + ".cpp"
    replaceList.append(["$HEADER_PATH$", "../" + args.name + ".h"])
    for line in source:
        for replaceItem in replaceList:
            line = line.replace(replaceItem[0], replaceItem[1])
        if args.namespace == "none" and line.startswith("$N"):
            continue
        elif line[:2] == "$N":
            line = line[2:]
        classSource.write(line)


if __name__ == "__main__":
    main()
