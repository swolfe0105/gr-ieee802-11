#!/usr/bin/env python2
# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: Wifi Rx
# Generated: Fri Jan 13 11:35:11 2017
##################################################

import adaptive_pursuit
import random

K = 4
Pmin = 0.1
Pmax = 1-(K-1)*Pmin
alpha = .3
beta  = .3

ap = adaptive_pursuit.adaptive_pursuit(K, Pmin, alpha, beta)
current_state = ap.current_state

for i in range(0,10000):
    if random.random() < 1.0/10.0:
      current_state = ap.run_once()
    else:
      reward = current_state+3*(random.random()-0.5)
      current_state = ap.run_once(reward)

print(ap.findOptimalOperator())
