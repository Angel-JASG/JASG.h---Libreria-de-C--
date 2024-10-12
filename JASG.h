#ifndef JASG_H
#define JASG_H

#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <stdexcept>

using namespace std;

// Declaraci�n del mapa de variables
extern map<string, class Variable*> variableMap;

// Clase base Variable
class Variable {
public:
    Variable() {}
    Variable(const string& val) : value(val) {}
    string getValue() const { return value; }
    virtual ~Variable() {}
    virtual string toString() const = 0;

private:
    string value;
};

// Clases derivadas para diferentes tipos de variables
class IntVariable : public Variable {
public:
    IntVariable(int v) : Variable(convertToString(v)) {}
    string toString() const { return getValue(); }

private:
    static string convertToString(int value) {
        ostringstream oss;
        oss << value;
        return oss.str();
    }
};

class StringVariable : public Variable {
public:
    StringVariable(const string& v) : Variable(v) {}
    string toString() const { return getValue(); }
};

class BoolVariable : public Variable {
public:
    BoolVariable(bool v) : Variable(v ? "true" : "false") {}
    string toString() const { return getValue(); }
};

// Mapa para almacenar las variables registradas
map<string, Variable*> variableMap;

// Funci�n para registrar una variable
void SV(const string& name, const Variable* var) {
    variableMap[name] = const_cast<Variable*>(var);
}

// Funci�n para obtener el valor de la variable
string getVariableValue(const string& name) {
    map<string, Variable*>::iterator it = variableMap.find(name); // Declaraci�n de 'it'
    if (it != variableMap.end()) {
        return it->second->getValue();
    }
    return ""; // Maneja el caso donde no se encuentra la variable
}

// Aseg�rate de liberar la memoria al finalizar
void cleanupVariables() {
    for (map<string, Variable*>::iterator it = variableMap.begin(); it != variableMap.end(); ++it) { // Declaraci�n de 'it'
        delete it->second; // Libera la memoria
    }
    variableMap.clear(); // Limpia el mapa
}



// Funci�n Art modificada para aceptar m�ltiples l�neas
void Art(int lineCount, ...) {
    va_list args;
    va_start(args, lineCount);
    
    for (int i = 0; i < lineCount; ++i) {
        const char* line = va_arg(args, const char*);
        cout << line << endl;
    }
    
    va_end(args);
}

// Funciones de sonidos y m�sica usando sistema nativo
void beepTone(int frequency, int duration) {
    #ifdef _WIN32
    Beep(frequency, duration);
    #else
    cout << "Beep no disponible en este sistema operativo." << endl;
    #endif
}

void playSound(const char* filename) {
    #ifdef _WIN32
    // Reproducir sonido simple en Windows
    string command = "start /min cmd /c start " + string(filename);
    system(command.c_str());
    #else
    cout << "Reproducci�n de sonido no disponible en este sistema operativo." << endl;
    #endif
}

// Funci�n para detectar teclas especiales
bool isSpecialKeyPressed(const string& key) {
    // Esta funci�n deber�a ser implementada seg�n la plataforma
    return false; // Placeholder
}

// Sobrecarga de la funci�n needs para teclas espec�ficas
void needs(const string& specificKey) {
    if (isSpecialKeyPressed(specificKey)) {
        cout << "Tecla " << specificKey << " presionada." << endl;
        // Opci�n para pausar aqu�
        system("pause");
    }
}

// Sobrecarga para m�ltiples teclas
void needs(const char keys[], const string& specificKey) {
    for (int i = 0; keys[i] != '\0'; ++i) {
        if (isSpecialKeyPressed(string(1, keys[i]))) {
            cout << "Tecla " << keys[i] << " presionada para " << specificKey << "." << endl;
            break;
        }
    }
}

// Funciones para evaluaci�n de expresiones matem�ticas
double getNextNumber(istringstream& stream) {
    double number;
    if (!(stream >> number)) {
        throw runtime_error("Invalid number in expression");
    }
    return number;
}

double processExpression(istringstream& stream) {
    double result = getNextNumber(stream);
    while (true) {
        char op;
        if (!(stream >> op)) break;
        double nextNumber = getNextNumber(stream);
        switch (op) {
            case '+': result += nextNumber; break;
            case '-': result -= nextNumber; break;
            case '*': result *= nextNumber; break;
            case '/':
                if (nextNumber == 0) throw runtime_error("Division by zero");
                result /= nextNumber; break;
            default: throw runtime_error("Unexpected operator in expression");
        }
    }
    return result;
}

double eval(const string& expression) {
    istringstream stream(expression);
    return processExpression(stream);
}

#endif // JASG_H

