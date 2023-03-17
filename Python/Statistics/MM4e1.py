import numpy as np
from scipy.stats import norm

def find_CI(values,distribution_type,alpha = 0.05, sigma = None, bound = None, override = False):
    """
    > The function find_CI takes in a list of values, a distribution type, and a significance level,
    and returns a confidence interval for the mean of the distribution
    
    :param values: the data
    :param distribution_type: 'norm' or 't'
    :param alpha: the significance level, which is the probability of a Type I error
    :param sigma: the standard deviation of the population
    :param bound: "upper" or "lower" or None - determines side of CI. If None is chosen, CI is twosided
    :return: the lower and upper bounds of the confidence interval.
    """


    n = np.size(values)
    n_t = n
    if override == True:
        n = 1
    mu_hat = np.sum(values)/n
    
    if distribution_type == 'norm':
        dist = norm()
    elif distribution_type == 't':
        dist = t(n_t-1,0,1)
        
    if sigma == None or distribution_type == 't':
        sigma = np.sqrt(1/(n-1)*np.sum((values-mu_hat)**2))
    if bound == None:        
        z = dist.ppf(1-alpha/2)
        return (mu_hat-z*sigma/np.sqrt(n), mu_hat+z*sigma/np.sqrt(n))
    z = dist.ppf(1-alpha)
    if bound == "upper":
        return mu_hat+z*sigma/np.sqrt(n)
    if bound == "lower":
        return mu_hat-z*sigma/np.sqrt(n)

values = np.array([8.18,8.16,8.17,8.22,8.19,8.17,8.15,8.21,8.16,8.18])
a = 0.0009
h_0 = 8.20
ci10 = find_CI(values,'norm',alpha = a,sigma=0.02)
print(ci10)
sample_mean = np.mean(values)

dist = norm(loc = sample_mean, scale = 0.02/np.sqrt(10))
print(dist.interval(1-a))
if h_0 >= ci10[0] and h_0 <=ci10[1]:
    print('The hypothesis is accepted with', (1-a)*100,"% significance level")
else:
    print('The hypothesis is rejected', (1-a)*100,"% confidence")

p_val = 2*norm.cdf(-np.sqrt(10)/0.02*abs(sample_mean-h_0))
print("p-value is found as:",p_val)