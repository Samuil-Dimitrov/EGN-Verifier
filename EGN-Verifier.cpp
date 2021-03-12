#include <iostream>
#include <string>
#include <ctime>

#pragma warning(disable:4996)

#define EGN_SIZE 10
using namespace std;


enum EGenderType
{
	gender_type_male = 1,
	gender_type_female = 2,
	gender_type_unknown = 3
};
// 1    2      3     - index

class CPerson
{
private:
	std::string m_strName;
	std::string m_strEGN;
	EGenderType m_eGenderType;

public:
	//Конструктори - default,explicit,copy

	//default constructor
	CPerson()
	{
		m_eGenderType = EGenderType::gender_type_unknown;
	}

	//explicit constructor
	CPerson(const std::string& strName, const std::string& strEGN, EGenderType eGenderType)
	{
		m_strName = strName;
		m_strEGN = strEGN;
		m_eGenderType = eGenderType;
	}

	//copy constructor
	CPerson(const CPerson &oPerson)
	{
		m_strName = oPerson.m_strName;
		m_strEGN = oPerson.m_strEGN;
		m_eGenderType = oPerson.m_eGenderType;
	}

	//get и set ф-ции

	void SetName(const string& strName)
	{
		m_strName = strName;
	}

	void SetEGN(const string& strEGN)
	{
		m_strEGN = strEGN;
	}



	std::string GetName()const
	{
		return m_strName;
	}

	std::string GetEGN()const
	{
		return m_strEGN;
	}

	EGenderType GetGender()const
	{
		return m_eGenderType;
	}

	//print
	void Print()
	{
		std::cout << "Name: " << m_strName << endl;
		std::cout << "EGN: " << m_strEGN << endl;
		std::cout << "Gender: " << m_eGenderType << endl;
	}


	bool IsEGNValid()
	{
		if (m_strEGN.size() != EGN_SIZE)
			return false;

		if (!HasOnlyDigits())// ако връща false - 2рата проверка не е премината
			return false;

		int nDay = 0;
		int nMonth = 0;
		int nYear = 0;

		if (!GetBirthday(nDay, nMonth, nYear))
			return false;

		if (nDay < 1 && nDay > 31)
			return false;


		if (nMonth == 2)
		{
			if (nDay > 29)
				return false;

			if (nDay == 29 && !IsLeap(nYear))
				return false;
		}

		if ((nMonth == 4 || nMonth == 6 || nMonth == 9 || nMonth == 11) && nDay > 30)
			return false;

		if (!CheckAlgorithm())
			return false;

		return true;
	}


	bool GetBirthday(int &nDay, int &nMonth, int &nYear) //от ЕГН, като параметри дни, месец, година
	{
		//atoi-от const char* в int,c_str - превръща стринга в const char* - масив
		nYear = atoi(m_strEGN.substr(0, 2).c_str()); //от коя позивия, колко символа

		nMonth = atoi(m_strEGN.substr(2, 2).c_str());

		nDay = atoi(m_strEGN.substr(4, 2).c_str());

		if (nMonth >= 1 && nMonth <= 12)
		{
			nYear += 1900;
		}
		else if ((nMonth - 20) >= 1 && (nMonth - 20) <= 12)
		{
			nYear += 1800;
		}
		else if ((nMonth - 40) >= 1 && (nMonth - 40) <= 12)
		{
			nYear += 2000;
		}
		else
			return false;

		return true;
	}

	int GetPersonAge()//връща възраст
	{
		tm tmNow = GetTime();

		int nDay = 0;
		int nMonth = 0;
		int nYear = 0;

		if (!GetBirthday(nDay, nMonth, nYear))
			return 0;

		int nPersonAge = tmNow.tm_year - nYear;
		int nMonthResult = tmNow.tm_mon - nMonth;

		if (nMonth > 0)
			return nPersonAge;
		else if (nMonth < 0)
			return nPersonAge - 1;
		else // ако са равни
		{
			int nDayResult = tmNow.tm_mday - nDay;

			if (nDayResult > 0 || nDayResult == 0) //равно на нула(същия ден)
				return nPersonAge;

			return nPersonAge - 1;
		}
	}

	EGenderType GetPersonGender()
	{
		int nValidation = atoi(m_strEGN.substr(8, 1).c_str()); //взимаме 9тата цифра

		if (!(nValidation % 2)) //ако е false = дава остатък 0 => четно е мъж
			return EGenderType::gender_type_male;

		return EGenderType::gender_type_female;
	}

	std::string GenderToString() // да върне пол като стринг, а не като индекс
	{
		switch (m_eGenderType)
		{
		case EGenderType::gender_type_male:
			return "male";
		case EGenderType::gender_type_female:
			return "female";
		default:
			return "unknown";
		}
	}

	void SetGender(EGenderType eGenderType)
	{
		m_eGenderType = eGenderType;
	}

	void StringToGender(const string &strGenderType)
	{
		if (strGenderType == "male")
			m_eGenderType = EGenderType::gender_type_male;
		else if (strGenderType == "female")
			m_eGenderType = EGenderType::gender_type_female;
		else
			m_eGenderType = EGenderType::gender_type_unknown;
	}

private:
	bool HasOnlyDigits()
	{
		for (int i = 0; i < EGN_SIZE; i++)
		{
			if (!isdigit(m_strEGN[i])) //функция,която проверява дали има само числа - ако намери символ различен от число връща false
				return false;
		}

		return true;
	}

	bool IsLeap(int nYear)
	{
		if (nYear % 4 != 0 || (nYear % 100 == 0 && nYear % 400 != 0))
			return true;
		else
			return false;
	}

	tm GetTime()
	{
		time_t t = time(0);   // get time now
		tm* now = localtime(&t);

		now->tm_year += 1900;
		now->tm_mon += 1;

		return *now;
	}

	bool CheckAlgorithm()
	{
		int arrWeight[] = { 2,4,8,5,10,9,7,3,6 };

		int nSum = 0;

		for (int i = 0; i < EGN_SIZE - 1; i++)
		{
			//превръща го в масив от символи
			nSum += arrWeight[i] * atoi(m_strEGN.substr(i, 1).c_str());
		}

		int nControlDigit = nSum % 11;

		if (atoi(m_strEGN.substr(EGN_SIZE - 1, 1).c_str()) != nControlDigit)
			return false;
	}
};



int main()
{
	CPerson oPersonFirst("Ivan", "9502058541", EGenderType::gender_type_male);
	oPersonFirst.Print();

	cout << oPersonFirst.GenderToString() << endl;
	cout << oPersonFirst.GetPersonAge() << " years" << endl;
	cout << "1 for male,2 for female,3 for unknown." << oPersonFirst.GetPersonGender() << endl;
	cout << "1 for valid, 0 for not valid." << oPersonFirst.IsEGNValid() << endl;


	system("pause");
}
