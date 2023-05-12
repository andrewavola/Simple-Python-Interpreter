
# loop 1
for i in range(10):
    print("For-loop step 1", i)

# loop 2
for i in range(0, 200, 50):
    print("For-loop step 50", i)

# loop 3

for a in range(2):
    for b in range(2):
        for c in range(2):
            for d in range(2):
                for e in range(1):
                    for f in range(1):
                        for g in range(1):
                            for h in range(1):
                                print( "for loop test 3 ", a, b, c, d, e, f, g )


# if test 1

i = 0
if i == 0:
    print( "test 1 -- i is 0 and it is correct." )

# if test 2

i = 5
if i == 0:
    print("test 2 -- i is 0 and it is not correct.")

else:
    print("test 2 -- else -- i is not 0 and it is correct.")


#  if else test 3
i = 6
if i == 0:
        print( "if-else -- i is 0 and it is not correct." )

elif i == 1:
        print( "if-else -- i is 1 and it is not correct." )

elif i == 2:
        print( "if-else -- i is 2 and it is not correct." )
elif i == 3:
        print( "if-else -- i is 3 and it is not correct." )
elif i == 4:
        print( "if-else -- i is 4 and it is not correct." )

elif i == 5:
        print( "if-else -- i is 5 and it is not correct." )

elif i == 6:
        print( "if-else -- i is 6 and it is correct." )

elif i == 7:
     print( "if-else -- i is 7 and it is not correct." )

elif i == 8:
        print( "if-else -- i is 8 and it is not correct." )
else:
     print( "if-else -- i < 0 or i > 8  and it is not correct." )


# if test 4

if 1 > 2:
    print( "1 else of if test 4  error" )
else:
    print( "1 else of if test 4  works" )

a = 5
b = a - 2
c = b + 4
if a > b and (a < 10 or b < c):
    print( "1 if test 4 --  works" )
else:
    print( "1 if test 4 --  error" )


# if test 5

if 4 and not 0:
    print( "if test 5 works" )
else:
    print( "if test 5 doesn't works" )

