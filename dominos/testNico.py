#!/usr/bin/python
from turtle import *

tracer(2, 25)
color('green')
width(5)

up()
goto(-50,-150)
down()
dot(5, "blue"); 

up()
goto(-100,-100)
down()
for i in range(4):
  forward(100)
  right(90)

up()
goto(0,-100)
down()
for i in range(4):
  forward(100)
  right(90)

up()
goto(100,-100)
down()
for i in range(4):
  forward(100)
  right(90)


raw_input()
