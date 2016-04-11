#First and Follow
 This is a program to find out the first and follow of the variable from a given grammar


##Usage
```
g++ firstAndFollow.cpp -o firstAndFollow
./firstAndFollow
```

##Algorithm
###First
```
1. If X is a terminal then First(X) is just X!
2. If there is a Production X → ε then add ε to first(X)
3. If there is a Production X → Y1Y2..Yk then add first(Y1Y2..Yk) to first(X)
4. First(Y1Y2..Yk) is either
    4.1. First(Y1) (if First(Y1) doesn't contain ε)
    4.2. OR (if First(Y1) does contain ε) then First (Y1Y2..Yk) is everything in First(Y1) <except for ε > as well as everything in First(Y2..Yk)
    4.3. If First(Y1) First(Y2)..First(Yk) all contain ε then add ε to First(Y1Y2..Yk) as well.


```

###Follow
```
1. First put $ (the end of input marker) in Follow(S) (S is the start symbol)
2. If there is a production A → aBb, (where a can be a whole string) then everything in FIRST(b) except for ε is placed in FOLLOW(B).
3. If there is a production A → aB, then everything in FOLLOW(A) is in FOLLOW(B)
4. If there is a production A → aBb, where FIRST(b) contains ε, then everything in FOLLOW(A) is in FOLLOW(B)
```


##Sample input
```
Expression --> Tee ExpressionBar
ExpressionBar --> + Tee ExpressionBar | epsilon 
Final --> ( Expression ) | x
Tee --> Final TeeBar
TeeBar --> * Final TeeBar | epsilon
```

##Sample output
```
Expression
----------
First : (, x, 
Follow : $, ), 

ExpressionBar
-------------
First : +, epsilon, 
Follow : $, ), 

Final
-----
First : (, x, 
Follow : $, ), *, +, 

Tee
---
First : (, x, 
Follow : $, ), +, 

TeeBar
------
First : *, epsilon, 
Follow : $, ), +, 
```
