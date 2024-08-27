#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <sstream>
#include "textToMatrix.h"


//Preprocess step to extract variables coefficients.
std::string replaceVariablesWithOne(const std::string& equation) {
    // Regex to match variables x, y, z
    std::regex variableRegex("[xyz]");
    std::string processedEquation = regex_replace(equation, variableRegex, "*1*");
    return processedEquation;
}

// Function to split an equation string into parts based on signs
std::vector<std::string> splitBySigns(const std::string& equation) {
    std::vector<std::string> parts;
    std::regex segmentRegex("([+-]?\d*[xyz]|[+-]?\d+)"); // Match terms with signs, variables, or constants
    std::smatch match;
    std::string::const_iterator searchStart(equation.cbegin());

    while (regex_search(searchStart, equation.cend(), match, segmentRegex)) {
        if (!match[0].str().empty()) {
            parts.push_back(match[0].str());
        }
        searchStart = match.suffix().first;
    }
    return parts;
}

//Preprocess step to extract variables coefficients.
std::string processAsterisks(const std::string& equation) {
    std::string result = equation;
    
    // Regex to match patterns with * between two numbers
    std::regex multiplyRegex("(\\d+)\\*(\\d+)");
    std::smatch match;

    // Replace * with the product of the numbers
    while (regex_search(result, match, multiplyRegex)) {
        int num1 = stoi(match[1]);
        int num2 = stoi(match[2]);
        int product = num1 * num2;
        result.replace(match.position(0), match.length(0), std::to_string(product));
    }

    // Regex to match standalone * not between two numbers
    std::regex standaloneAsteriskRegex("\\*");
    result = regex_replace(result, standaloneAsteriskRegex, "");
    return result;
}

std::vector<int> extractNumbersWithSigns(const std::string& equation) {
    std::vector<int> numbers;
    // Match optional sign and number
    std::regex numberRegex("([+-]?)\\s*(\\d+)");
    std::smatch match;
    std::string::const_iterator searchStart(equation.cbegin());
    
    while (regex_search(searchStart, equation.cend(), match, numberRegex)) {
        std::string sign = match[1].str();
        int number = stoi(match[2].str());

        // Determine the actual number value based on the sign.
        if (sign == "-") {
            number = -number;
        } else if (sign.empty() || sign == "+") {
            // Number is positive if there's no sign or a + sign.
        } // No action needed for empty sign (positive) or + sign.
        
        numbers.push_back(number);
        searchStart = match.suffix().first;
    }

    return numbers;
}

//Main function in this file. This function returns the extracted text.
std::vector<std::vector<int>> extractInfo(std::string& text){
    std::vector<std::string> equations;
    std::istringstream iss(text);
    std::string line;
    std::vector<std::string > processedlines;
    std::vector<std::vector<int> > linesToNumbers ;

    while (getline(iss, line)) {
        line = replaceVariablesWithOne(line);
        line = processAsterisks(line) ;
        processedlines.push_back(line) ;
        std::vector<int> lineOfNum = extractNumbersWithSigns(line) ;
        linesToNumbers.push_back(lineOfNum) ;
    }

    return linesToNumbers ;
}
