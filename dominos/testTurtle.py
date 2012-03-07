#!/usr/bin/python
from turtle import *

angle = 5
distance = 10
compteur = 0

while compteur <= 30:
    forward(distance)
    left(angle)
    compteur += 1
    angle += 2

color('red')
forward(200)
left(90)
color('black')
forward(50)
right(150)
color('blue')
forward(300)
up()
goto(-100,-100)
down()
color('green')
width(5)
forward(100)
raw_input()
