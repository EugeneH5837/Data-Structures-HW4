#include <iostream>
#include "ArgumentManager.h"
#include "LinkedList.h"
#include "Stack.h"
#include <string>
#include <fstream>
#include <istream>
#include <ostream>
#include <new>
using namespace std;
int prec(char c);
void makelowercase(string &input);
void read(string thename, string file);
void write(string thename, string thefile);
void infixToPostfix(string &s);
bool checkifallexists(string a);
bool isOperator(char currentChar);
bool checkforlist(string list);
bool detectinvalid(string expression);
void getunion(LinkedList<string> arr1, LinkedList<string> arr2, LinkedList<string> &output);
void getintersection(LinkedList<string> first, LinkedList<string> second, LinkedList<string> &output);
void putintoarray(string *thearray, LinkedList<string> list);
void evalpostfixover(string postfix, string var);
void sortLL(LinkedList<string> &list);
void MergeSort(string a[], int low, int high);
void evalassign(LinkedList<string> a, LinkedList<string> &b);
void Merge(string a[], int low, int high, int mid);
bool recurBinarySearch(string sortedArray[], int first, int last, string key);
void evaluatepostfix(string postfix, string var);
void skipdup(LinkedList<string> &list);
void eval(LinkedList<string> op1, LinkedList<string> op2, LinkedList<string> &output, char operate);
bool checkifnooperators(string expression);
void overwrite(LinkedList<string> &a, LinkedList<string> b);
LinkedList<string> findLL(string name);
LinkedList<string>& findLLref(string name);
LinkedList<LinkedList<string>> Inputlist;
bool overwriteeq = false;
int main(int argc, char* argv[]) {

	if (argc < 2) {
		std::cerr << "Usage: spellchecker inputfile=input.txt" << std::endl;
		return -1;
	}
	ArgumentManager am(argc, argv);
	const std::string script = am.get("script");
	std::cout << "input script file name is " << script << std::endl;

	ifstream input(script);
	//ifstream input("input.txt");
	string str;
	string cmd, variname, file;
	while (getline(input, str)) {
		if (str.substr(0, 4) == "read"&&str.find_first_of("=") == -1) {
			for (int i = 0; i < str.length(); i++) {
				if (str[i] == '(' || str[i] == ')' || str[i] == ',') {
					str[i] = ' ';
				}
			}
			stringstream test(str);
			test >> cmd >> variname >> file;
			read(variname, file);
		}
		else if (str.substr(0, 5) == "write"&&str.find_first_of("=") == -1) {
			for (int i = 0; i < str.length(); i++) {
				if (str[i] == '(' || str[i] == ')' || str[i] == ',') {
					str[i] = ' ';
				}
			}
			stringstream test(str);
			test >> cmd >> variname >> file;
			write(variname, file);
		}
		else {

			if (detectinvalid(str)) {
				
				string var, exp, eq;
				int found = str.find_first_of("=");
				for (int i = 0; i < str.length(); i++) {
					if (str[i] == '=') {
						var = str.substr(0, found);
					}
				}
				eq = str.substr(found + 1, str.length());
				//assignment
				if (checkifallexists(eq)==true) {
					if (!checkifnooperators(eq) && !checkforlist(var)) {
						evaluatepostfix(eq, var);
					}
					//overwrite
					else if (checkforlist(var)) {
						if (checkifnooperators(eq)) {
							infixToPostfix(eq);
							overwriteeq = true;
							evalpostfixover(eq, var);

						}
						else {
							evaluatepostfix(eq, var);
						}
					}
					//expression
					else {
						infixToPostfix(eq);
						evaluatepostfix(eq, var);
					}

				}

			}
			
		}
	}
	

	return 0;

}


//used in reading in expressions to see that if it is an overwrite or assignment expression
bool checkifnooperators(string expression) {
	bool check = false;
	for (int i = 0; i < expression.length(); i++) {
		if (expression[i] == '*' || expression[i] == '+') {
			check = true;
		}
	}
	return check;
}

//makes sure that all variables read exists in order to determine if it is a valid expression
bool checkifallexists(string a) {
	bool check = true;
	string var, exp, eq;
	int found = a.find_first_of("=");
	for (int i = 0; i < a.length(); i++) {
		if (a[i] == '=') {
			var = a.substr(0, found);
		}
	}
	eq = a.substr(found + 1, a.length());
	infixToPostfix(eq);
	int j = 0;
	string ch;
	while (j < eq.length()) {
		if (!isOperator(eq[j]) && !isspace(eq[j])) {
			ch += eq[j];
		}
		else if (!ch.empty()) {

			if (!checkforlist(ch)) {

				check = false;
			}
			ch.clear();
		}

		j++;
	}
	return check;
}

//checks for a list
bool checkforlist(string list) {
	bool check = false;
	Node<LinkedList<string>> *current = Inputlist.head;
	while (current != NULL) {
		if (list == current->info.name) {
			check = true;
		}
		current = current->next;
	}
	delete current;
	return check;
}

//function used to determine if the expression read is valid or not, if not it appends to log file the statement INVALID EXPRESSION and what the expression was
bool detectinvalid(string expression) {
	ofstream file("log.txt", ios::app);
	int eqtn = 0;
	bool check = true;
	int balanced = 0;
	for (int i = 0; i < expression.length(); i++) {

		if (expression[i] == '(') {
			if (expression[i + 1] == '+' || expression[i + 1] == '*' || expression[i + 1] == '=' || expression[i + 1] == ')') {
				check = false;
			}
			balanced++;

		}

		else if (expression[i] == ')') {
			if (expression[i + 1] == '=') {
				check = false;
			}
			balanced--;
		}
		else if (expression[i] == '+' || expression[i] == '*') {
			if (expression[i + 1] == '+' || expression[i + 1] == '*' || expression[i + 1] == '=' || expression[i + 1] == '\0') {
				
				check = false;
			}
		}
		else if (expression.find_first_of("=") == -1) {
			
			check = false;
		}
		else if (expression[i] == '=') {

			eqtn++;
			if (expression[i + 1] == '+' || expression[i + 1] == '*' || expression[i + 1] == '=') {
				check = false;
			}
		}

	}
	if (balanced != 0) {

		//file << "INVALID EXPRESSION: " << expression << endl;
		check = false;
	}
	else if (eqtn != 1) {
		//file << "balanced wrong" << expression<<endl;
		check = false;
	}
	if (!check) {
		file << "INVALID EXPRESSION: " << expression << endl;
	}
	return check;
}

//if in postfix it reads the + operand, then it performs union by converting the LinkedLists to arrays and then inputting the new array into a new linkedlist with the name given
void getunion(LinkedList<string> arr1, LinkedList<string> arr2, LinkedList<string> &output) {
	//smaller first
	LinkedList<string> final;
	int size1 = arr1.count;
	int size2 = arr2.count;
	int counter = size1;
	string *arr = new string[size1];
	string *secondarr = new string[size2];
	putintoarray(arr, arr1);
	putintoarray(secondarr, arr2);



	MergeSort(arr, 0, size1 - 1);
	
	for (int i = 0; i < size1; i++) {
		//final.insertion(arr[i]);
		output.insertion(arr[i]);
	}



	for (int i = 0; i < size2; i++) {
		if (!recurBinarySearch(arr, 0, size1 - 1, secondarr[i])) {
			output.insertion(secondarr[i]);
			//final.insertion(secondarr[i]);
		}

	}

	delete[] arr;
	delete[] secondarr;
	//output = final;
	//final.destroylist();
}

//if in postfix it reads the * operand, then it performs intersection by converting the LinkedLists to arrays and then inputting the new array into a new linkedlist with the name given
void getintersection(LinkedList<string> first, LinkedList<string> second, LinkedList<string> &output) {
	LinkedList<string> final;
	int size1 = first.count;
	int size2 = second.count;
	string *arr = new string[size1];
	string *secondarr = new string[size2];
	putintoarray(arr, first);
	putintoarray(secondarr, second);
	MergeSort(arr, 0, size1 - 1);


	for (int i = 0; i < size2; i++) {
		if (recurBinarySearch(arr, 0, size1 - 1, secondarr[i])) {
			final.insertion(secondarr[i]);
		}
	}
	delete[] arr;
	delete[] secondarr;
	output = final;
	//return final;
}

//recursive binary search, same as from previous HW
bool recurBinarySearch(string sortedArray[], int first, int last, string key) {
	int mid;

	if (first > last) {

		return false;
	}
	if (first <= last) {

		mid = (first + last) / 2;  // compute mid point.

		if (key == sortedArray[mid]) {

			return true;   // found it.
		}
		else if (key < sortedArray[mid]) {


			// first half of array
			return recurBinarySearch(sortedArray, first, mid - 1, key);

		}
		else {

			// other half of array
			return recurBinarySearch(sortedArray, mid + 1, last, key);

		}
	}

	return false;    // not in array
}

//function used to read the variable specified from the file specified, and given the name that isread
void read(string thename, string thefile) {
	LinkedList<string> temp;
	thefile[0] = ' ';
	thefile[thefile.length() - 1] = ' ';
	stringstream test(thefile);
	test >> thefile;
	temp.name = thename;
	temp.putwordsintolist(thefile);
	Inputlist.insertion(temp);
}
//function used to write the variable specified to the file specified
void write(string thename, string thefile) {
	Node<LinkedList<string>> *current = Inputlist.head;
	thefile[0] = ' ';
	thefile[thefile.length() - 1] = ' ';
	stringstream test(thefile);
	test >> thefile;
	while (current != NULL) {
		if (current->info.name == thename) {
			skipdup(current->info);
			current->info.writetofile(thefile);
		}
		current = current->next;
	}

	delete current;

}

//used for overwriting variables that are already exist
void overwrite(LinkedList<string> &a, LinkedList<string> b) {
	a = b;
}

//used when writing to file, makes sure it is sorted and then skips duplicates when writing
void skipdup(LinkedList<string> &list) {
	LinkedList<string> temp;
	string *arr = new string[list.count];
	putintoarray(arr, list);
	MergeSort(arr, 0, list.count - 1);
	for (int i = 0; i < list.count; i++) {
		if (i == 0) {
			temp.insertion(arr[i]);
		}
		else if (arr[i] != arr[i - 1]) {
			temp.insertion(arr[i]);
		}
	}
	list = temp;


}

//mergesort list, format from internet and from last homework
void MergeSort(string a[], int low, int high) {

	int mid;
	if (low < high)
	{

		mid = (low + high) / 2;

		//splits in half
		MergeSort(a, low, mid);

		MergeSort(a, mid + 1, high);

		//put the two halves back together, merge
		Merge(a, low, high, mid);

	}
}
void Merge(string a[], int low, int high, int mid) {

	int i = low;
	int k = 0;
	int j = mid + 1;
	string *temp = new string[high - low + 1];
	//merge both halves into temp
	while (i <= mid && j <= high) {
		if (a[i] < a[j]) {
			temp[k] = a[i];
			k++;
			i++;
		}
		else {
			temp[k] = a[j];
			k++;
			j++;
		}
	}

	//first half into temp
	while (i <= mid) {
		temp[k] = a[i];
		k++;
		i++;
	}

	//second half into temp
	while (j <= high) {
		temp[k] = a[j];
		k++;
		j++;
	}


	//put temp values into a
	for (i = low; i <= high; i++) {
		a[i] = temp[i - low];
	}
}
//takes elements from linked list and converts it into an array of the correct size
void putintoarray(string *thearray, LinkedList<string> list) {
	Node<string> *temp = list.head;
	int i = 0;
	while (temp != NULL&&i<list.count) {
		thearray[i] = temp->info;
		i++;
		temp = temp->next;
	}

	//delete temp;
}
//mergesort elements in a linked list by passing to array 
void sortLL(LinkedList<string>& list)
{
	LinkedList<string> temp;
	string *arr = new string[list.count];

	putintoarray(arr, list);


	MergeSort(arr, 0, list.count - 1);


	for (int i = 0; i < list.count; i++) {
		temp.insertion(arr[i]);
	}
	list = temp;


}

//converts word to all lowercase
void makelowercase(string &input) {
	for (int i = 0; i < input.length(); i++) {
		input[i] = tolower(input[i]);
	}
}

//format from internet to convert to postfix
void infixToPostfix(string &s)
{
	MyStack<char> st;
	st.push('N');
	int l = s.length();
	string ns;
	for (int i = 0; i < l; i++)
	{
		//if an operand is read, add it to the output
		if ((s[i] >= 'a' && s[i] <= 'z') || (s[i] >= 'A' && s[i] <= 'Z') || (s[i] >= '0' && s[i] <= '9')) {
			ns += s[i];

		}
		//pushes ( to stack

		else if (s[i] == '(') {
			st.push('(');

		}
		//if reads ) then it pops until it enocunters (
		else if (s[i] == ')')
		{
			ns += " ";
			while (st.gettop() != 'N' && st.gettop() != '(')
			{
				char c = st.gettop();
				st.pop();
				ns += c;
			}
			if (st.gettop() == '(')
			{
				char c = st.gettop();
				st.pop();
			}
		}

		//checks if operator 
		else {
			ns += " ";
			while (st.gettop() != 'N' && prec(s[i]) <= prec(st.gettop()))
			{
				char c = st.gettop();
				st.pop();
				ns += c;
			}
			st.push(s[i]);
		}

	}
	//pop remaining from stack
	while (st.gettop() != 'N')
	{
		char c = st.gettop();
		st.pop();
		ns += " ";
		ns += c;
	}
	s = ns;

}

int prec(char c)
{
	if (c == '^')
		return 3;
	else if (c == '*' || c == '/')
		return 2;
	else if (c == '+' || c == '-')
		return 1;
	else
		return -1;
}

//evalutes the postfix expression, some parts from internet
void evaluatepostfix(string postfix, string var) {
	bool overwriting = false;
	MyStack<string> s;
	LinkedList<string> final;
	string the = var;
	int i = 0;
	string ch;
	int val;
	int b = rand() % 1000;
	string p = to_string(b);
	if (!overwriteeq) {
		while (i < postfix.length()) {

			if (!isOperator(postfix[i]) && !isspace(postfix[i]) && postfix[i] != '=') {
				ch += postfix[i];
			}

			else if (!ch.empty()) {
				s.push(ch);
				ch.clear();
			}

			else if (isOperator(postfix[i])) {
				string first = s.gettop();
				LinkedList<string> opone = findLL(first);
				opone.name = first;
				s.pop();
				string second = s.gettop();
				LinkedList<string> optwo = findLL(second);
				optwo.name = second;
				s.pop();
				if (opone.count < optwo.count) {
					eval(opone, optwo, final, postfix[i]);
				}
				else {
					eval(optwo, opone, final, postfix[i]);
				}
				
				final.name = first + second + p;
				Inputlist.insertion(final);
				s.push(final.name);
			}

			if ((!checkifnooperators(postfix)) && ((ch == postfix) || (ch == var))) {

				if (checkforlist(var) && checkforlist(postfix)) {

					overwriting = true;

					s.push(ch);

					string first = s.gettop();
					s.pop();
					s.push(var);
					string second = s.gettop();
					overwrite(findLLref(second), findLLref(first));
				}
				else if (!checkforlist(var)) {

					overwriting = false;
					s.push(ch);
					string first = s.gettop();
					LinkedList<string> opone = findLL(first);
					opone.name = first;
					s.pop();
					final.copylist(opone);
					final.name = first + var;
				}
			}


			i++;

		}

	}
	else {
		//cout << "overwrite exp " << postfix << endl;
		//while (i < postfix.length()) {
		//	if (!isOperator(postfix[i]) && !isspace(postfix[i]) && postfix[i] != '=') {
		//		ch += postfix[i];
		//		
		//	}
		//	else if (!ch.empty()) {
		//		s.push(ch);
		//		ch.clear();
		//	}
		//	else if (isOperator(postfix[i])) {
		//		//cout << postfix[i] << "is op" << endl;
		//		string first = s.gettop();
		//		cout << "looking for " << first << endl;
		//		LinkedList<string> opone = findLL(first);
		//		opone.name = first;
		//		s.pop();
		//		string second = s.gettop();
		//		cout << "looking for " << second << endl;
		//		LinkedList<string> optwo = findLL(second);
		//		optwo.name = second;
		//		s.pop();
		//		cout << "found" << opone.name << " " << optwo.name << endl;
		//		eval(opone, optwo, final, postfix[i]);
		//		final.name = first + second + p;
		//		//cout << "Inserting" << final.name << endl;
		//		Inputlist.insertion(final);
		//		//cout << "breaks here?" << endl;
		//		s.push(final.name);
		//	}
		//	i++;
		//}

	}
	if (overwriting == false||overwriteeq==true) {
		final.name = var;
		Inputlist.insertion(final);
	}


}

void evalpostfixover(string postfix, string var) {
	
	MyStack<string> s;
	LinkedList<string> final;
	string the = var;
	int i = 0;
	string ch;
	int val;
	int b = rand() % 1000;
	string p = to_string(b);
	while (i < postfix.length()) {
		//cout << "here" << endl;
		if (!isOperator(postfix[i]) && !isspace(postfix[i]) && postfix[i] != '=') {
			ch += postfix[i];
		}

		else if (!ch.empty()) {
			s.push(ch);
			ch.clear();
		}

		else if (isOperator(postfix[i])) {
;
			string first = s.gettop();
			LinkedList<string> opone = findLL(first);
			opone.name = first;
			s.pop();
			string second = s.gettop();
			LinkedList<string> optwo = findLL(second);
			optwo.name = second;
			s.pop();
			if (opone.count < optwo.count) {
				eval(opone, optwo, final, postfix[i]);
			}
			else {
				eval(optwo, opone, final, postfix[i]);
			}
			final.name = first + second + p;
			Inputlist.insertion(final);
			s.push(final.name);
		}
		i++;
	}
	final.name = var;
	Inputlist.insertion(final);
	
	string first = s.gettop();
	s.pop();
	s.push(var);
	string second = s.gettop();
	s.pop();
	overwrite(findLLref(second), findLLref(first));
}
//used for assignment operator
void evalassign(LinkedList<string> a, LinkedList<string> &b) {
	string *arr = new string[a.count];
	putintoarray(arr, a);
	for (int i = 0; i < a.count; i++) {
		b.insertion(arr[i]);
	}
}
//when an operator is read from the stack, it checks which one it is and evaluates it to give the correct output
void eval(LinkedList<string> op1, LinkedList<string> op2, LinkedList<string> &output, char operate) {

	if (operate == '*') {
		getintersection(op1, op2, output);
	}
	else if (operate == '+') {

		getunion(op1, op2, output);
	}

}
//checks if what is passed is an operator or not
bool isOperator(char currentChar)
{
	if (currentChar == '*' || currentChar == '+') {
		return true;
	}
	else {
		return false;
	}
}
//looks in the Linked List of Linked Lists for a specific linked list
LinkedList<string> findLL(string name) {
	Node<LinkedList<string>> *current = Inputlist.head;
	while (current != NULL) {

		if (current->info.name == name) {

			return (current->info);
		}
		current = current->next;

	}
}
//used for overwriting previously existing variables
LinkedList<string>& findLLref(string name)
{
	Node<LinkedList<string>> *current = Inputlist.head;
	while (current != NULL) {
		if (current->info.name == name) {
			return (current->info);
		}
		current = current->next;
	}
}
//writes linked list to file
template <class T>
void LinkedList<T>::writetofile(string filename) {
	ofstream file(filename);
	Node<T> *current;
	current = head;
	while (current != NULL) {
		file << current->info << endl;
		current = current->next;
	}

}
//copy function for LinkedList
template<class T>
void LinkedList<T>::copylist(LinkedList<T>& otherlist)
{
	Node<T> *current = otherlist.head;
	otherlist.count = count;
	otherlist.name = name;
	for (current; current; current = current->next) {
		insertion(current->info);
	}
}
//overridden = operator
template<class T>
LinkedList<T>& LinkedList<T>::operator=(LinkedList<T> &otherlist)
{
	destroylist();
	Node<T> *current = otherlist.head;
	otherlist.count = count;
	otherlist.name = name;
	for (current; current; current = current->next) {
		insertion(current->info);
	}
	return *this;

}

//Default constructor
template <class T>
LinkedList<T>::LinkedList() {
	head = NULL;
	tail = NULL;
	count = 0;
}

template<class T>
std::string LinkedList<T>::getname()
{
	return name;
}


//Insertion function to put words into nodes of doubly linked list and doubly linked lists into the doubly linked list, from previous hw
template <class T>
void LinkedList<T>::insertion(T item) {
	Node<T> *temp = new Node<T>(item);

	if (tail == NULL) {
		temp->next = tail;
		temp->prev = head;

		head = temp;
		tail = temp;
	}
	else {
		temp->prev = tail;
		tail->next = temp;
		tail = temp;
	}


	//gets the number of bytes
	count++;
}

//overwritten << operator in order to display the inner linked lists' nodes
template <class T>
ostream &operator << (ostream &strm, LinkedList<T> InnerList)
{
	Node<T> *current = InnerList.tail;
	while (current != NULL)
	{
		strm << current->info << " ";
		current = current->prev;
	}
	return strm;
}


template<class T>
void LinkedList<T>::overwriteLL(LinkedList<T> &otherlist)
{
	Node<T> *current;
	Node<T> *temp = otherlist.head;
	head = new Node<T>();
	head->info = temp->info;
	head->next = NULL;
	current = head;
	temp = temp->next;
	while (temp != NULL) {
		current->next = new Node<T>();
		current = current->next;
		current->info = temp->info;
		current->next = NULL;
		temp = temp->next;
	}

}
//used to display LL
template <class T>
void LinkedList<T>::display() {
	Node<T> *current;
	current = head;
	while (current != NULL) {
		//cout << current->info.name << endl;
		cout << current->info.name << endl;
		cout << current->info << endl;
		current = current->next;
	}
}



//destroys list
template<class T>
void LinkedList<T>::destroylist()
{
	Node<T> *temp;
	while (head != NULL) {
		temp = head;
		head = head->next;
		delete temp;
	}
	tail = NULL;
	count = 0;
	//name.clear();
}
//from previous HW, used to put only words from file in a linked list
template<class T>
void LinkedList<T>::putwordsintolist(std::string filename) {
	string line;
	string holder;
	ifstream input(filename);
	while (input >> line) {
		makelowercase(line);
		for (int i = 0; i < line.length(); i++) {
			if (isalpha(line[i])) {
				holder += line[i];
			}

			else if (!holder.empty()) {

				insertion(holder);
				holder.clear();


			}
		}
		if (!holder.empty()) {

			insertion(holder);

		}
		holder.clear();

	}
}

template <class T>
void MyStack<T>::push(T val)
{
	top++;
	if (top < thesize) {
		arr[top] = val;
	}
	else {
		cout << "Stack overflow" << endl;
		top--;
	}
}

template<class T>
T MyStack<T>::pop() {
	if (isEmpty()) {
		cout << "Stack Empty" << endl;
		return 0;
	}
	else if (isFull()) {
		cout << "Stack full" << endl;
		return 0;
	}
	else {
		T temp = arr[top];
		top--;
		return temp;
	}
}

template <class T>
T MyStack<T>::gettop()
{
	return arr[top];
}

template <class T>
bool  MyStack<T>::isEmpty() {
	if (top == -1) {
		return true;
	}
	else {
		return false;
	}
}

template <class T>
bool  MyStack<T>::isFull() {
	if (top == thesize) {
		return true;
	}
	else {
		return false;
	}
}