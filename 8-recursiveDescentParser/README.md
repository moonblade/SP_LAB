#Recursive Descent Parser
Recursive descent is a top-down parsing technique that constructs the parse tree from the top and the input is read from left to right. It uses procedures for every terminal and non-terminal entity. This parsing technique recursively parses the input to make a parse tree, which may or may not require back-tracking. But the grammar associated with it (if not left factored) cannot avoid back-tracking. A form of recursive-descent parsing that does not require any back-tracking is known as predictive parsing.

This parsing technique is regarded recursive as it uses context-free grammar which is recursive in nature.

##Algorithm
```
create function mimicking the productions
use functions for non terminals
check input symbol for terminals, advance if same and error if not
```

##Usage
```
g++ recursiveDescentParser.cpp -o recursiveDescentParser
./recursiveDescentParser
```

##Sample input
```
x+x
xx*(x+x)
x*x+(x+(x+x*x))
((((x))))
```

##Sample Output
```
Accepted : x+x
Rejected : xx*(x+x)
Accepted : x*x+(x+(x+x*x))
Accepted : ((((x))))
```

