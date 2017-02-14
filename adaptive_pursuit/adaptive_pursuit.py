#!/usr/bin/env python2
# -*- coding: utf-8 -*-
##################################################
# GNU Radio Python Flow Graph
# Title: Wifi Rx
# Generated: Fri Jan 13 11:35:11 2017
##################################################

import random
import math
import time

class adaptive_pursuit:

    def __init__(self, K, Pmin, alpha, beta):
        self.K = K
        self.Pmin = Pmin
        self.Pmax = 1-(K-1)*Pmin
        self.alpha = alpha
        self.beta = beta
        self.gamma = alpha  # <-- todo
        self.SNR = [1.0]*K
        self.X   = [1.0]*K
        self.Q   = [1.0]*K
        self.P   = [1.0/K]*K
        self.current_state = random.sample(range(0,K), 1)[0]
        self.logfilename = "ap.log"

        # empty log file
        log = open(self.logfilename, 'w')
        log.truncate();
        log.close();

        self.start_time = time.time();

    def run_once(self, reward=None):

        if reward:
            self.SNR[self.current_state] = self.SNR[self.current_state] + self.alpha*(reward-self.SNR[self.current_state])
            self.X[self.current_state] = self.X[self.current_state] + self.gamma*(1-self.X[self.current_state])
        else:
            self.X[self.current_state] = self.X[self.current_state] - self.gamma*self.X[self.current_state]

        # calculate quality via SNR and X
        self.Q = [a+10*math.log10(b) for a,b in zip(self.SNR,self.X)]

        self.optimal_arm = self.findOptimalOperator()

        self.log(self.current_state, reward, self.Q[self.current_state], self.optimal_arm)

        for i in range(0,self.K):
            if i == self.optimal_arm:
                self.P[i] = self.P[i] + self.beta*(self.Pmax-self.P[i])
            else:
                self.P[i] = self.P[i] + self.beta*(self.Pmin-self.P[i])

        self.current_state = self.proportionalSelectOperator(self.P)
        return self.current_state

    def findOptimalOperator(self):
        optimal_arms = []
        Q_optimal = -1e6

        for i in range(0,self.K):
            if abs(self.Q[i] - Q_optimal) < .01:
               optimal_arms.append(i)
            elif self.Q[i] > Q_optimal:
               Q_optimal = self.Q[i];
               optimal_arms = [i]
        return random.sample(optimal_arms, 1)[0]

    def proportionalSelectOperator(self, weights):
        totals = []
        running_total = 0

        for w in weights:
            running_total += w
            totals.append(running_total)

        rnd = random.random() * running_total
        for i, total in enumerate(totals):
            if rnd < total:
                return i

    def log(self, state, reward, quality, optimal_arm):

        log = open(self.logfilename, 'a')
        # log timestamp and current state
        log.write(str('{:.10f}'.format(time.time()-self.start_time))
                      + ',' + str(state) + ',')

        # log current reward
        if reward == None:
            log.write('-999'+',')
        else:
            log.write(str('{:.10f}'.format(reward))+',')

        # log current state quality
        log.write(str(quality)+',')

        # log current optimal state
        log.write(str(optimal_arm))
        log.write('\n')

        log.close()
