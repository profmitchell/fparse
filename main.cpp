
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip> // For std::setprecision
#include "FormulaParser.h"

// Function to read formulas from a file
std::vector<std::string> readFormulasFromFile(const std::string& filePath) {
    std::ifstream file(filePath);
    std::vector<std::string> formulas;
    std::string formula;

    if (file.is_open()) {
        while (std::getline(file, formula)) {  // Read each line as a formula
            if (!formula.empty()) {
                formulas.push_back(formula);
            }
        }
        file.close();
    } else {
        std::cerr << "Error: Could not open file " << filePath << std::endl;
    }

    if (formulas.empty()) {
        std::cerr << "Error: No formulas found in the file or file is empty." << std::endl;
    }

    return formulas;
}

int main() {
    // Example formulas to test
    std::string formulas[] = {
        "sin(x)", "cos(x)", "tanh(x)", "atan(x)", "exp(x) - 1",
        "1 / (1 + exp(-x))", "log(x + 1)", "fabs(x) - 0.5", "sqrt(fabs(x))",
        "pow(x, 2)", "pow(fabs(x), 0.5)", "alpha * sin(x) + (1 - alpha) * cos(x)",
        "sin(x) * cos(x)", "(x + 1) * sin(x)", "sin(x) / (1 + fabs(x))",
        "x / (1 + fabs(x))", "sin(x) + x / (1 + fabs(x))", "exp(-fabs(x))",
        "min(max(x, -1), 1)", "2 * atan(x)", "atan(2 * x)", "pow(x, 3)",
        "pow(x, -1)", "log(fabs(x) + 1)", "1 / (1 + exp(-fabs(x)))",
        "(x - 1) * log(x + 1)", "fabs(x) * sin(x)", "sqrt(x * x + 1)",
        "max(x, -x)", "min(x + 1, 2 * x)", "x * atan(x)",
        "exp(x) / (1 + exp(x))", "alpha * pow(x, 2) + (1 - alpha) * exp(-x)",
        "cos(x) + tanh(x)", "atan(x) * exp(x)"
    };

    int totalSuccesses = 0;
    int totalTests = 0;

    // Iterate over each formula and parse it
    for (const auto& formula : formulas) {
        std::cout << "Testing formula: " << formula << std::endl;
        FormulaParser parser(formula);

        // Log before parsing
        std::cout << "Parsing formula: " << formula << std::endl;
        bool success = parser.parse();

        if (!success) {
            std::cerr << "Failed to parse formula: " << formula << std::endl;
        } else {
            // Evaluate at several points to test
            double testInputs[] = {-1, -0.5, 0, 0.5, 1, -0.01};
            int successes = 0; // Count successes for this formula
            int formulaTests = 0; // Count total tests for this formula

            for (double x : testInputs) {
                try {
                    double result = parser.evaluate(x);
                    std::cout << "Input: " << x << " -> Evaluated Result: " << result << std::endl;

                    // Check if result is within the range (-1, 1)
                    if (result >= -1 && result <= 1) {
                        successes++;
                    }
                    formulaTests++;
                } catch (const std::exception& e) {
                    std::cerr << "Error: " << e.what() << " for input " << x << std::endl;
                }
            }

            // Update overall success and test counts
            totalSuccesses += successes;
            totalTests += formulaTests;

            // Display the percentage of outputs within range for this formula
            double formulaPercentage = (static_cast<double>(successes) / formulaTests) * 100.0;
            std::cout << "Percentage of outputs within range (-1 to 1) for " << formula << ": "
                      << std::fixed << std::setprecision(2) << formulaPercentage << "%" << std::endl;
        }
        std::cout << "----------------------------------------------------" << std::endl;
    }

    // Calculate and display the overall success rate
    double overallPercentage = (static_cast<double>(totalSuccesses) / totalTests) * 100.0;

    // Output the overall percentage in the required format
    std::cout << "\n          YOU GET " << std::fixed << std::setprecision(2) << overallPercentage << "%" << std::endl;
    std::cout << "----------------------------------------------------" << std::endl;
    return 0;
}
