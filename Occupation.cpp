// Occupation.cpp
// Darren Daniel

#include "Occupation.h"
#include "Helpers.h"

// getters return the specific data field of jobStruct
std::string Occupation::getOccupation() const {
	return occupation;
}
int Occupation::getMatrixCodeInt() const {
	return matrixCodeInt;
}
std::string Occupation::getMatrixCode() const {
	return matrixCode;
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
void Occupation::setOccupation(std::string o) {
	occupation = o;
}
void Occupation::setMatrixCode(std::string mc) {
	if (mc == "-") {
		matrixCode = mc;
		matrixCodeInt = 0;
	}
	else {
		matrixCode = mc;
		// erases the - character from the matrix code
		mc.erase(2, 1);
		// converts the modified code to an int
		matrixCodeInt = stoi(mc);
	}
}
void Occupation::setOccupationType(std::string ot) {
	occupationType = ot;
}
void Occupation::setEmploymentCurrent(std::string ec) {
	employmentCurrentString = ec;
	employmentCurrent = toFloat(ec);
}
void Occupation::setEmploymentFuture(std::string ef) {
	employmentFutureString = ef;
	employmentFuture = toFloat(ef);
}
void Occupation::setDistributionCurrent(std::string dc) {
	distributionCurrentString = dc;
	distributionCurrent = toFloat(dc);
}
void Occupation::setDistributionFuture(std::string df) {
	distributionFutureString = df;
	distributionFuture = toFloat(df);
}
void Occupation::setNumericChange(std::string nc) {
	numericChangeString = nc;
	numericChange = toFloat(nc);
}
void Occupation::setPercentageChange(std::string pc) {
	percentChangeString = pc;
	percentChange = toFloat(pc);
}
void Occupation::setPercentSelfEmployed(std::string pse) {
	percentSelfEmployedString = pse;
	percentSelfEmployed = toFloat(pse);
}
void Occupation::setJobOpenings(std::string jo) {
	jobOpeningsString = jo;
	jobOpenings = toFloat(jo);
}
void Occupation::setWage(std::string w) {
	wageString = w;
	size_t tempIndex = w.find(">=$");
	if (tempIndex != std::string::npos) {
		w.erase(tempIndex, 3); // deletes >=$
		w.erase(tempIndex + 3, 1); // deletes comma
	}
	wage = toFloat(w);
}
void Occupation::setEducation(std::string e) {
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
void Occupation::setWorkExperience(std::string we) {
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
void Occupation::setTraining(std::string t) {
	training = t;
}
void Occupation::setHandbookContent(std::string hc) {
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
	float numeric = std::round((employmentFuture - employmentCurrent) * 10) / 10.0f;
	float percent =	std::round((numeric / employmentCurrent) * 100 * 10) / 10.0f;

	setNumericChange(std::to_string(numeric));
	setPercentageChange(std::to_string(percent));
}

/*
Make a new function to Generate matrix code

generateUniqueKey
promptJobAttributes
calculateOccupationChange
addEntry

Set the job index of the job when adding the job to the array.auto

Make function for rounding calculations.
*/