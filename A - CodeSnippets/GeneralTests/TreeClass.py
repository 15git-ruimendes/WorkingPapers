from matplotlib import pyplot
from matplotlib.widgets import Button

class Nodes :
    Name = ""
    Value = 0
    LayerNum = 0;
    InputNodes = []
    def __init__(self,InputNodes):
        self.InputNodes = InputNodes;
    
        

class And (Nodes) :
    def __init__(self, InputNodes):
        super().__init__(InputNodes)
        self.Name = " AND "
    
    def __str__(self):
        aux = ""
        for i in self.InputNodes:
            aux += str(i) + " AND "

        aux = aux[::-1]
        aux = aux.replace(" DNA ","",1)
        aux = aux[::-1]

        return "( " + aux + " )"

class Or (Nodes) :
    def __init__(self, InputNodes):
        super().__init__(InputNodes)
        self.Name = "OR"

    def __str__(self):
        aux = ""
        for i in self.InputNodes:
            aux += str(i) + " OR "

        aux = aux[::-1]
        aux = aux.replace(" RO ","",1)
        aux = aux[::-1]
        return "( " + aux + " )"

class Not (Nodes) :
    def __init__(self, InputNodes):
        super().__init__(InputNodes)
        self.Name = "NOT"
    
    def __str__(self):
        return "NOT( " + str(self.InputNodes) + " )"

class TIMER (Nodes) :
    def __init__(self, InputNodes):
        super().__init__(InputNodes)
        self.Name = "TIMER"

class SetRes (Nodes) :
    def __init__(self, InputNodes):
        super().__init__(InputNodes)
        self.Name = "SR"

    def __str__(self):
        return "SR( " + str(self.InputNodes[0]) + " , " + str(self.InputNodes[1]) + " )"

class Input (Nodes) :
    def __init__(self, Value):
        InputNodes = [];
        LayerNum = 0;
        super().__init__(InputNodes)
        self.Name = "Input"
        self.Value = Value
    
    def __str__(self):
        return str(bool(self.Value))

if __name__ == '__main__' :
    Expression = And([Or([Not(Input(1)),Input(0)]),Input(1)])
    print(Expression)