// Occupation.cpp
// Darren Daniel

#include "Occupation.h"
#include "Helpers.h"

// getters return the specific data field of jobStruct
std::string Occupation::getOccupation() const {
	return jobStruct.occupation;
}
int Occupation::getMatrixCodeInt() const {
	return jobStruct.matrixCodeInt;
}
std::string Occupation::getMatrixCode() const {
	return jobStruct.matrixCode;
}
std::string Occupation::getOccupationType() const {
	return jobStruct.occupationType;
}
float Occupation::getEmploymentCurrent() const {
	return jobStruct.employmentCurrent;
}
std::string Occupation::getEmploymentCurrentString() const {
	return jobStruct.employmentCurrentString;
}
float Occupation::getEmploymentFuture() const {
	return jobStruct.employmentFuture;
}
std::string Occupation::getEmploymentFutureString() const {
	return jobStruct.employmentFutureString;
}
float Occupation::getDistributionCurrent() const {
	return jobStruct.distributionCurrent;
}
std::string Occupation::getDistributionCurrentString() const {
	return jobStruct.distributionCurrentString;
}
float Occupation::getDistributionFuture() const {
	return jobStruct.distributionFuture;
}
std::string Occupation::getDistributionFutureString() const {
	return jobStruct.distributionFutureString;
}
float Occupation::getNumericChange() const {
	return jobStruct.numericChange;
}
std::string Occupation::getNumericChangeString() const {
	return jobStruct.numericChangeString;
}
float Occupation::getPercentageChange() const {
	return jobStruct.percentChange;
}
std::string Occupation::getPercentageChangeString() const {
	return jobStruct.percentChangeString;
}
float Occupation::getPercentSelfEmployed() const {
	return jobStruct.percentSelfEmployed;
}
std::string Occupation::getPercentSelfEmployedString() const {
	return jobStruct.percentSelfEmployedString;
}
float Occupation::getJobOpenings() const {
	return jobStruct.jobOpenings;
}
std::string Occupation::getJobOpeningsString() const {
	return jobStruct.jobOpeningsString;
}
float Occupation::getWage() const {
	return jobStruct.wage;
}
std::string Occupation::getWageString() const {
	return jobStruct.wageString;
}
std::string Occupation::getEducation() const {
	return jobStruct.education;
}
int Occupation::getEducationScore() const {
	return jobStruct.educationScore;
}
std::string Occupation::getWorkExperience() const {
	return jobStruct.workExperience;
}
int Occupation::getWorkExperienceScore() const {
	return jobStruct.workExperienceScore;
}
std::string Occupation::getTraining() const {
	return jobStruct.training;
}
std::string Occupation::getHandbookContent() const {
	return jobStruct.handbookContent;
}
int Occupation::getJobIndex() const {
	return jobStruct.jobIndex;
}

// setters assign std::string arguments to std::string data fields and call toFloat() function for float data fields
void Occupation::setOccupation(std::string o) {
	jobStruct.occupation = o;
}
void Occupation::setMatrixCode(std::string mc) {
	if (mc == "-") {
		jobStruct.matrixCode = mc;
		jobStruct.matrixCodeInt = 0;
	}
	else {
		jobStruct.matrixCode = mc;
		// erases the - character from the matrix code
		mc.erase(2, 1);
		// converts the modified code to an int
		jobStruct.matrixCodeInt = stoi(mc);
	}
}
void Occupation::setOccupationType(std::string ot) {
	jobStruct.occupationType = ot;
}
void Occupation::setEmploymentCurrent(std::string ec) {
	jobStruct.employmentCurrentString = ec;
	jobStruct.employmentCurrent = toFloat(ec);
}
void Occupation::setEmploymentFuture(std::string ef) {
	jobStruct.employmentFutureString = ef;
	jobStruct.employmentFuture = toFloat(ef);
}
void Occupation::setDistributionCurrent(std::string dc) {
	jobStruct.distributionCurrentString = dc;
	jobStruct.distributionCurrent = toFloat(dc);
}
void Occupation::setDistributionFuture(std::string df) {
	jobStruct.distributionFutureString = df;
	jobStruct.distributionFuture = toFloat(df);
}
void Occupation::setNumericChange(std::string nc) {
	jobStruct.numericChangeString = nc;
	jobStruct.numericChange = toFloat(nc);
}
void Occupation::setPercentageChange(std::string pc) {
	jobStruct.percentChangeString = pc;
	jobStruct.percentChange = toFloat(pc);
}
void Occupation::setPercentSelfEmployed(std::string pse) {
	jobStruct.percentSelfEmployedString = pse;
	jobStruct.percentSelfEmployed = toFloat(pse);
}
void Occupation::setJobOpenings(std::string jo) {
	jobStruct.jobOpeningsString = jo;
	jobStruct.jobOpenings = toFloat(jo);
}
void Occupation::setWage(std::string w) {
	jobStruct.wageString = w;
	size_t tempIndex = w.find(">=$");
	// delete instances of >=$ and the comma in the wage data for jobs with >=$239,200 wage
	// code learned from geeks for geeks website and chatGPT
	if (tempIndex != std::string::npos) {
		w.erase(tempIndex, 3); // deletes >=$
		w.erase(tempIndex + 3, 1); // deletes comma
	}
	jobStruct.wage = toFloat(w);
}
void Occupation::setEducation(std::string e) {
	jobStruct.education = e;
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
	jobStruct.educationScore = score;
}
void Occupation::setWorkExperience(std::string we) {
	jobStruct.workExperience = we;
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
	jobStruct.workExperienceScore = score;
}
void Occupation::setTraining(std::string t) {
	jobStruct.training = t;
}
void Occupation::setHandbookContent(std::string hc) {
	jobStruct.handbookContent = hc;
}
void Occupation::setJobIndex(int i) {
	jobStruct.jobIndex = i;
}