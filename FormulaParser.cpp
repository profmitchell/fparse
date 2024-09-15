#include "FormulaParser.h"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <regex>
#include <limits>  // For std::numeric_limits
#include <cmath>   // For mathematical functions

//---------------------------------------

FormulaParser::FormulaParser(const std::string& formula)
    : formula_(formula), alpha_(0.5) {  // Default alpha value
    initializeFunctionMap();
}

//---------------------------------------

void FormulaParser::setAlpha(double alpha) {
    if (alpha < 0.0 || alpha > 1.0) {
        throw std::runtime_error("Alpha must be between 0 and 1");
    }
    alpha_ = alpha;
}

//---------------------------------------

void FormulaParser::initializeFunctionMap() {
    // Initialize the map for unary functions with individual handling
    functionMapUnary = {
        {"sin", [](double x) { return handleSin(x); }},
        {"cos", [](double x) { return handleCos(x); }},
        {"tanh", [](double x) { return handleTanh(x); }},
        {"fabs", [](double x) { return handleFabs(x); }},
        {"sqrt", [](double x) { return handleSqrt(x); }},
        {"exp", [](double x) { return handleExp(x); }},
        {"log", [](double x) { return handleLog(x); }},
        {"sigmoid", [](double x) { return handleSigmoid(x); }},
        {"atan", [](double x) { return handleAtan(x); }},
        {"ceil", [](double x) { return handleCeil(x); }},
        {"floor", [](double x) { return handleFloor(x); }},
        {"round", [](double x) { return handleRound(x); }},
    };

    // Initialize the map for binary functions
    functionMapBinary = {
        {"+", [](double x, double y) { return x + y; }},
        {"-", [](double x, double y) { return x - y; }},
        {"*", [](double x, double y) { return x * y; }},
        {"/", [](double x, double y) {
            if (y == 0) throw std::runtime_error("Division by zero");
            return x / y;
        }},
        {"min", [](double x, double y) { return std::fmin(x, y); }},
        {"max", [](double x, double y) { return std::fmax(x, y); }},
        {"pow", [](double base, double exponent) { return handlePow(base, exponent); }},
    };
}

//---------------------------------------

// Individual handling functions for unary operations
double FormulaParser::handleSin(double x) {
    return std::sin(x);
}

//---------------------------------------

double FormulaParser::handleCos(double x) {
    return std::cos(x);
}

//---------------------------------------

double FormulaParser::handleTanh(double x) {
    return std::tanh(x);
}

//---------------------------------------

double FormulaParser::handleFabs(double x) {
    return std::fabs(x);
}

//---------------------------------------

double FormulaParser::handleSqrt(double x) {
    if (x < 0) {
        throw std::runtime_error("Square root of a negative number is undefined.");
    }
    return std::sqrt(x);
}

//---------------------------------------

double FormulaParser::handleExp(double x) {
    if (x > std::log(std::numeric_limits<double>::max())) {
        throw std::runtime_error("Exponential result too large to handle.");
    }
    double result = std::exp(x);
    std::cout << "Exp(" << x << ") = " << result << std::endl;
    return result;
}


//---------------------------------------

double FormulaParser::handleLog(double x) {
    if (x <= 0) {
        throw std::runtime_error("Logarithm of non-positive number is undefined.");
    }
    return std::log(x);
}

//---------------------------------------

double FormulaParser::handleSigmoid(double x) {
    return 1.0 / (1.0 + std::exp(-x));
}

//---------------------------------------

double FormulaParser::handleAtan(double x) {
    return std::atan(x);
}

//---------------------------------------

double FormulaParser::handleCeil(double x) {
    return std::ceil(x);
}

//---------------------------------------

double FormulaParser::handleFloor(double x) {
    return std::floor(x);
}

//---------------------------------------

double FormulaParser::handleRound(double x) {
    return std::round(x);
}

//---------------------------------------

// Individual handling function for the power operation
double FormulaParser::handlePow(double base, double exponent) {
    if (base == 0 && exponent < 0) {
        throw std::runtime_error("Invalid input for pow: Zero to negative power.");
    }
    return std::pow(base, exponent);
}

//---------------------------------------

bool FormulaParser::parse() {
    std::cout << "Parsing formula: " << formula_ << std::endl;
    std::regex tokenRegex(R"((\balpha\b)|(\w+\([^\)]+\))|([+\-*/()])|(\b\d+\.?\d*\b)|(x))");
    auto words_begin = std::sregex_iterator(formula_.begin(), formula_.end(), tokenRegex);
    auto words_end = std::sregex_iterator();

    tokens_.clear();  // Clear previous tokens
    for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
        tokens_.push_back((*i).str());
    }

    std::cout << "Parsed tokens: ";
    for (const std::string& token : tokens_) {
        std::cout << token << " ";
    }
    std::cout << std::endl;

    return !tokens_.empty();
}




//---------------------------------------

double FormulaParser::evaluate(double x) {
    if (tokens_.empty()) {
        throw std::runtime_error("No formula to evaluate");
    }

    std::vector<double> values;  // Stack for values
    std::vector<std::string> ops;  // Stack for operators

    auto applyOp = [](double a, double b, const std::string& op) -> double {
        if (op == "+") return a + b;
        if (op == "-") return a - b;
        if (op == "*") return a * b;
        if (op == "/") {
            if (b == 0) throw std::runtime_error("Division by zero");
            return a / b;
        }
        throw std::runtime_error("Unsupported operation: " + op);
    };

    for (const std::string& token : tokens_) {
        if (std::isdigit(token[0]) || token == "x" || token == "alpha" || token.find("(x)") != std::string::npos) {
            values.push_back(evaluateToken(x, token));
        } else if (token == "(") {
            ops.push_back(token);
        } else if (token == ")") {
            while (!ops.empty() && ops.back() != "(") {
                double val2 = values.back(); values.pop_back();
                double val1 = values.back(); values.pop_back();
                std::string op = ops.back(); ops.pop_back();
                values.push_back(applyOp(val1, val2, op));
            }
            ops.pop_back();  // Pop the '('
        } else {  // Operator
            while (!ops.empty() && ops.back() != "(") {
                double val2 = values.back(); values.pop_back();
                double val1 = values.back(); values.pop_back();
                std::string op = ops.back(); ops.pop_back();
                values.push_back(applyOp(val1, val2, op));
            }
            ops.push_back(token);
        }
    }

    // Apply remaining operations
    while (!ops.empty()) {
        double val2 = values.back(); values.pop_back();
        double val1 = values.back(); values.pop_back();
        std::string op = ops.back(); ops.pop_back();
        values.push_back(applyOp(val1, val2, op));
    }

    return values.back();
}

//---------------------------------------
double FormulaParser::evaluateToken(double x, const std::string& token) {
    if (token == "x") return x;
    if (token == "alpha") return alpha_;

    std::smatch match;
    if (std::regex_match(token, match, std::regex(R"((\w+)\(([^)]+)\))"))) {
        std::string functionName = match[1];
        std::string arguments = match[2];
        double argValue = 0;

        if (arguments == "x") argValue = x;
        else if (arguments == "alpha") argValue = alpha_;
        else {
            try {
                argValue = std::stod(arguments);
            } catch (const std::invalid_argument&) {
                throw std::runtime_error("Invalid argument for function: " + arguments);
            }
        }

        auto it = functionMapUnary.find(functionName);
        if (it != functionMapUnary.end()) return it->second(argValue);

        throw std::runtime_error("Unknown function: " + functionName);
    }

    auto it = functionMapUnary.find(token);
    if (it != functionMapUnary.end()) return it->second(x);

    try {
        return std::stod(token);
    } catch (const std::invalid_argument&) {
        throw std::runtime_error("Invalid token: " + token);
    }
}
