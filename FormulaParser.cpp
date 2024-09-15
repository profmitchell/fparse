#include "FormulaParser.h"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <regex>
#include <limits>
#include <cmath>

FormulaParser::FormulaParser(const std::string& formula)
    : formula_(formula), alpha_(0.5) {
    initializeFunctionMap();
}

void FormulaParser::setAlpha(double alpha) {
    if (alpha < 0.0 || alpha > 1.0) {
        throw std::runtime_error("Alpha must be between 0 and 1");
    }
    alpha_ = alpha;
}

void FormulaParser::initializeFunctionMap() {
    functionMapUnary = {
        {"sin", [](double x) { return handleSin(x); }},
        {"cos", [](double x) { return handleCos(x); }},
        {"tanh", [](double x) { return handleTanh(x); }},
        {"fabs", [](double x) { return handleFabs(x); }},
        {"sqrt", [](double x) { return handleSqrt(x); }},
        {"exp", [](double x) { return handleExp(x); }},
        {"log", [](double x) { return handleLog(x); }},
        {"atan", [](double x) { return handleAtan(x); }}
    };

    functionMapBinary = {
        {"+", [](double x, double y) { return x + y; }},
        {"-", [](double x, double y) { return x - y; }},
        {"*", [](double x, double y) { return x * y; }},
        {"/", [](double x, double y) {
            if (y == 0) throw std::runtime_error("Division by zero");
            return x / y;
        }},
        {"min", [](double x, double y) { return handleMin(x, y); }},
        {"max", [](double x, double y) { return handleMax(x, y); }},
        {"pow", [](double base, double exponent) { return handlePow(base, exponent); }}
    };
}

double FormulaParser::handleSin(double x) { return std::sin(x); }
double FormulaParser::handleCos(double x) { return std::cos(x); }
double FormulaParser::handleTanh(double x) { return std::tanh(x); }
double FormulaParser::handleFabs(double x) { return std::fabs(x); }
double FormulaParser::handleSqrt(double x) {
    if (x < 0) {
        throw std::runtime_error("Square root of a negative number is undefined.");
    }
    return std::sqrt(x);
}

double FormulaParser::handleExp(double x) {
    if (x > std::log(std::numeric_limits<double>::max())) {
        throw std::runtime_error("Exponential result too large to handle.");
    }
    return std::exp(x);
}

double FormulaParser::handleLog(double x) {
    if (x <= 0) {
        throw std::runtime_error("Logarithm of non-positive number is undefined.");
    }
    return std::log(x);
}

double FormulaParser::handleMin(double x, double y) { return std::fmin(x, y); }
double FormulaParser::handleMax(double x, double y) { return std::fmax(x, y); }
double FormulaParser::handleAtan(double x) { return std::atan(x); }
double FormulaParser::handlePow(double base, double exponent) {
    if (base == 0 && exponent < 0) {
        throw std::runtime_error("Invalid input for pow: Zero to negative power.");
    }
    return std::pow(base, exponent);
}

bool FormulaParser::matchWithRegex(const std::regex& regex) {
    auto words_begin = std::sregex_iterator(formula_.begin(), formula_.end(), regex);
    auto words_end = std::sregex_iterator();
    tokens_.clear();
    for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
        tokens_.push_back((*i).str());
    }
    return !tokens_.empty();
}


bool FormulaParser::parse() {
    std::cout << "Parsing formula: " << formula_ << std::endl;

    // List of regex patterns and their descriptions
    std::vector<std::pair<std::regex, std::string>> regexPatterns = {
        {std::regex(R"((\balpha\b)|(\w+\s*\(x\))|([+\-*/()])|(\b\d+\.?\d*\b)|(x)|M_PI)"), "Basic Regex"},
        {std::regex(R"((\balpha\b)|(\w+\s*\((?:[^()]+|\([^()]*\))*\))|([+\-*/()])|(\b\d+\.?\d*\b)|(x)|M_PI)"), "Nested Regex"},
        {std::regex(R"((\w+\s*\(-?[\w+\-*\/().\s]+\))|([+\-*/()])|(\b\d+\.?\d*\b)|(x)|M_PI)"), "Edge Case Regex"},
        {std::regex(R"((\w+\s*\(-?\w+\s*\(-?\w+[^()]*\)\))|(\w+\s*\(.*\))|([+\-*/()])|(\b\d+\.?\d*\b)|(x)|M_PI)"), "Fail Specific Regex"},
        {std::regex(R"((\d*\s*\/\s*\(\s*1\s*\+\s*exp\s*\(\s*-\s*x\s*\)\s*\)))"), "Exponential Negative Regex"},
        {std::regex(R"(log\s*\(\s*x\s*\+\s*1\s*\))"), "Logarithm Plus Regex"},
        {std::regex(R"(exp\s*\(\s*-\s*fabs\s*\(\s*x\s*\)\s*\))"), "Exponential Absolute Regex"},
        {std::regex(R"(pow\s*\(\s*x\s*,\s*-?\d+\.?\d*\s*\))"), "Power Function Regex"},
        {std::regex(R"(\d+\s*\*\s*atan\s*\(x\))"), "Blended Function Regex 1"},
        {std::regex(R"(\(x\s*\+\s*1\)\s*\*\s*sin\s*\(x\))"), "Blended Function Regex 2"},
        {std::regex(R"(exp\s*\(\s*-\s*fabs\s*\(x\)\s*\))"), "Exponential Blended Regex"}
    };

    for (const auto& [regexPattern, description] : regexPatterns) {
        std::cout << "Trying " << description << " on formula: " << formula_ << std::endl;
        if (matchWithRegex(regexPattern)) {
            std::cout << "Matched with " << description << std::endl;
            break;
        } else {
            std::cout << "No match with " << description << std::endl;
        }
    }

    if (tokens_.empty()) {
        std::cerr << "Error: Formula parsing failed for " << formula_ << std::endl;
        return false;
    }

    std::cout << "Parsed tokens: ";
    for (const std::string& token : tokens_) {
        std::cout << token << " ";
    }
    std::cout << std::endl;

    return true;
}

double FormulaParser::evaluate(double x) {
    if (tokens_.empty()) {
        throw std::runtime_error("No formula to evaluate");
    }

    std::vector<double> values;
    std::vector<std::string> ops;

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
            ops.pop_back();
        } else {
            while (!ops.empty() && ops.back() != "(") {
                double val2 = values.back(); values.pop_back();
                double val1 = values.back(); values.pop_back();
                std::string op = ops.back(); ops.pop_back();
                values.push_back(applyOp(val1, val2, op));
            }
            ops.push_back(token);
        }
    }

    while (!ops.empty()) {
        double val2 = values.back(); values.pop_back();
        double val1 = values.back(); values.pop_back();
        std::string op = ops.back(); ops.pop_back();
        values.push_back(applyOp(val1, val2, op));
    }

    return values.back();
}

double FormulaParser::evaluateToken(double x, const std::string& token) {
    if (token == "x") return x;
    if (token == "alpha") return alpha_;

    std::smatch match;
    if (std::regex_match(token, match, std::regex(R"(pow\s*\(([^,]+),\s*([^)]+)\))"))) {
        double baseValue = evaluateToken(x, match[1]);
        double exponentValue = evaluateToken(x, match[2]);

        return handlePow(baseValue, exponentValue);
    }

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

