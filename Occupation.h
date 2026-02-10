// Occupation.h
// Darren Daniel

#pragma once
#include <string>

// Create occupations class with a private struct containing different variables
class Occupation {
private:
	struct Job {
		// default values for Job struct
		std::string occupation = "-";
		std::string matrixCode = "-";
		int matrixCodeInt = 0;
		std::string occupationType = "-";
		float employmentCurrent = -1.0f;
		std::string employmentCurrentString = "-";
		float employmentFuture = -1.0f;
		std::string employmentFutureString = "-";
		float distributionCurrent = -1.0f;
		std::string distributionCurrentString = "-";
		float distributionFuture = -1.0f;
		std::string distributionFutureString = "-";
		float numericChange = -1.0f;
		std::string numericChangeString = "-";
		float percentChange = -1.0f;
		std::string percentChangeString = "-";
		float percentSelfEmployed = -1.0f;
		std::string percentSelfEmployedString = "-";
		float jobOpenings = -1.0f;
		std::string jobOpeningsString = "-";
		float wage = -1.0f;
		std::string wageString = "-";
		std::string education = "-";
		int educationScore = 0;
		std::string workExperience = "-";
		int workExperienceScore = 0;
		std::string training = "-";
		std::string handbookContent = "-";
		int jobIndex = -1;
	} jobStruct;
public:
	// declaring getters
	std::string getOccupation() const;
	int getMatrixCodeInt() const;
	std::string getMatrixCode() const;
	std::string getOccupationType() const;
	float getEmploymentCurrent() const;
	std::string getEmploymentCurrentString() const;
	float getEmploymentFuture() const;
	std::string getEmploymentFutureString() const;
	float getDistributionCurrent() const;
	std::string getDistributionCurrentString() const;
	float getDistributionFuture() const;
	std::string getDistributionFutureString() const;
	float getNumericChange() const;
	std::string getNumericChangeString() const;
	float getPercentageChange() const;
	std::string getPercentageChangeString() const;
	float getPercentSelfEmployed() const;
	std::string getPercentSelfEmployedString() const;
	float getJobOpenings() const;
	std::string getJobOpeningsString() const;
	float getWage() const;
	std::string getWageString() const;
	std::string getEducation() const;
	int getEducationScore() const;
	std::string getWorkExperience() const;
	int getWorkExperienceScore() const;
	std::string getTraining() const;
	std::string getHandbookContent() const;
	int getJobIndex() const;

	// Declaring setters
	void setOccupation(std::string o);
	void setMatrixCode(std::string mc);
	void setOccupationType(std::string ot);
	void setEmploymentCurrent(std::string ec);
	void setEmploymentFuture(std::string ef);
	void setDistributionCurrent(std::string dc);
	void setDistributionFuture(std::string df);
	void setNumericChange(std::string nc);
	void setPercentageChange(std::string pc);
	void setPercentSelfEmployed(std::string pse);
	void setJobOpenings(std::string jo);
	void setWage(std::string w);
	void setEducation(std::string e);
	void setWorkExperience(std::string we);
	void setTraining(std::string t);
	void setHandbookContent(std::string hc);
	void setJobIndex(int i);
};

