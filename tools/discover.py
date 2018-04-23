import sys
import glob
import os
import time


def post(tag, extension):
    print(tag, extension)
    sys.stdout.flush()


def scan_collection(collection_match):

    filenames = glob.glob("../assets/" + collection_match)
    for fname in filenames:

        fname = os.path.basename(fname)
        if "." not in fname:
            continue

        fname = os.path.basename(fname)
        post(tag        = fname.split(".")[0],
             extension  = fname.split(".")[1])


if __name__ == "__main__":

    running = True
    while running:
        #collection_match = input("Query: ")
        collection_match = sys.stdin.readline()

        if collection_match == "EXIT":
            break
        
        scan_collection(collection_match);

    print("EXITING!")
