import numpy as np
import random
PRIME_LIST = [85751, 66173, 82651, 376583, 796181, 620929, 4597459, 5717021, 9739817, 95752733, 88254581, 365496587, 941039989]
def isPrime(n):
  if n == 2 or n == 3: return True
  if n < 2 or n%2 == 0: return False
  if n < 9: return True
  if n%3 == 0: return False
  r = int(n**0.5)
  # since all primes > 3 are of the form 6n ± 1
  # start with f=5 (which is prime)
  # and test f, f+2 for being prime
  # then loop by 6.
  f = 5
  while f <= r:
    #print('\t',f)
    if n % f == 0: return False
    if n % (f+2) == 0: return False
    f += 6
  return True

primes = [i for i in range(0,100) if isPrime(i)]

big_primes = [i for i in range(0,1000) if isPrime(i)]

for N in PRIME_LIST:
    a = random.choice(primes)
    b = random.choice(big_primes)
    c = random.choice(big_primes)
    points = np.zeros((N, 2), dtype=int)
    for i in range(N):
        points[i, 0] = i
        points[i, 1] = (a*i*i + b*i + c) % N
    filename = "test_"+str(N)+".txt"
    with open(filename , 'wb') as f:
        np.savetxt(f, points, delimiter=' ', newline='\n', header='', footer='', comments='# ', fmt='%d')
