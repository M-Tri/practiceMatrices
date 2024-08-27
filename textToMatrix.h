#ifndef TEXTTOMATRIX_H
#define TEXTTOMATRIX_H

#include <vector>
#include <string>

std::vector<std::vector<int> > extractInfo(std::string& text);
std::vector<int> extractNumbersWithSigns(const std::string& equation);
std::string processAsterisks(const std::string& equation);
std::string replaceVariablesWithOne(const std::string& equation);
std::vector<std::string> splitBySigns(const std::string& equation);

#endif // EQUATION_PROCESSOR_H
