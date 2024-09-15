#ifndef FORMULA_PARSER_H
#define FORMULA_PARSER_H

#include <string>
#include <vector>
#include <map>
#include <functional>

class FormulaParser {
public:
    FormulaParser(const std::string& formula);
    void setAlpha(double alpha);
    bool parse();
    double evaluate(double x);

private:
    std::string formula_;
    double alpha_;
    std::vector<std::string> tokens_;
    
    // Maps for unary and binary functions
    std::map<std::string, std::function<double(double)>> functionMapUnary;
    std::map<std::string, std::function<double(double, double)>> functionMapBinary;

    // Initialization and utility functions
    void initializeFunctionMap();
    double evaluateToken(double x, const std::string& token);
    bool matchSpecificPatterns(); // Function to match specific problematic formulas

    // Handling functions for specific math operations
    static double handleSin(double x);
    static double handleCos(double x);
    static double handleTanh(double x);
    static double handleFabs(double x);
    static double handleSqrt(double x);
    static double handleExp(double x);
    static double handleLog(double x);
    static double handleAtan(double x);
    static double handleMin(double x, double y);
    static double handleMax(double x, double y);
    static double handlePow(double base, double exponent);
};

#endif // FORMULA_PARSER_H
