class preprocessing:
    characters = []

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
        print(str(node1) + " -> " + str(node2) + '[label="' + str(weight) + '"] //' + expression) # (a|b)*abb  # ? <- Only for debbuging
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
                startNode = expressionNodes[0][0]
                nfa.connect(expression, startNode, endNode, '#')
                nfa.connect(expression, endNode, startNode, '#')
                nfa.connect(expression, expressionNodes[0][1], endNode, '#')

            case '+':
                # print("//PLUS: "+str(expressionNodes)) # ? <- Only for debbuging
                startNode = expressionNodes[0][0]
                nfa.connect(expression, endNode, startNode, '#')
                nfa.connect(expression, expressionNodes[0][1], endNode, '#')

            case '.':
                # print("//DOT: "+str(expressionNodes)) # ? <- Only for debbuging
                nfa.connect(expression, startNode, endNode, nfa.getValue(expression)) # It's a single character, uses the nodes and adds it's weight

        return [startNode, endNode] # Returns the nodes of start and end of the expression
    
    def print(regEx):
        nfa.newNode() # Generates innitial node
        coords = nfa.model(0, preprocessing.prefixNotation(regEx)) # Get's the start and ending nodes
        connections = nfa.connections[:len(nfa.connections)-1] # Erases the last node generated (it's empty)
        for i in connections: # Prints the nodes
            output = str(i[0]) + " => ["
            for j in range(len(i)):
                if(j == 0): continue
                output += "(" + str(i[j][0]) + ", " + str(i[j][1]) + ")"
                if(j < len(i) - 1): output += ", " 
            output += "]"
            print(output)
        print("Starting at: " + str(coords[0]))
        print("Ending at: " + str(coords[1]))


regEx = input("Type your regular expression: ")
nfa.print(regEx)