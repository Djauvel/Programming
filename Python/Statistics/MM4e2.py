import numpy as np
from scipy.stats import norm

values = np.array([2.82,2.72,3.22,2.67])

sigma = 1
sample_mean = np.mean(values)
n = np.size(values)

dist = norm(loc = sample_mean, scale = sigma/np.sqrt(n))
mu = 2
a = 0.1
true_dist = norm(loc = mu, scale = sigma/np.sqrt(n))
standard_norm = norm()
c90=dist.interval(1-a)
print(c90)

if c90[0] <= mu <= c90[1]:
    print("H0 accepted")
else:
    print("H0 Rejected")

beta = standard_norm.cdf((sample_mean-mu)*np.sqrt(n)/sigma+ standard_norm.ppf(1-a/2)) - standard_norm.cdf((sample_mean-mu)*np.sqrt(n)/sigma- standard_norm.ppf(1-a/2))
print("beta:",beta)
