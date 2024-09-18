#ifndef FORMULA_PARSER_H
#define FORMULA_PARSER_H

#include <string>
#include <vector>
#include <map>
#include <functional>
#include <regex>

class FormulaParser {
public:
    FormulaParser(const std::string& formula);  // Constructor to initialize the formula
    void setAlpha(double alpha);                // Method to set the alpha value
    bool parse();                               // Method to parse the formula
    double evaluate(double x);                  // Method to evaluate the formula

private:
    std::string formula_;                       // The formula to be parsed and evaluated
    double alpha_;                              // Alpha value used in formula calculations
    std::vector<std::string> tokens_;           // Tokens extracted from the formula

    // Maps for unary and binary functions
    std::map<std::string, std::function<double(double)>> functionMapUnary;         // Map for unary functions
    std::map<std::string, std::function<double(double, double)>> functionMapBinary; // Map for binary functions

    // Initialization and utility functions
    void initializeFunctionMap();               // Initialize the function maps
    void initializeRegexPatterns();             // Initialize all regex patterns
    double evaluateToken(double x, const std::string& token); // Evaluate a specific token

    // Helper function to match formulas with regex patterns
    bool matchWithRegex(const std::regex& regex); // Match the formula with a given regex pattern
    
    // Handling functions for specific math operations
    static double handleSin(double x);          // Handle sin function
    static double handleCos(double x);          // Handle cos function
    static double handleTanh(double x);         // Handle tanh function
    static double handleFabs(double x);         // Handle fabs function
    static double handleSqrt(double x);         // Handle sqrt function
    static double handleExp(double x);          // Handle exp function
    static double handleLog(double x);          // Handle log function
    static double handleAtan(double x);         // Handle atan function
    static double handleMin(double x, double y); // Handle min function
    static double handleMax(double x, double y); // Handle max function
    static double handlePow(double base, double exponent); // Handle pow function

    // Regex patterns for matching specific formulas
    std::regex basicRegex;                      // Regex for basic patterns
    std::regex nestedRegex;                     // Regex for nested patterns
    std::regex edgeCaseRegex;                   // Regex for complex expressions
    std::regex failSpecificRegex;               // Regex for specific failure patterns
    std::regex expNegRegex;                     // Regex for expressions involving exp(-x)
    std::regex logPlusRegex;                    // Regex for log(x + 1) patterns
    std::regex powRegex;                        // Regex for pow(x, constant or expression)
    std::regex expAbsRegex;                     // Regex for exp(-fabs(x)) patterns
    std::regex blendedRegex1;                   // Regex for blended patterns like 2 * atan(x)
    std::regex blendedRegex2;                   // Regex for blended patterns like (x + 1) * sin(x)
    std::regex expBlendedRegex;                 // Regex for exp(-fabs(x)) patterns
    std::regex nestedFunctionRegex;             // Regex for nested functions
    std::regex operatorRegex;                   // Regex for operators
    std::regex divisionByZeroRegex;             // Regex for division by zero
};

#endif // FORMULA_PARSER_H
