import random

class adaptive_pursuit:

    def __init__(self, K, Pmin, alpha, beta):
        self.K = K
        self.Pmin = Pmin
        self.Pmax = 1-(K-1)*Pmin
        self.alpha = alpha
        self.beta = beta
        self.Q = [1.0]*K
        self.P = [1.0/K]*K
        self.current_state = random.sample(range(0,K), 1)[0]

    def run_once(self, reward):

        self.Q[self.current_state] = self.Q[self.current_state] + self.alpha*(reward-self.Q[self.current_state])
        A_opt = self.findOptimalOperator()

        for i in range(0,self.K):
            if i == A_opt:
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

#K = 4
#Pmin = 0.1
#Pmax = 1-(K-1)*Pmin
#alpha = .7
#beta  = .7
#
#current_state = 0
#ap = adaptive_pursuit(K, Pmin, alpha, beta)
#
#for i in range(0,100):
#  current_state = ap.run_once(current_state)
