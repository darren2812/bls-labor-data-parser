#pragma once
#include <string>

struct OccupationRow {
	std::string occupation = "-";
	std::string matrixCode = "-";
	std::string occupationType = "-";
	std::string employmentCurrent = "-";
	std::string employmentFuture = "-";
	std::string distributionCurrent = "-";
	std::string distributionFuture = "-";
	std::string numericChange = "-";
	std::string percentageChange = "-";
	std::string percentSelfEmployed = "-";
	std::string jobOpenings = "-";
	std::string wage = "-";
	std::string education = "-";
	std::string workExperience = "-";
	std::string training = "-";
	std::string handbookContent = "-";
};

class Occupation {
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

public:
	Occupation() {
		if (employmentCurrentString == "-" || employmentFutureString == "-") {
			calculateChanges();
		}
	};

	// some data fields have string and corresponding numerical return types.
	// This is due to some blank data from the original website. In this implementation,
	// non-existent data is equated to a 0, but will be displayed as a "-" on the console output.

	std::string getOccupation() const;
	int getMatrixCodeInt() const;
	std::string getMatrixCode() const;
	std::string getMatrixPrefix() const;
	std::string getMatrixSuffix() const;
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

	void setOccupation(const std::string &o);
	void setMatrixCode(const std::string &mc);
	void setOccupationType(const std::string &ot);
	void setEmploymentCurrent(const std::string &ec);
	void setEmploymentFuture(const std::string &ef);
	void setDistributionCurrent(const std::string &dc);
	void setDistributionFuture(const std::string &df);
	void setNumericChange(const std::string &nc);
	void setPercentageChange(const std::string &pc);
	void setPercentSelfEmployed(const std::string &pse);
	void setJobOpenings(const std::string &jo);
	void setWage(const std::string &w);
	void setEducation(const std::string &e);
	void setWorkExperience(const std::string &we);
	void setTraining(const std::string &t);
	void setHandbookContent(const std::string &hc);
	void setJobIndex(int i);

	// functions for calculations
	void calculateChanges();
	static std::string formatOneDecimal(float& value);
};

