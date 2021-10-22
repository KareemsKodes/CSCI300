// Created By: Kareem Elkwae
// Date: 10/16/2021
// Revision: 4
// CSV Parser was taken and adopted from previous SNHU projects
// 
// The purpose of this program is to read a csv file containing course information into a data structure then perform various actions with the information in the structure based on menu input
//

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include "CSVparser.hpp"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

// structure to hold course information
struct Course {
    string courseNum; // unique identifier
    string courseName; // Name of course
    vector<string> prereqs; //vector to hold course prereqs
};

struct Node {

    Course course;

    Node* left;
    Node* right;

    Node() {
        left = nullptr;
        right = nullptr;
    }

    Node(Course course) :Node() {
        this->course = course;
    }

};

//============================================================================
// Binary Search Tree class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a binary search tree
 */
class BinarySearchTree {

private:
    

    void addNode(Node* node, Course course);

public:
    Node* root;

    BinarySearchTree();
    virtual ~BinarySearchTree();
    void Insert(Course course);
    void Remove(string courseNum);
    void displayCourse(Course course);
    void printAllCourses(Node* node);
    Course Search(string CourseNum);
    
};

//Default constructor for binary search tree
BinarySearchTree::BinarySearchTree() {
    root = nullptr; //setting root to null
}

//destructor for binary search tree
BinarySearchTree::~BinarySearchTree() {

}

//function to insert course into binary search tree
void BinarySearchTree::Insert(Course course) {

    // if root equal to null ptr
    if (root == nullptr) {
        // root is equal to new node course
        root = new Node(course);
    }
    // else
    else {
        // add Node root and course
        this->addNode(root, course);
    }

}


//function to search the binary search tree
Course BinarySearchTree::Search(string courseNum) {
    
    // set current node equal to root
    Node* currentNode = root;
    // keep looping downwards until bottom reached or matching courseNum found
    while (currentNode != nullptr) {
        // if match found, return current bid
        if (currentNode->course.courseNum.compare(courseNum) == 0) {
            return currentNode->course;
        }
        // if bid is smaller than current node then traverse left
        else if (currentNode->course.courseNum.compare(courseNum) < 0) {
            currentNode = currentNode->left;
        }
        // else larger so traverse right
        else {
            currentNode = currentNode->right;
        }

    }
    //creating empy course
    Course course;
    //returning empty course
    return course;
}

//Function to add a node to binary search tree
void BinarySearchTree::addNode(Node* node, Course course) {
    
    // if node is larger then add to left
    if (node->course.courseNum.compare(course.courseNum) > 0) {
        // if no left node
        if (node->left == nullptr) {
            // this node becomes left
            node->left = new Node(course);
        }
        // else recurse down the left node
        else {
            this->addNode(node->left, course);
        }
    }
    // else
    else {
        // if no right node
        if (node->right == nullptr) {
            // this node becomes right
            node->right = new Node(course);
        }
        //else
        else {
            //recurse down right
            this->addNode(node->right, course);
        }

    }

}

//function to print a list of courses
void BinarySearchTree::printAllCourses(Node* node) {
    //creating node to hold a current node
    Node* currentNode;

    //setting current node to supplied node
    currentNode = node;

    //if node is null, return.
    if (currentNode == nullptr) {
        return;
    }

    //recursively calling function for inorder traversal while displayin course along the way
    printAllCourses(currentNode->left);
    displayCourse(currentNode->course);
    printAllCourses(currentNode->right);

}


//Function to print menu
void printMenu() {
   
    cout << "===================================" << endl;
    cout << "Main Menu:" << endl;
    cout << "===================================" << endl;
    cout << " 1. Load Data Structure" << endl;
    cout << " 2. Print Course List" << endl;
    cout << " 3. Print Course" << endl;
    cout << " 4. Exit Program" << endl;
    cout << "===================================" << endl;
    
}

//function to take in and validate menu input
void takeMenuInput(int& userInput) {
    //Loop to validate userInput is an int
    while (true) {

        //after much searching, found that using an if statement to get input could check to make sure it was valid
        if (cin >> userInput) {
            if (userInput < 1 || userInput > 4) { //if option is not 1-4, error is displayed
                cout << "Error: Invalid input. Please enter a number corresponding to one of the 4 menu options." << endl;
            }
            else { //otherwise, if input is valid, the loop is broken and program continues
                break;
            }

        }
        else { //if the value did not pass due to it not being an int, it would clear buffer and prompt user to enter a valid option
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Error: Invalid input. Please enter a integer corresponding to one of the 4 menu options." << endl;
        }
    }
}

//function to load courses from csv file
void loadCourses(string csvPath, BinarySearchTree* bst) {
    
    cout << "Loading CSV file " << csvPath << endl;

    // initialize the CSV Parser using the given path
    csv::Parser file = csv::Parser(csvPath);

    // read and display header row - optional
    vector<string> header = file.getHeader();
    for (auto const& c : header) {
        cout << c << " | ";
    }
    cout << "" << endl;

    try {
        // loop to read rows of a CSV file
        for (unsigned int i = 0; i < file.rowCount(); i++) {

            // Create a data structure and add to the collection of bids
            Course course;
            course.courseNum = file[i][0]; //getting course num from first item in line
            course.courseName = file[i][1]; //getting course name from second item in line
            //iterating through remaining items on line
            for (unsigned int j = 2; j < file.columnCount(); ++j) {
               course.prereqs.push_back(file[i][j]); //adding next prereq to end of prereq vector
            }

            bst->Insert(course);
        }
    }
    catch (csv::Error& e) {
        std::cerr << e.what() << std::endl;
    }
}



//function to print the out course info
void BinarySearchTree::displayCourse(Course course) {
    //outputting course num and name
    cout << course.courseNum << ": " << course.courseName << " | ";

    //iterating through prereqs
    for (unsigned int i = 0; i < course.prereqs.size(); ++i) {
        
        if (course.prereqs[i].compare("2128928") == 0) {
            break;
        }
        cout << course.prereqs[i] << ' | ';
    }
    //ending with a new line
    cout << endl;
        
}

//function to print idividual course
void searchCourse(string courseNum, BinarySearchTree* courses) {
    //initializing an empty course
    Course course;

    //attempting to set course to a course from the binary search tree with course number matching provided course num
    course = courses->Search(courseNum);

    //if course number of course is empty, display error
    if (course.courseNum.empty()) {

        //displaying no course found message
        cout << "Course Number " << courseNum << " not found." << endl;
        
    }
    //else course num is not empty so displaying course
    else { 
        courses->displayCourse(course);
    }
}



int main()
{
    //initializing variables to store input
    int menuInput = 1;
    string userCourseNumber;
    string filePath;

    //initializing binary search tree memory to hold courses
    BinarySearchTree* courses;

    //creating the binary search tree
    courses = new BinarySearchTree();

    //loop to keep program going until exit option is selected
    while (menuInput != 4) {

        //displaying menu for user
        printMenu();

        takeMenuInput(menuInput);

        //switch to handle menu input
        switch (menuInput) {

        //option 1, Load Data Structure
        case 1:
            //prompting for file path from user
            cout << "Please enter the file path to course information csv:" << endl;
            cin >> filePath;

            //For testing purposes:
            //filePath = "courses.csv";

            //calling load courses function
            loadCourses(filePath, courses);
            
            //pausing until key is pressed
            system("pause");
            //clearing output window for clean output
            system("cls");
            
            break;


        //option 2, Print Course List
        case 2:
            //calling print course information csv.
            courses->printAllCourses(courses->root);

            //pausing until key is pressed
            system("pause");
            //clearing output window for clean output
            system("cls");

            break;

        //option 3, Print Course
        case 3:

            //prompting and taking in course number input from user.
            cout << "Enter course number: ";
            cin >> userCourseNumber;

            //search for and printing course based on course number supplied
            searchCourse(userCourseNumber, courses);

            //pausing until key is pressed
            system("pause");
            //clearing output window for clean output
            system("cls");
            
            break;

        //option 4, Exit 
        case 4:
            
            cout << "Exiting... ";
            
            break; //exiting switch statement. Loop condition will cause loop if option 4 is given as input.

        //just in case input doesnt get validated properly, this option will handle any improper input values.
        default:
            
            //outputting error
            cout << "Error: Please input a valid menu option." << endl;

            break;
        }
        
    }

        

}

