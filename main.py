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
                    return operands[0]
                newRegex = operation
                for i in operands:
                    newRegex += '(' + preprocessing.prefixNotation(i) + ')'

            case _:
                newRegex = operation
                operand = operands[0][:len(operands[0]) - 1] # Removes the operation token to run inside it
                newRegex += '(' + preprocessing.prefixNotation(operand) + ')'

        return newRegex



regEx = input("Type your regular expression: ")

print(str(preprocessing.prefixNotation(regEx)))