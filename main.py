class preprocessing:
    characters = [] # ! NEED IMPLEMENTATION OF CHARACTERS AND VERIFY REGEX

    operators = ['|', '*', '+']

    # Given a regular expression returns the main operation done in it
    def getOperator(msg):
        parenthesis = 0
        for i in range(len(msg)): # In case it's an 'or', it must find the operator, but it cannot be inside parenthesis
            if (msg[i] == '('): parenthesis += 1
            elif (msg[i] == ')'): parenthesis -= 1
            elif (msg[i] == '|' and parenthesis == 0): return '|'
        if (msg[len(msg) - 1] == '+'): return '+'
        if (msg[len(msg) - 1] == '*'): return '*'
        return '_'

    # If the operation needed to do is an 'or', gets the index of separation
    def getSeparation(msg):
        parenthesis = 0
        for i in range(len(msg)):
            if (msg[i] == '('): parenthesis += 1
            elif (msg[i] == ')'): parenthesis -= 1
            elif (msg[i] == '|' and parenthesis == 0): return i

    # Gets the operands in a regular expression
    def getOperands(msg):
        operands = []
        i = 0
        while(i < len(msg)):
            extra = 1
            j = i
            if (msg[i] == '('): # The tokens inside a parenthesis will pass on exactly as they are
                parenthesis = 1
                j += 1
                while(parenthesis != 0):
                    if (msg[j] == '('): parenthesis += 1
                    elif (msg[j] == ')'): parenthesis -= 1
                    j += 1
                j -= 1
                extra += j - i
                
            if (j < (len(msg) - 1) and msg[j + 1] in preprocessing.operators): extra += 1
            operands.append(msg[i:i+extra])
            i += extra
        return operands
    
    # Cleans up any garbage parenthesis
    def cleanRegEx(msg):
        parenthesis = 0
        i = 0
        counter = 0
        while(i < len(msg)):
            if (msg[i] == '('): 
                parenthesis += 1
                counter += 1
            elif (msg[i] == ')'):
                parenthesis -= 1
                counter += 1
            if(parenthesis == 0 and i < len(msg) - 1): return msg # In case the parenthesis end mid loop, the parenthesis are necessary
            i += 1

        if(counter == 0): return msg
        return preprocessing.cleanRegEx(msg[1 : len(msg) - 1])
            
    # Gets the prefix notation of the regular expression
    def prefixNotation(ogRegEx):
        # Cleans the garbage parenthesis
        ogRegEx = preprocessing.cleanRegEx(ogRegEx)

        # Gets the operands in the regular expression
        operands = preprocessing.getOperands(ogRegEx)

        # Depending on the last token, checks the type of operation it is
        operation = preprocessing.getOperator(ogRegEx)

        # If it has more than one operator, it cannot be any other than 'concatenate'. Unless is an 'or'
        if(len(operands) > 1 and operation != '|'): operation = '_'

        # Depending on the operation acts differently
        match operation:
            case '|':
                separation = preprocessing.getSeparation(ogRegEx)
                firstOperand = ogRegEx[:separation] # It separates the expression in two
                secondOperand = ogRegEx[(separation + 1):]
                return operation + '(' + preprocessing.prefixNotation(firstOperand) + ')' + '(' + preprocessing.prefixNotation(secondOperand) + ')'

            case '_':
                if (len(operands) == 1): # If it's only one token, it returns as itself
                    return "." + operands[0]
                newRegex = operation
                for i in operands:
                    newRegex += '(' + preprocessing.prefixNotation(i) + ')'

            case _:
                newRegex = operation
                operand = operands[0][:len(operands[0]) - 1] # Removes the operation token to run inside it
                newRegex += '(' + preprocessing.prefixNotation(operand) + ')'

        return newRegex

class nfa:
    connections = []
    operators = ['|', '*', '+', '_', '.']

    # Generates a new node
    def newNode():
        connectTemplate = [len(nfa.connections)]
        nfa.connections.append(connectTemplate)
        return len(nfa.connections) - 1

    # Connects 2 nodes
    def connect(expression, node1, node2, weight):
        # print(str(node1) + " -> " + str(node2) + '[label="' + str(weight) + '"] //' + expression) # (a|b)*abb  # ? <- Only for debbuging
        nfa.connections[node1].append([node2, weight])

    # Given a expression, separates it based on it's operands
    def getOperands(expression):
        operands = []
        i = 0
        while(i < len(expression)):
            extra = 1
            j = i
            if (expression[i] == '('): # The tokens inside a parenthesis will pass on exactly as they are
                parenthesis = 1
                j += 1
                while(parenthesis != 0):
                    if (expression[j] == '('): parenthesis += 1
                    elif (expression[j] == ')'): parenthesis -= 1
                    j += 1
                j -= 1
                extra += j - i
                operands.append(expression[i+1:i+extra-1]) 
            i += extra # Adds to the index the length of the operand
        return operands
    
    # Gets the character with expressions with this format .X
    def getValue(expression):
        return expression[1]

    # Gets the nfa model for an expression
    def model(startNode, expression):
        currentOperation = expression[0] # Since it's in prefix notation, the first character has the expression
        expressionOperands = nfa.getOperands(expression) # Separates between components
        expressionNodes = [] # The nodes of start and end of the operands inside the expression

        for i in expressionOperands: # Runs the operands first
            travelNode = nfa.model(startNode, i) 
            startNode = nfa.newNode() # Generates a new node for the next operand
            expressionNodes.append(travelNode)

        endNode = nfa.newNode() # Generates the end node of the expression
        match currentOperation: # Depending on the operation, connects the nodes in a different way
            case '|':
                # print("//OR: "+str(expressionNodes)) # ? <- Only for debbuging
                nfa.connect(expression, startNode, expressionNodes[0][0], '#')
                nfa.connect(expression, startNode, expressionNodes[1][0], '#')
                endNode = nfa.newNode()
                nfa.connect(expression, expressionNodes[0][1], endNode, '#')
                nfa.connect(expression, expressionNodes[1][1], endNode, '#')

            case '_':
                # print("//CONCAT: "+str(expressionNodes)) # ? <- Only for debbuging
                i = 0
                startNode = expressionNodes[0][0]
                endNode = expressionNodes[len(expressionNodes)-1][1]
                while (i < len(expressionNodes) - 1): # Connects all the nodes that will concatenate
                    nfa.connect(expression, expressionNodes[i][1], expressionNodes[i+1][0], '#')
                    i += 1

            case '*':
                # print("//STAR: "+str(expressionNodes)) # ? <- Only for debbuging
                midNode = expressionNodes[0][0]
                nfa.connect(expression, startNode, midNode, '#')
                nfa.connect(expression, startNode, endNode, '#')
                nfa.connect(expression, expressionNodes[0][1], midNode, '#')
                nfa.connect(expression, expressionNodes[0][1], endNode, '#')

            case '+':
                # print("//PLUS: "+str(expressionNodes)) # ? <- Only for debbuging
                midNode = expressionNodes[0][0]
                nfa.connect(expression, startNode, midNode, '#')
                nfa.connect(expression, expressionNodes[0][1], midNode, '#')
                nfa.connect(expression, expressionNodes[0][1], endNode, '#')

            case '.':
                # print("//DOT: "+str(expressionNodes)) # ? <- Only for debbuging
                nfa.connect(expression, startNode, endNode, nfa.getValue(expression)) # It's a single character, uses the nodes and adds it's weight

        return [startNode, endNode] # Returns the nodes of start and end of the expression
    
    def print(regEx, characters):
        nfa.newNode() # Generates innitial node
        coords = nfa.model(0, preprocessing.prefixNotation(regEx)) # Get's the start and ending nodes
        connections = nfa.connections[:len(nfa.connections)-1] # Erases the last node generated (it's empty)
        print("----RESULTS----\nINPUT:\n" + regEx + "\n\nNFA:")
        for i in connections: # Prints the nodes
            if(len(i) == 1): continue
            output = str(i[0]) + " => ["
            for j in range(len(i)):
                if(j == 0): continue
                output += "('" + str(i[j][0]) + "', '" + str(i[j][1]) + "')"
                if(j < len(i) - 1): output += ", " 
            output += "]"
            print(output)
        print("Accepting state: " + str(coords[1]) + "\n")
        return [connections, coords[0], coords[1], characters]


class dfa:
    connections = []
    endNode = 0
    startNode = 0
    states = [] # Holds the elements of a state
    statesConnections = [] # Holds the connections between states
    acceptingStates = []

    characters = []

    def getEpsilonConnections(index, visited, queue):
        visited.append(index)
        queue.append(index)

        for i in dfa.connections[index][1:]:
            if(i[1] == '#'):
                if(i[0] not in visited):
                    dfa.getEpsilonConnections(i[0], visited, queue)

        return list(set(queue))
    
    def getConnections(index, weight):
        fullArray = []
        for i in dfa.connections[index][1:]:
            if(i[1] == weight): fullArray.append(i[0])
        return list(set(fullArray))
    
    def eClosure(indexArray):
        fullArray = []
        isEnd = False
        for i in indexArray:
            fullArray.extend(dfa.getEpsilonConnections(i, [], []))
        fullArray = list(set(fullArray)) # removes repeated elements and sorts it

        if(dfa.endNode in fullArray): isEnd = True
        
        i = 0 # Verifies if the current state has already been created
        while(i < len(dfa.states)):
            if(sorted(dfa.states[i]) == sorted(fullArray)):
                if(isEnd): dfa.acceptingStates.append(i)
                return i
            i += 1
        
        # In case the state doesn't exist, it will create another one
        dfa.states.append(fullArray)
        dfa.statesConnections.append([len(dfa.statesConnections)])
        if(isEnd): dfa.acceptingStates.append(i)
        return i
    
    def move(startState, weight):
        fullArray = []
        for i in dfa.states[startState]: # Repeats for all the elements of the state
            fullArray.extend(dfa.getConnections(i, weight))
        fullArray = list(set(fullArray)) # removes repeated elements and sorts it

        return fullArray

    def doState(state):
        for i in dfa.characters:
            nodes = dfa.move(state, i)

            if(len(nodes) == 0): continue # Since it has no elements, it cannot connect

            endState = dfa.eClosure(nodes)
            dfa.statesConnections[state].append([endState, i])

    def getChar(number):
        number = int(number)
        state = ""
        while(number >= 0):
            state = chr(number % 26 + ord('A')) + state
            number //= 26
            number -= 1
        return state

    def model(auxData):
        dfa.connections = auxData[0]
        dfa.startNode = auxData[1]
        dfa.endNode = auxData[2]
        dfa.characters = auxData[3]

        dfa.eClosure([dfa.startNode])

        i = 0
        j = 1
        while(i < j):
            dfa.doState(i)
            i += 1
            j = len(dfa.states)

        print("DFA:")

        for i in dfa.statesConnections:
            output = dfa.getChar(i[0]) + " => ["
            for j in range(len(i)):
                if(j == 0): continue
                output += "('" + dfa.getChar(i[j][0]) + "', '" + i[j][1] + "')"
                if(j < len(i) - 1): output += ", " 
            output += "]"
            print(output)

        accepted = list(set(dfa.acceptingStates))
        output = "Accepting states: ["
        for i in range(len(accepted)):
            output += "'" + dfa.getChar(accepted[i]) + "'"
            if(i < len(accepted) - 1): output += ", " 
        output += "]"
        print(output)

        

characters = list(input("Enter the characters in your regular expression: "))

regEx = input("Type your regular expression: ")

print(preprocessing.prefixNotation(regEx))
auxData = nfa.print(regEx, characters)

dfa.model(auxData)

#print(preprocessing.prefixNotation("(a|b)*abb"))
#print(preprocessing.prefixNotation("a+b((abc)|a+)*"))