/**
*
* Solution to course project # 05
* Introduction to programming course
* Faculty of Mathematics and Informatics of Sofia University
* Winter semester 2022/2023
*
* @author Natalia Nakollofski
* @idnumber 1MI8000034
* @compiler  VC
*
* * file with helper functions for maintaining datas for students 
*
*/
#include<iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <format>
using namespace std;

struct discipline {
	string disciplineName;
	int grade;
};

struct student {
	string name;
	string middlename;
	string surname;
	int faculty_number;
	int group;
	int number_of_disciplines;
	vector<discipline> disciplines;
};

double getAvg(student& student) {
	double sum = 0;

	for (discipline subject : student.disciplines) {
		sum += subject.grade;
	}

	return sum / student.disciplines.size();
}

bool isBigger(student &a, student &b, string criteria) {
	if (criteria == "facultyNum")
		return a.faculty_number > b.faculty_number;
	else if (criteria == "avgGrade")
		return getAvg(a) > getAvg(b);
}

void sortGroup(vector<student> &students, string criteria, bool asc = true) {
	if (asc) {
		for (int i = 0; i < students.size(); ++i) {
			for (int j = 0; j < students.size() - 1; ++j) {
				if (isBigger(students[j], students[j + 1], criteria)) {
					student temp = students[j];
					students[j] = students[j + 1];
					students[j + 1] = temp;
				}
			}
		}
	}
	else {
		for (int i = 0; i < students.size(); ++i) {
			for (int j = 0; j < students.size() - 1; ++j) {
				if (isBigger(students[j + 1], students[j], criteria)) {
					student temp = students[j];
					students[j] = students[j + 1];
					students[j + 1] = temp;
				}
			}
		}
	}
}

size_t split(const string& txt, vector<std::string>& strs, char ch)
{
	size_t pos = txt.find(ch);
	size_t initialPos = 0;
	strs.clear();

	// Decompose statement
	while (pos != string::npos) {
		strs.push_back(txt.substr(initialPos, pos - initialPos));
		initialPos = pos + 1;

		pos = txt.find(ch, initialPos);
	}

	// Add the last one
	strs.push_back(txt.substr(initialPos, std::min(pos, txt.size()) - initialPos + 1));

	return strs.size();
}

void initializeStudents(vector<student> &students, int groupNum) { 
	ifstream file;

	string fileName = "group" + to_string(groupNum) + ".txt";

	file.open(fileName);

	while (file.is_open()) {
		while (!file.eof()) {
			student curStudent = student();

			vector<string> studentInfo;

			string input;

			getline(file, input);
			
			split(input, studentInfo, ' ');

			curStudent.name = studentInfo[0];
			curStudent.middlename = studentInfo[1];
			curStudent.surname = studentInfo[2];
			curStudent.faculty_number = stoi(studentInfo[3]);

			auto i = studentInfo.begin() + 4;

			while (i != studentInfo.end() && *i != "") {
				discipline subject;

				subject.disciplineName = *i;
				i++;
				subject.grade = stoi(*i);
				i++;
				curStudent.disciplines.push_back(subject);
			}
			students.push_back(curStudent);
		}
		file.close();
	}		
}

void writeToFile(vector<student> students, int groupNum) {
	ofstream file;

	string fileName = "group" + to_string(groupNum) + ".txt";

	file.open(fileName);

	for (int i = 0; i != students.size(); ++i) {
		file << students[i].name << ' ' << students[i].middlename << ' ' << students[i].surname << ' ' << students[i].faculty_number;

		for (discipline subject : students[i].disciplines) {
			file << ' ' << subject.disciplineName << ' ' << subject.grade;
		}
		if(i != students.size() - 1)
			file << '\n';
	}
	file.close();
}

void deleteStudent(vector<student> &students, int facultyNum) {
	for (int i = 0; i < students.size(); ++i) {
		if (students[i].faculty_number == facultyNum) {
			students.erase(students.begin() + i);
			return;
		}
	}
}

void studentFromInput(vector<student> &students) {
	student curStudent = student();

	vector<string> studentInfo;

	string input;

	getline(cin, input);

	split(input, studentInfo, ' ');

	curStudent.name = studentInfo[0];
	curStudent.middlename = studentInfo[1];
	curStudent.surname = studentInfo[2];
	curStudent.faculty_number = stoi(studentInfo[3]);

	auto i = studentInfo.begin() + 4;

	while (i != studentInfo.end() && *i != "") {
		discipline subject;

		subject.disciplineName = *i;
		i++;
		subject.grade = stoi(*i);
		i++;
		curStudent.disciplines.push_back(subject);
	}
	students.push_back(curStudent);
}
void intializegroups(vector<vector<student>> &groups) {
	for (int i = 1; i <= 8; i++) {
	
		initializeStudents(groups[i - 1], i);
	}
}
void display(vector<student>group) {

	for (student student : group) {   
		cout << student.name << ' ' << student.middlename << ' ' << student.surname << ' ' << student.faculty_number;
		for (discipline subject:student.disciplines) {
			cout << ' ' << subject.disciplineName << ' ' << subject.grade;

		}
		cout << '\n';
	}



}

int main() {
	vector<vector<student>> groups(8, vector<student>()); 
	intializegroups(groups);
	cout << "-----STUDENT INFORMATION SYSTEM MANAGEMENT-----" << endl;
	cout << "Choose a command to continue" << endl;
		cout << "1.Insert a Student " << endl; 
		cout << "2.Display a group" << endl; 
		cout << "3.Unsubscribe a student" << endl;
	cout << "4.Sort" << endl;
	cout << "5.Display data for students of  all groups" << endl;

	int command;
	cin >> command;
	while (true) {
		if (command == 1) {
			int groupnum;
			cout << "Enter the group:";
			cin >> groupnum;
			cin.get();
			studentFromInput(groups[groupnum-1]);
			writeToFile(groups[groupnum-1], groupnum);

		}
		else if (command == 2) {
			int groupnum;
			cout << "Enter the group:";
			cin >> groupnum;
			display(groups[groupnum-1]);

		}
		else if (command == 3) {
			int groupnum;
			cout << "Enter the group:";
			cin >> groupnum;
			int id;
			cout << "Enter Faculty Number:";
			cin >> id;
			deleteStudent(groups[groupnum-1], id);
			writeToFile(groups[groupnum - 1], groupnum);


		}
		else if (command == 4) {
			int groupnum;
			cout << "Enter the group:";
			cin >> groupnum;
			cout << "1.Sort by id" << endl;
			cout << "2.Sort by average score" << endl;
			int criteria;
			cin >> criteria;

			cout << "1.Ascending Order" << endl;
			cout << "2.Descending Order" << endl;
			int  option;
			cin >> option;
			string   indicator = criteria == 1 ? "facultyNum" : "avgGrade";

			if (option == 1) {
				sortGroup(groups[groupnum-1], indicator, true);
			}
			else if (option == 2) {
				sortGroup(groups[groupnum-1], indicator, false);
			}

			writeToFile(groups[groupnum-1], groupnum);
		}
		else if (command == 5) {
			vector<student>students;
			for (int i = 1; i <= 8; i++) {
				initializeStudents(students, i);
			}

			cout << "1.Sort by id" << endl;
			cout << "2.Sort by average score" << endl;
			int criteria;
			cin >> criteria;

			cout << "1.Ascending Order " << endl;
			cout << "2.Descending Order" << endl;
			int  option;
			cin >> option;
			string   indicator = criteria == 1 ? "facultyNum" : "avgGrade";

			if (option == 1) {
				sortGroup(students, indicator, true);
			}
			else if (option == 2) {
				sortGroup(students, indicator, false);
			}
			display(students);
			
		}
		cout << "Choose a command to continue" << endl;
		cout << "1.Insert a Student " << endl; 
		cout << "2.Display a group" << endl; 
		cout << "3.Unsubscribe a student" << endl;
		cout << "4.Sort" << endl;
		cout << "5.Display data for students of  all groups" << endl;
		cin >> command;
	}
}