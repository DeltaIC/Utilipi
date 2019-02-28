#!/usr/bin/env python

import RPi.GPIO as GPIO

GPIO.setmode(GPIO.BCM)
GPIO.setup(4, GPIO.OUT)     #T1
GPIO.setup(17, GPIO.OUT)    #T2
GPIO.setup(27, GPIO.OUT)    #T3
GPIO.setup(22, GPIO.OUT)    #T4
GPIO.setup(5, GPIO.OUT)     #T5
GPIO.setup(6, GPIO.OUT)     #T6
GPIO.setup(13, GPIO.OUT)    #T7
GPIO.setup(19, GPIO.OUT)    #T8

#on met la sortie numerique a l'etat 1
GPIO.output(4, GPIO.HIGH)
GPIO.output(17, GPIO.HIGH)
GPIO.output(27, GPIO.HIGH)
GPIO.output(22, GPIO.HIGH)
GPIO.output(5, GPIO.HIGH)
GPIO.output(6, GPIO.HIGH)
GPIO.output(13, GPIO.HIGH)
GPIO.output(19, GPIO.HIGH)

# une touche pour quitter le programme.  
input('Appuyez sur une touche pour stopper') 
# La fonction input est bloquante. Si on arrive ici alors on peut fermer le programme
p.stop()
GPIO.cleanup()
