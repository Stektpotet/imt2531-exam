#!/usr/local/bin/python3
# -*- coding:utf-8 -*-

import argparse
from subprocess import call
import os
import platform
import sys
import random
import math

def cameras(file):
    file.write("""
cameras: 1

camera: mainCam
  position: 0 0 -20
  rotation: 0 0 0
  velocity: 0 0 0
  angleVelocity: 0 0 0
  mode: orbital
  fov: 90
  aspectRatio: 1.25
  nearClip: 0.1
  farClip: 500

""")

def cube(file, model, count, spacing):

    for i in range(count):
        for j in range(count):
            for k in range(count):

                file.write("""

    entity: cube{}
    model: {}
    position: {} {} {}
    rotation: 4 0 90
    scale: {} 1 1
    velocity: 0 0 0
    angleVelocity: {} {} {}

""".format( (i*count**2)+(j*count)+(k),
            model,
            i*spacing,
            j*spacing,
            k*spacing,
            random.randint(1,5),
            random.randint(1,5),
            random.randint(1,100),
            random.randint(1,100),
            random.randint(1,100)))

def line(file, model, count, spacing):
    for i in range(count):

        file.write("""

    entity: {}
    model: {}
    position: {} 0 0
    rotation: 4 0 90
    scale: 2 2 2
    velocity: 0 0 0
    angleVelocity: {} {} {}

""".format(i,
        model,
        i*spacing,
        random.randint(1,100),
        random.randint(1,100),
        random.randint(1,100)))

def circle(file, model, count, spacing):
    TAU = 6.19
    step = TAU / count

    for i in range(count):

        file.write("""

    entity: circle{}
    model: {}
    position: {} {} 0
    rotation: 4 0 90
    scale: 2 2 2
    velocity: 0 0 0
    angleVelocity: 0 0.5 0

    """.format(i,
               model,
               math.sin(step * i)*spacing,
               math.cos(step * i)*spacing))

def entities(file, model, count, spacing, argv):
    if argv.primitive == "cube":
        file.write("entities: "+str(count**3))
        cube(file, model,count, spacing)

    elif argv.primitive == "line":
        file.write("entities: "+str(count))
        line(file, model,count, spacing)

    elif argv.primitive == "circle":
        file.write("entities: "+str(count))
        circle(file, model,count,spacing)

    else:
        print("primitive not supported")


def relations(file, count = 0):

    file.write("relations: "+ str(count)+"\n")

if __name__ == "__main__":

    parser = argparse.ArgumentParser()
    parser.add_argument("model",     help="cube|Suzanne|Icosphere")
    parser.add_argument("primitive", help="cube|line|circle")
    parser.add_argument("count",     type=int)
    parser.add_argument("spacing",   type=int)
    argv = parser.parse_args()

    buildfile = "./build/assets/scenes/demojonas.yml"

    count = int(argv.count)
    spacing = int(argv.spacing)
    model = argv.model


    with open(buildfile, "w") as file:
        cameras(file)
        entities(file, model, count, spacing, argv)

        relations(file)