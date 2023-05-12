# This program provide a few simple tests for the third phase of the Python Interpreter.

for p in range(2):
    print(p)

for i in range(2, 20):
     sum = 0
     for j in range(1, 10):
         sum = sum + j  # add j to the culumlative sum.
     print("The sum of the first ", i, " numbers is ", sum)


fib0 = 1   # The first Fibonacci number is either 0 or 1
fib1 = 1   # The second number is usually 1

for x in range (1, 20, 1):
     print("Fib", x, " = ", fib0)
     fib0 = fib0 + fib1    # The next Fib
     fib1 = fib0 - fib1    # The prev Fib

print("The following is the a list of the binary digits") #NOT equal to 133 in binary
for v in range(133, 0, -1):
    v = v // 2
    print(v % 2)


print("The sum of 2 and 3 is", 2 + 3)
print("The sum of 2 and 3.5 is", 2 + 3.5)
print("The sum of 2.5 and 3.5 is", 2.5 + 3.5)

numEvens = 0
numOdds1 = 0
numOdds2 = 0
for i in range(20):
    if i % 2 == 0:
        numEvens = numEvens + 1
    else:
        numOdds1 = numOdds1 + 1

    if i % 2 == 1:
       numOdds2 = numOdds2 + 1

if numOdds1 != numOdds2:
   print("Odd counters should be equal but they are not.")
else:
   print("Odd counters should be equal but they are!")

divBy2 = 0
divBy3 = 0
divBy5 = 0
divBy7 = 0
numZeros = 0
notDivBy2357 = 0
jLoopOver = 0


for i in range(10):
    for j in range(20):
      	 product = i * j
      	 if product == 0:
      	    numZeros = numZeros + 1
      	 elif product % 2 == 0:
      	      divBy2 = divBy2 + 1
      	 else:
      	      notDivBy2357 = notDivBy2357 + 1
    jLoopOver = 0
print( "divBy2 = ", divBy2 )
print( "divBy3 = ", divBy3 )
print( "divBy5 = ", divBy5 )
print( "divBy7 = ", divBy7 )
print( "numZeros = ", numZeros) 
print( "notDivBy2357 = ", notDivBy2357)
print( "jLoopOver = ", jLoopOver)

