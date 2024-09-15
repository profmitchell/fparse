#include <iostream>
#include <fstream>
#include <vector>
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
    // Specify the path to your formulas.txt file
    std::string filePath = "/Users/mitchellcohen/Desktop/formulas.txt";
    std::vector<std::string> formulas = readFormulasFromFile(filePath);

    if (formulas.empty()) {
        std::cerr << "Error: No formulas provided." << std::endl;
        return 1;
    }

    // Define test values for evaluation
    double testValues[] = { -1.0, -0.5, 0.0, 0.5, 1.0 };

    // Loop through each formula and evaluate it
    for (const std::string& formula : formulas) {
        std::cout << "Testing formula: " << formula << std::endl;
        FormulaParser parser(formula);
        if (!parser.parse()) {
            std::cerr << "Error: Formula parsing failed for " << formula << std::endl;
            continue;
        }

        // Evaluate the formula for each test value
        for (double x : testValues) {
            try {
                double result = parser.evaluate(x);
                std::cout << "Input: " << x << " -> Evaluated Result: " << result << std::endl;
            } catch (const std::runtime_error& e) {
                std::cerr << "Error: " << e.what() << " for input " << x << std::endl;
            }
        }
        std::cout << std::endl;  // Blank line between formula tests
    }

    return 0;
}
