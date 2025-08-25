//============================================================================
// Name        : AdvisingAssistance.cpp
// Author      : Dakota Parker
// Version     : 1.0
// Description : ABCU Advising Assistance Program
//============================================================================

#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

// Handles case conversion for user input
string ToUpper(string str) {
	transform(str.begin(), str.end(), str.begin(), ::toupper);
	return str;
}

// Data structure definition to hold course information
struct Course {
	string courseId;
	string courseTitle;
	vector<string> coursePrerequisites;
};
vector<Course> courses;

// Structure for tree node
struct Node {
	Course course;
	Node* left;
	Node* right;

	// Default constructor
	Node() {
		left = nullptr;
		right = nullptr;
	}

	// Initializes with a course
	Node(Course aCourse) :
		Node() {
		course = aCourse;
	}
};

// Class definition for implementing binary search tree
class BinarySearchTree {
private:
	Node* root;

	void addNode(Node* node, Course course);
	void inOrder(Node* node);

public:
	BinarySearchTree();
	void InOrder();
	void Insert(Course course);
	Course* Search(string courseId);
};

// Default constructor
BinarySearchTree::BinarySearchTree() {
	root = nullptr;
}

// Traverse the tree in order
void BinarySearchTree::InOrder() {
	inOrder(root);
}

// Inserts a course
void BinarySearchTree::Insert(Course course) {
	if (root == nullptr) {
		root = new Node(course); // Creates a new node if root is empty
	}
	else {
		this->addNode(root, course); // Otherwise calls addNode to insert course
	}
}

// Searches for a course
Course* BinarySearchTree::Search(string courseId) {
	// Creates current node set to root, then loops to find matching courseId
	Node* current = root;
	while (current != nullptr) {
		if (current->course.courseId.compare(courseId) == 0) {
			return &current->course; // Returns mathcing course if found
		}
		// Traverses left if course is smaller than the current node
		else if (courseId.compare(current->course.courseId) < 0) {
			current = current->left;
		}
		// Traverses right if course is larger than the current node
		else {
			current = current->right;
		}
	}
	return nullptr;
}

// Adds course to some node (recursive)
void BinarySearchTree::addNode(Node* node, Course course) {
	// If node is larger than courseId, add to the left
	if (node != nullptr && node->course.courseId.compare(course.courseId) > 0) {
		if (node->left == nullptr) {
			node->left = new Node(course);
			return;
		}
		else {
			// Recursively calls addNode on the left
			this->addNode(node->left, course);
		}
	}
	// If node is smaller than courseId, add to the right
	else if (node != nullptr && node->course.courseId.compare(course.courseId) < 0) {
		if (node->right == nullptr) {
			node->right = new Node(course);
			return;
		}
		else {
			// Recursively calls addNode on the right
			this->addNode(node->right, course);
		}
	}
}

// In order traversal starting with the left, outputs course details, then goes right
void BinarySearchTree::inOrder(Node* node) {
	if (node != nullptr) {
		inOrder(node->left);
		cout << node->course.courseId << ", " << node->course.courseTitle << endl;
		inOrder(node->right);
	}
}

// Loads input file into data structure (binary search tree)
void LoadDataStructure(BinarySearchTree* bst) {
	// Prompts user for file name
	string fileName;
	cout << "Enter file name to load: ";
	cin.ignore(); // Clears leftover newline from prior input
	getline(cin, fileName); // Allows file names that have spaces

	ifstream infile(fileName);

	if (infile.is_open()) {
		string line;
		// Reads each line of the file and splits them with tokens and commas
		while (getline(infile, line)) {
			vector<string> tokens;
			string token;
			stringstream ss(line);

			while (getline(ss, token, ',')) {
				tokens.push_back(token);
			}
			if (tokens.size() < 2) {
				continue;
			}

			// Creates course object from tokens
			Course course;
			course.courseId = tokens[0];
			course.courseTitle = tokens[1];

			// Adds any prerequistes found
			for (int i = 2; i < tokens.size(); ++i) {
				course.coursePrerequisites.push_back(tokens[i]);
			}
			// Inserts course into binary search tree
			bst->Insert(course);
		}
		cout << "Data Structure loaded successfully!" << endl;
	}
	else {
		cout << "Error: File not found. Please check file path." << endl;
	}
	infile.close();
}

// Disaplys sample course list schedule, sorted alphanumerically
void PrintCourseList(BinarySearchTree* bst) {
	cout << "Here is a sample schedule: " << endl;
	bst->InOrder();
}

// Displays a single course and any prerequisites it may have
void PrintCourse(BinarySearchTree* bst) {
	string inputId;
	cout << "What course do you want to know about? ";
	cin >> inputId;

	// Converts user input to uppercase for error handling
	string courseId = ToUpper(inputId);

	// Searches for course in BST, printing course and prerequisite info if found
	Course* course = bst->Search(courseId);
	if (course != nullptr) {
		cout << course->courseId << ", " << course->courseTitle << endl;

		if (course->coursePrerequisites.empty()) {
			cout << "Prerequisites: None" << endl;
		}
		else {
			cout << "Prerequisites: ";
			for (size_t i = 0; i < course->coursePrerequisites.size(); ++i) {
				cout << course->coursePrerequisites[i];
				if (i < course->coursePrerequisites.size() - 1) {
					cout << ", ";
				}
			}
			cout << endl;
		}
	}
	else {
		cout << "Course not found." << endl;
	}
}

// Main method containing the menu the user will interact with
int main() {
	// Defines binary search tree to hold courses
	BinarySearchTree* bst = new BinarySearchTree();
	int choice = 0;

	cout << "Welcome to the course planner." << endl;

	while (choice != 9) {
		cout << "\nMenu:" << endl;
		cout << "  1. Load Data Structure" << endl;
		cout << "  2. Print Course List" << endl;
		cout << "  3. Print Course" << endl;
		cout << "  9. Exit" << endl;
		cout << "\nWhat would you like to do? ";

		cin >> choice;
		if (cin.fail()) {
			cin.clear();
			cin.ignore(1000, '\n');
			cout << "Invalid input. Please enter a number." << endl;
			continue;
		}
		switch (choice) {
		case 1:
			LoadDataStructure(bst);
			break;
		case 2:
			PrintCourseList(bst);
			break;
		case 3:
			PrintCourse(bst);
			break;
		case 9:
			cout << "Thank you for using the course planner!" << endl;
			break;
		default:
			cout << choice << " is not a valid menu option. Try again." << endl;
			break;
		}
	}
	delete bst;
}