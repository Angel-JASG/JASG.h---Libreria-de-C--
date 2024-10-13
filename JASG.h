#ifndef JASG_H
#define JASG_H

#include <iostream>
#include <string>
#include <sstream>
#include <map>
#include <stdexcept>
#include <vector>
#include <cstdarg>
#include <windows.h>
#include <conio.h>
#include <ostream>

using namespace std;

// Declaración del mapa de variables
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

// Función para registrar una variable
void SV(const string& name, const Variable* var) {
    variableMap[name] = const_cast<Variable*>(var);
}

// Función para obtener el valor de la variable
string getVariableValue(const string& name) {
    map<string, Variable*>::iterator it = variableMap.find(name); // Declaración de 'it'
    if (it != variableMap.end()) {
        return it->second->getValue();
    }
    return ""; // Maneja el caso donde no se encuentra la variable
}

// Asegúrate de liberar la memoria al finalizar
void cleanupVariables() {
    for (map<string, Variable*>::iterator it = variableMap.begin(); it != variableMap.end(); ++it) { // Declaración de 'it'
        delete it->second; // Libera la memoria
    }
    variableMap.clear(); // Limpia el mapa
}



// Función Art modificada para aceptar múltiples líneas
void Art(int lineCount, ...) {
    va_list args;
    va_start(args, lineCount);
    
    for (int i = 0; i < lineCount; ++i) {
        const char* line = va_arg(args, const char*);
        cout << line << endl;
    }
    
    va_end(args);
}

// Funciones de sonidos y música usando sistema nativo
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
    cout << "Reproducción de sonido no disponible en este sistema operativo." << endl;
    #endif
}

// Función para detectar teclas especiales
bool isSpecialKeyPressed(const string& key) {
    if (_kbhit()) { // Verifica si hay una tecla presionada
        char ch = _getch(); // Obtiene la tecla presionada
        // Comprobar teclas específicas
        if (key == "space" && ch == ' ') return true; // Tecla espacio
        if (key == "enter" && ch == 13) return true; // Tecla enter
        if (key == "esc" && ch == 27) return true; // Tecla escape
        if (key == "a" && ch == 'a') return true; // Tecla A
        if (key == "b" && ch == 'b') return true; // Tecla B
        if (key == "c" && ch == 'c') return true; // Tecla C
        if (key == "up" && ch == 72) return true; // Tecla flecha arriba
        if (key == "down" && ch == 80) return true; // Tecla flecha abajo
        if (key == "left" && ch == 75) return true; // Tecla flecha izquierda
        if (key == "right" && ch == 77) return true; // Tecla flecha derecha
    }
    return false; // No se detectó la tecla
}

// Función needs que espera a que se presione una tecla específica
void needs(const string& specificKey) {
    
    while (true) {
        if (isSpecialKeyPressed(specificKey)) {
            cout << "Tecla " << specificKey << " presionada." << endl;
            break; // Sale del bucle al detectar la tecla
        }
    }
}

// Sobrecarga para múltiples teclas
void needs(const char keys[], const string& specificKey) {
    cout << "Esperando que se presione una de las teclas: " << specificKey << "..." << endl;
    while (true) {
        for (int i = 0; keys[i] != '\0'; ++i) {
            if (isSpecialKeyPressed(string(1, keys[i]))) {
                cout << "Tecla " << keys[i] << " presionada para " << specificKey << "." << endl;
                return; // Sale de la función al detectar una tecla
            }
        }
    }
}

// Funciones para evaluación de expresiones matemáticas
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

// Función para centrar el texto y devolverlo como string
string c(const string& text) {
    // Obtener el ancho de la consola
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    int consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;

    // Asegurarse de que el texto no sea más ancho que la consola
    if (text.length() >= static_cast<size_t>(consoleWidth)) {
        return text; // Devolver el texto original si es más largo
    }

    // Calcular el espacio necesario para centrar
    int padding = (consoleWidth - static_cast<int>(text.length())) / 2;

    // Crear la cadena centrada
    string centeredText = string(max(padding, 0), ' ') + text; // Evitar negativos
    return centeredText; // Devolver la cadena centrada
}

// Sobrecarga para manejar una variable
string c(const string& text, const Variable& var) {
    return c(text + " " + var.toString());
}

// Sobrecarga para manejar múltiples variables
string c(const string& text, const vector<Variable*>& vars) {
    string fullText = text;
    for (size_t i = 0; i < vars.size(); ++i) {
        fullText += " " + vars[i]->toString();
    }
    return c(fullText);
}

// Colores de fondo y texto
enum Color {
    BLACK = 0,
    BLUE = 1,
    GREEN = 2,
    CYAN = 3,
    RED = 4,
    MAGENTA = 5,
    BROWN = 6,
    LIGHT_GRAY = 7,
    DARK_GRAY = 8,
    LIGHT_BLUE = 9,
    LIGHT_GREEN = 10,
    LIGHT_CYAN = 11,
    LIGHT_RED = 12,
    LIGHT_MAGENTA = 13,
    LIGHT_BROWN = 14,
    WHITE = 15,
};

// Función para cambiar el tamaño, el color de fondo y el color de texto de la ventana
void Window(const std::string& size, const std::string& backgroundColor = "black", const std::string& textColor = "white") {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    
    // Configurar el color de fondo
    int bgColor;
    if (backgroundColor == "black") bgColor = 0;
    else if (backgroundColor == "blue") bgColor = 1;
    else if (backgroundColor == "green") bgColor = 2;
    else if (backgroundColor == "red") bgColor = 4;
    else if (backgroundColor == "purple") bgColor = 5;
    else if (backgroundColor == "yellow") bgColor = 6;
    else if (backgroundColor == "lightGray") bgColor = 7;
    else bgColor = 0; // default to black

    // Configurar el color de texto
    int txtColor;
    if (textColor == "white") txtColor = 15;
    else if (textColor == "black") txtColor = 0;
    else if (textColor == "green") txtColor = 10;
    else if (textColor == "red") txtColor = 12;
    else if (textColor == "yellow") txtColor = 14;
    else txtColor = 15; // default to white

    // Cambiar el color
    SetConsoleTextAttribute(hConsole, bgColor << 4 | txtColor);

    // Cambiar el tamaño de la ventana
    if (size == "full") {
        HWND hwnd = GetConsoleWindow();
        ShowWindow(hwnd, SW_MAXIMIZE);
    } else {
        // Aquí puedes implementar cambios de tamaño específicos si lo deseas
        // Por ejemplo, establecer un tamaño específico
        COORD bufferSize = { 80, 25 }; // Ajusta según tus necesidades
        SMALL_RECT windowSize = { 0, 0, 79, 24 }; // Ajusta el tamaño de la ventana
        SetConsoleScreenBufferSize(hConsole, bufferSize);
        SetConsoleWindowInfo(hConsole, TRUE, &windowSize);
    }
}

// Función para mostrar mensajes de alerta
bool msg(const string& text, const string& type, const string& size = "medium", const string& sound = "", const string& title = "") {
    UINT typeFlag;

    // Determina el tipo de mensaje y el icono correspondiente
    if (type == "alert") {
        typeFlag = MB_ICONWARNING | MB_OK;
        string defaultTitle = "Alerta";
        MessageBoxA(NULL, text.c_str(), title.empty() ? defaultTitle.c_str() : title.c_str(), typeFlag);
    } else if (type == "error") {
        typeFlag = MB_ICONERROR | MB_OK;
        string defaultTitle = "Error";
        MessageBoxA(NULL, text.c_str(), title.empty() ? defaultTitle.c_str() : title.c_str(), typeFlag);
    } else if (type == "info") {
        typeFlag = MB_ICONINFORMATION | MB_OK;
        string defaultTitle = "Información";
        MessageBoxA(NULL, text.c_str(), title.empty() ? defaultTitle.c_str() : title.c_str(), typeFlag);
    } else if (type == "yesno") {
        string defaultTitle = "Confirmación";
        int result = MessageBoxA(NULL, text.c_str(), title.empty() ? defaultTitle.c_str() : title.c_str(), MB_YESNO | MB_ICONQUESTION);
        return result == IDYES; // Retorna true si es "Sí"
    } else {
        typeFlag = MB_OK; // Tipo por defecto
        string defaultTitle = "Mensaje";
        MessageBoxA(NULL, text.c_str(), title.empty() ? defaultTitle.c_str() : title.c_str(), typeFlag);
    }

    // Reproducir sonido si se proporciona un archivo
    if (!sound.empty()) {
        playSound(sound.c_str());
    }

    return false; // Por defecto
}

// Clase para manejar múltiples saltos de línea
class Endls {
public:
    Endls(int count) : count(count) {}

    // Sobrecarga del operador << para usar con cout
    friend ostream& operator<<(ostream& os, const Endls& e) {
        for (int i = 0; i < e.count; ++i) {
            os << endl; // Imprime un salto de línea
        }
        return os; // Devuelve el flujo
    }

private:
    int count;
};

// Función que crea un objeto Endls
Endls endls(int count) {
    return Endls(count);
}

#endif // JASG_H

