#include "Occupation.h"
#include "Helpers.h"
#include <sstream>
#include <iomanip>
#include <random>

std::string Occupation::getOccupation() const {
	return occupation;
}
int Occupation::getMatrixCodeInt() const {
	return matrixCodeInt;
}
std::string Occupation::getMatrixCode() const {
	return matrixCode;
}
std::string Occupation::getMatrixPrefix() const {
	auto dash = matrixCode.find('-');
	return matrixCode.substr(0, dash);
}
std::string Occupation::getMatrixSuffix() const {
	auto dash = matrixCode.find('-');
	return matrixCode.substr(dash + 1);
}
std::string Occupation::getOccupationType() const {
	return occupationType;
}
float Occupation::getEmploymentCurrent() const {
	return employmentCurrent;
}
std::string Occupation::getEmploymentCurrentString() const {
	return employmentCurrentString;
}
float Occupation::getEmploymentFuture() const {
	return employmentFuture;
}
std::string Occupation::getEmploymentFutureString() const {
	return employmentFutureString;
}
float Occupation::getDistributionCurrent() const {
	return distributionCurrent;
}
std::string Occupation::getDistributionCurrentString() const {
	return distributionCurrentString;
}
float Occupation::getDistributionFuture() const {
	return distributionFuture;
}
std::string Occupation::getDistributionFutureString() const {
	return distributionFutureString;
}
float Occupation::getNumericChange() const {
	return numericChange;
}
std::string Occupation::getNumericChangeString() const {
	return numericChangeString;
}
float Occupation::getPercentageChange() const {
	return percentChange;
}
std::string Occupation::getPercentageChangeString() const {
	return percentChangeString;
}
float Occupation::getPercentSelfEmployed() const {
	return percentSelfEmployed;
}
std::string Occupation::getPercentSelfEmployedString() const {
	return percentSelfEmployedString;
}
float Occupation::getJobOpenings() const {
	return jobOpenings;
}
std::string Occupation::getJobOpeningsString() const {
	return jobOpeningsString;
}
float Occupation::getWage() const {
	return wage;
}
std::string Occupation::getWageString() const {
	return wageString;
}
std::string Occupation::getEducation() const {
	return education;
}
int Occupation::getEducationScore() const {
	return educationScore;
}
std::string Occupation::getWorkExperience() const {
	return workExperience;
}
int Occupation::getWorkExperienceScore() const {
	return workExperienceScore;
}
std::string Occupation::getTraining() const {
	return training;
}
std::string Occupation::getHandbookContent() const {
	return handbookContent;
}
int Occupation::getJobIndex() const {
	return jobIndex;
}

// setters assign std::string arguments to std::string data fields and call toFloat() function for float data fields
void Occupation::setOccupation(const std::string &o) {
	occupation = o;
}
void Occupation::setMatrixCode(const std::string &mc) {
	if (mc == "-") {
		std::random_device generator;
		// if matrix code is blank, generate a 6-digit matrix code to prevent possible
		// conflicts with 5-digit codes
		std::uniform_int_distribution<int> distribution(100000, 999999);
		int randomNumber = distribution(generator);;
		matrixCode = std::to_string(randomNumber);
		matrixCodeInt = randomNumber;
	}
	else {
		matrixCode = mc;
		// erases the - character from the matrix code
		std::string cleanedMatrixCode;
		for (char c : mc) {
			if (isnumber(c)) {
				cleanedMatrixCode+= c;
			}
		}
		// converts the modified code to an int
		matrixCodeInt = stoi(cleanedMatrixCode);
	}
}
void Occupation::setOccupationType(const std::string &ot) {
	occupationType = ot;
}
void Occupation::setEmploymentCurrent(const std::string &ec) {
	employmentCurrentString = ec;
	employmentCurrent = toFloat(ec);
}
void Occupation::setEmploymentFuture(const std::string &ef) {
	employmentFutureString = ef;
	employmentFuture = toFloat(ef);
}
void Occupation::setDistributionCurrent(const std::string &dc) {
	distributionCurrentString = dc;
	distributionCurrent = toFloat(dc);
}
void Occupation::setDistributionFuture(const std::string &df) {
	distributionFutureString = df;
	distributionFuture = toFloat(df);
}
void Occupation::setNumericChange(const std::string &nc) {
	numericChangeString = nc;
	numericChange = toFloat(nc);
}
void Occupation::setPercentageChange(const std::string &pc) {
	percentChangeString = pc;
	percentChange = toFloat(pc);
}
void Occupation::setPercentSelfEmployed(const std::string &pse) {
	percentSelfEmployedString = pse;
	percentSelfEmployed = toFloat(pse);
}
void Occupation::setJobOpenings(const std::string &jo) {
	jobOpeningsString = jo;
	jobOpenings = toFloat(jo);
}
void Occupation::setWage(const std::string &w) {
	wageString = w;
	std::string cleanedWage;
	for (char c : w) {
		if (isnumber(c)) {
			cleanedWage += c;
		}
	}
	wage = toFloat(cleanedWage);
}
void Occupation::setEducation(const std::string &e) {
	education = e;
	int score = 0;
	if (e == "No formal educational credential") {
		score = 1;
	}
	else if (e == "High school diploma or equivalent") {
		score = 2;
	}
	else if (e == "Postsecondary nondegree award") {
		score = 3;
	}
	else if (e == "Some college, no degree") {
		score = 4;
	}
	else if (e == "Associate's degree") {
		score = 5;
	}
	else if (e == "Bachelor's degree") {
		score = 6;
	}
	else if (e == "Master's degree") {
		score = 7;
	}
	else if (e == "Doctoral or professional degree") {
		score = 8;
	}
	educationScore = score;
}
void Occupation::setWorkExperience(const std::string &we) {
	workExperience = we;
	int score = 0;
	if (we == "None") {
		score = 1;
	}
	else if (we == "Less than 5 years") {
		score = 2;
	}
	else if (we == "5 years or more") {
		score = 3;
	}
	workExperienceScore = score;
}
void Occupation::setTraining(const std::string &t) {
	training = t;
}
void Occupation::setHandbookContent(const std::string &hc) {
	handbookContent = hc;
}
void Occupation::setJobIndex(int i) {
	jobIndex = i;
}

void Occupation::calculateChanges() {
	if (employmentCurrentString == "-" || employmentFutureString == "-") {
		setNumericChange("-");
		setPercentageChange("-");
		return;
	}

	// calculating numeric and percent and rounding off to one decimal place
	float numeric = employmentFuture - employmentCurrent;
	float percent =	numeric / employmentCurrent * 100;

	setNumericChange(formatOneDecimal(numeric));
	setPercentageChange(formatOneDecimal(percent));
}

std::string Occupation::formatOneDecimal(float &value) {
	// rounding to one decimal place
	value = std::round(value * 10) / 10.0f;

	std::ostringstream roundedString;
	roundedString << std::fixed << std::setprecision(1) << value;
	return roundedString.str();
}
