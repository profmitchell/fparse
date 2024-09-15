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
    std::string filePath = "/Users/mitchellcohen/Desktop/formulas.txt";
    std::vector<std::string> formulas = readFormulasFromFile(filePath);

    if (formulas.empty()) {
        std::cerr << "Error: No formulas provided." << std::endl;
        return 1;
    }

    // Define test values for evaluation
    double testValues[] = { -1.0, -0.5, 0.0, 0.5, 1.0, -0.01 };

    double totalTests = 0;
    double totalSuccesses = 0;

    for (const std::string& formula : formulas) {
        std::cout << "Testing formula: " << formula << std::endl;
        FormulaParser parser(formula);
        if (!parser.parse()) {
            std::cerr << "Error: Formula parsing failed for " << formula << std::endl;
            continue;
        }

        int successCount = 0;
        int testCount = sizeof(testValues) / sizeof(testValues[0]);

        for (double x : testValues) {
            try {
                double result = parser.evaluate(x);
                std::cout << "Input: " << x << " -> Evaluated Result: " << result << std::endl;

                // Check if result is within acceptable range (-1 to 1)
                if (result >= -1.0 && result <= 1.0) {
                    successCount++;
                }
            } catch (const std::runtime_error& e) {
                std::cerr << "Error: " << e.what() << " for input " << x << std::endl;
            }
        }

        double percentage = (static_cast<double>(successCount) / testCount) * 100.0;
        totalTests += testCount;
        totalSuccesses += successCount;

        std::cout << "Percentage of outputs within range (-1 to 1) for " << formula << ": "
                  << std::fixed << std::setprecision(2) << percentage << "%" << std::endl << std::endl;
    }

    // Calculate and display the overall success rate
    double overallPercentage = (totalSuccesses / totalTests) * 100.0;
    std::cout << "Overall success rate: " << std::fixed << std::setprecision(2) << overallPercentage << "%" << std::endl;

    return 0;
}
