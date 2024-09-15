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

    void initializeFunctionMap();
    double evaluateToken(double x, const std::string& token);

    // Handling functions
    static double handleSin(double x);
    static double handleCos(double x);
    static double handleTanh(double x);
    static double handleFabs(double x);
    static double handleSqrt(double x);
    static double handleExp(double x);
    static double handleLog(double x);
    static double handleSigmoid(double x);
    static double handleAtan(double x);
    static double handleCeil(double x);
    static double handleFloor(double x);
    static double handleRound(double x);
    static double handlePow(double base, double exponent);
    
};

#endif // FORMULA_PARSER_H
