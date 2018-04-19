//  Dalton Rick
//
//  CIS 22C
//
//  List.h

#ifndef ____List__
#define ____List__

#include <iostream>
#include <cstddef>
#include "assert.h"
#include "Hero.h"
using namespace std; // Less cout statement code

					 //============================= Linked List Template ============================
template <class listitem>
class List
{
private:
	struct Node
	{
		listitem data;
		Node* next;
		Node* previous;

		Node() : next(NULL), previous(NULL) {} //define our own default constructor
		Node(listitem data) : next(NULL), previous(NULL), data(data) {}
	};

	typedef struct Node* NodeRef;

	NodeRef head;
	NodeRef tail;
	NodeRef iterator; //points to one node at a time
	int size;

	void reverse(NodeRef node);
	//Helper function for the public printReverse() function.
	//Recursively prints the data in a List in reverse.

	// Functions to recursively sort list --obtained from outside sources for xtra cred
	void sort(NodeRef* headRef);
	void divideForSort(NodeRef source, NodeRef* front, NodeRef* back);
	NodeRef sorted(NodeRef left, NodeRef right);

public:
	/**Constructors and Destructors*/
	List();
	//Default constructor; initializes an empty list
	//Postcondition: int data - 0; head = null; tail = null; next - null;

	~List();
	//Destructor. Frees memory allocated to the list
	//Postcondition: head = null;

	List(const List &list);
	//Copy construcor. Initializes list to have the same elements as another list
	//Postcondition: Deep copy of List made;

	/**Accessors*/
	listitem back();
	//Returns the last element in the list
	//Precondition: Head != NULL
	//Postcondition: returned item is == to data from tail

	// ***** Cannot use current(), see linkList.cpp file for details ******** ERROR!

	listitem current();
	//Returns the element pointed to by the iterator
	//Precondition: head != NULL
	//Postcondition: data from iterator is returned

	bool empty();
	//Determines whether a list is empty.

	listitem front();
	//Returns the first element in the list
	//Precondition: head != NULL

	bool off_end();
	//Determines if the iterator is off the end of the list

	int get_size();
	//Returns the length of the list


	/**Manipulation Procedures*/

	void begin();
	//Moves the iterator to point to the first element in the list
	//If the list is empty, the iterator remains NULL
	//Postcondition: iterator == head


	void insert(listitem data);
	//Inserts a new element into the list in the position after the iterator
	//Precondition: Head != NULL
	//Postcondition: size++

	void pop_back();
	//Removes the value of the last element in the list
	//Precondition: iterator != NULL
	//Postcondition: int size--

	void pop_front();
	//Removes the value of the first element in the list
	//Precondition: head != NULL
	//Postcondition: int size--

	void push_back(listitem data);
	//Inserts a new element at the end of the list
	//If the list is empty, the new element becomes both head and tail
	//Postcondition: int size++

	void push_front(listitem data);
	//Inserts a new element at the front of the list
	//If the list is empty, the new element becomes both head and tail
	//Postcondition: int size ++

	void remove();
	//Removes the element pointed at by the iterator
	//causing iterator to go off_End
	//Precondition: iterator != NULL
	//Postcondition: iterator == NULL; size--

	void scroll();
	//Moves the iterator forward by one element in the list
	//Has no effect if the iterator is offend or the list is empty
	//Postcondition: Iterator == Iterator->next

	bool operator==(const List& list);
	//Compares one list to another to check for equality
	//Precondition: lists cannot be empty
	//Postcondition: bool true/false

	/**Additional List Operations*/

	void print();
	//Prints to the console the value of each element in the list sequentially
	//and separated by a blank space
	//Prints nothing if the list is empty

	void printReverse();
	//Calls the reverse helper function to
	//Pre: !isEmpty()

	int getIndex();
	// returns the index number for the iterator
	//Pre: !isEmpty()

	void scrollToIndex(int index);
	// Scrolls the iterator to the index point passed in
	//Pre: !isEmpty()

	int linear_search(listitem item);

	// Main sort function, calls: private sort() - obtained from external sources
	void callSort();

	int binary_search(int low, int high, listitem item);
	// Calls BinarySearch();

	void print1();

	void equipCurrent(const Equipment e);

	void dequipCurrent(int count);

	Equipment sendCurrentEquip(int count);

	void dequipAll();

	string currentEquipString(int count);

	string equipmentString();


};

template <class listitem>
List<listitem>::List() : size(0), head(NULL), tail(NULL), iterator(NULL) {}

//=========================================================== Destructor()
template <class listitem>
List<listitem>::~List()
{
	iterator = head;
	NodeRef temp;
	while (iterator != NULL)
	{
		temp = iterator->next;

		delete iterator;

		iterator = temp;

	}// End while loop
}// End List::~List()

 //=========================================================== Print()
template <class listitem>
void List<listitem>::print()
{
	iterator = head; int count = 1;
	while (iterator != NULL) {
		cout << endl << count << ".\n";
		iterator->data.print();
		iterator = iterator->next; count++;

	}// End While loop
	cout << endl << endl;
}// End List::print()
 //================================================================= Print1()
template <class listitem>
void List<listitem>::print1() {

	if (!off_end()) {


		iterator->data.print();
		cout << endl;
		iterator->data.printEquipment();


	}
	else
		cout << "\nIterator off end!\n";

	return;
}
//=========================================================== push_front()
template <class listitem>
void List<listitem>::push_front(listitem data) {

	if (size == 0)
	{
		head = new Node(data);
		iterator = tail = head;

	}
	else
	{
		NodeRef newNode = new Node(data);
		newNode->next = head;
		head->previous = newNode;
		iterator = head = newNode;
	}

	size++;

}

//=========================================================== push_back()
template <class listitem>
void List<listitem>::push_back(listitem data) {

	if (empty()) {
		head = new Node(data);
		iterator = tail = head;
	}
	else
	{

		NodeRef newNode = new Node(data);
		tail->next = newNode;
		newNode->previous = tail;
		iterator = tail = newNode;
	}
	size++;
}

//=========================================================== get_size()
template<class listitem>
int List<listitem>::get_size() {

	return size;
}

//=========================================================== back()
template <class listitem>
listitem List<listitem>::back()
{
	return tail->data;

}

//=========================================================== Off_End()
template <class listitem>
bool List<listitem>::off_end()
{
	if (iterator == NULL)
		return true;
	else
		return false;
}
//=========================================================== Copy Constructor
template <class listitem>
List<listitem>::List(const List &list) : size(list.size)
{

	NodeRef current;
	if (list.head == NULL) //If the original list is empty, make an empty list for this list
	{
		head = tail = current = NULL;
	}
	else
	{
		head = new Node;
		head->data = list.head->data;
		NodeRef temp = new Node; //Why must I create the temp Node here?
		temp = list.head;
		current = head;

		while (temp->next != NULL)
		{

			current->next = new Node;
			current = current->next;
			temp = temp->next;
			current->data = temp->data;



		}

		tail = current; //Why do I need this line of code?

	}
}

//=========================================================== pop_front()
template <class listitem>
void List<listitem>::pop_front() {

	if (empty()) //or make a call to the empty() function
	{
		cout << "Cannot call pop_front on an empty list" << endl;
	}
	else if (size == 1)
	{
		delete head;
		head = tail = iterator = NULL;

		size = 0;

	}
	else
	{
		NodeRef temp = head;
		head = head->next;
		head->previous = NULL;
		delete temp;

		size--;

	}
}

//=========================================================== pop_back()
template <class listitem>
void List<listitem>::pop_back() {

	if (empty())
	{
		cout << "Cannot call pop_back on an empty list" << endl;
	}
	else if (size == 1)
	{
		delete head;
		head = tail = iterator = NULL;
		size = 0;
	}
	else
	{

		iterator = tail->previous;
		delete tail;
		iterator->next = NULL;
		tail = iterator;

		size--;
	}
}

//=========================================================== Empty();
template <class listitem>
bool List<listitem>::empty() {

	if (size == 0)
		return true;
	else
		return false;
}

//=========================================================== front()
template <class listitem>
listitem List<listitem>::front() {
	if (empty())
	{
		cout << "Cannot call first node on empty list!" << endl;
		return 0;//xcode will not compile if a function with a return type can end without a proper return value
	}
	else
		return head->data;
}

//=========================================================== Current()
//current() **** current does not work, xcode will not allow me to compile unless i resolve the issue of current() reaching end of non void function.

template <class listitem>
listitem List<listitem>::current()
{
	if (off_end())
	{
		cout << "Current: Iterator is off the end of the list! Exiting..." << endl;
		//assert(iterator == NULL); //xcode will not allow end of non-void function cant compile without " return returntype; "

		exit(-1);
	}
	else
		return iterator->data;
}

//=========================================================== begin();
template <class listitem>
void List<listitem>::begin() {

	iterator = head;


}
//=========================================================== insert()
template <class listitem>
void List<listitem>::insert(listitem data) {

	if (get_size() == 0) {
		push_front(data);
		iterator = head;
	}
	else if (off_end()) {
		cout << "Cannot insert, iterator Off_End" << endl;
	}
	else if (get_size() == 1) {
		NodeRef newNode = new Node(data);
		iterator->next = newNode;
		newNode->previous = iterator;
		iterator = tail = newNode;

		size++;
	}

	else {
		NodeRef newNode = new Node(data);
		NodeRef node = iterator->next;
		newNode->next = node;
		newNode->previous = iterator;
		iterator->next = newNode;
		if (newNode->next == NULL)
			tail = newNode;
		else
		{
			NodeRef current = newNode->next;
			current->previous = newNode;
		}
		iterator = newNode;
		size++;
	}
}
//=========================================================== scroll();
template <class listitem>
void List<listitem>::scroll() {

	if (empty())
		cout << "Cannot scroll empty list!" << endl;

	else if (off_end())
		cout << "Cannot scroll! Iterator off_end!" << endl;
	else
		iterator = iterator->next;

}
//=========================================================== remove();
template <class listitem>
void List<listitem>::remove() {

	if (off_end()) {
		cout << endl << "Cannot delete NULL value" << endl;

	}
	else {

		if (iterator->previous == NULL) {

			head = iterator->next;
			head->previous = NULL;

		}
		else if (iterator->next == NULL) {

			tail = iterator->previous;
			tail->next = NULL;
		}
		else {
			NodeRef current;
			current = iterator->previous;
			current->next = iterator->next;

			current = iterator->next;
			current->previous = iterator->previous;
		}
		delete iterator;
		size--;

	}
}
//=========================================================== Overload == operator()
template <class listitem>
bool List<listitem>::operator==(const List& list)
{
	if (size != list.size)
		return false;
	iterator = head;
	NodeRef temp = list.head;
	while (iterator != NULL)
	{
		if (iterator->data != temp->data)
			return false;
		temp = temp->next;
		iterator = iterator->next;
	}
	return true;
}
template <class listitem>
void List<listitem>::reverse(NodeRef node) {

	if (node == NULL) {
		return;
	}

	cout << node->data << endl;
	node = node->previous;
	reverse(node);

}
template <class listitem>
void List<listitem>::printReverse() {

	NodeRef temp = tail;
	reverse(temp);

}
template <class listitem>
int List<listitem>::getIndex()
{
	NodeRef temp = head; int count = 2;

	if (get_size() < 2) {
		return get_size() + 1;
	}
	while (temp != iterator) {
		count++;
		temp = temp->next;
	}
	return count;
}
template <class listitem>
void List<listitem>::scrollToIndex(int index)
{
	if (size == 0 | (index <= -1)) {
		cout << "\nNull index!\n";
		iterator = NULL;
		return;

	}
	else {
		iterator = head;

		for (int i = 1; i < index; i++) {
			iterator = iterator->next;
		}
	}


}
template <class listitem>
int List<listitem>::linear_search(listitem item)
{

	iterator = head; int count = -1;
	int i = 1;

	while (iterator != NULL && count == -1) {
		if (iterator->data == item) {
			count = i;
		}
		else {
			iterator = iterator->next;
			i++;
		}

	}



	return count;
}
template <class listitem>
int List<listitem>::binary_search(int low, int high, listitem item)
{


	if (size == 0)
		return -1;

	//call to sort if list is not empty
	sort(&head);

	//Find halfway point
	int half = ((high - low) / 2) + low;

	//cout << "Low: " << low << " High: " << high << " Half: " << half << endl;

	scrollToIndex(half);

	if (low > high) {//if iterator gets thrown off end, or search finds nothing
		return -1;
	}
	else if (iterator->data == item)
	{
		return half;
	}
	else if (item < iterator->data)
	{
		return binary_search(low, half - 1, item);
	}
	else if (item > iterator->data)
	{
		return binary_search(half + 1, high, item);
	}
	else
		return -1;

}
template<class listitem>
void List<listitem>::callSort()
{
	sort(&head);

	// Set new tail
	head->previous = NULL;
	NodeRef temp = head;
	NodeRef last = head;


	for (int i = 1; i < size; i++) {
		temp = temp->next;
		temp->previous = last;
		last = last->next;

	}
	tail = temp;


}

template<class listitem>
void List<listitem>::sort(NodeRef* headRef)
{
	NodeRef headPtr = *headRef;
	NodeRef left = NULL;
	NodeRef right = NULL;

	// Base case -- length 0 or 1
	if ((headPtr == NULL) || (headPtr->next == NULL))
		return;

	// Split head into left and right lists
	divideForSort(headPtr, &left, &right);



	// Recursively sort the sublists
	sort(&left);
	sort(&right);

	/* answer = merge the two sorted lists together */
	*headRef = sorted(left, right);


}

template<class listitem>
void List<listitem>::divideForSort(NodeRef source, NodeRef* front, NodeRef* back)
{
	NodeRef fast = NULL;
	NodeRef slow = NULL;


	if (source == NULL || source->next == NULL)
	{
		/* length < 2 cases */
		*front = source;
		*back = NULL;
	}
	else
	{
		slow = source;
		fast = source->next;

		/* Advance 'fast' two nodes, and advance 'slow' one node */
		while (fast != NULL)
		{
			fast = fast->next;
			if (fast != NULL)
			{
				slow = slow->next;
				fast = fast->next;
			}
		}

		/* 'slow' is before the midpoint in the list, so split it in two
		at that point. */
		*front = source;
		*back = slow->next;
		slow->next = NULL;
	}
}

template<class listitem>
typename List<listitem>::NodeRef List<listitem>::sorted(NodeRef left, NodeRef right)
{
	NodeRef result = NULL;

	/* Base cases */
	if (left == NULL)
		return(right);
	else if (right == NULL)
		return(left);

	/* Pick either left or right, and recur */
	if (left->data <= right->data)
	{
		result = left;
		result->next = sorted(left->next, right);
	}
	else
	{
		result = right;
		result->next = sorted(left, right->next);
	}
	return result;
}
template <class listitem>
void List<listitem>::equipCurrent(const Equipment e) {

	iterator->data.setEquipment(e);


	//iterator->data.print();

	//iterator->data.printEquipment();
}

template<class listitem>
void List<listitem>::dequipCurrent(int count) {

	iterator->data.removeEquipment(count);

	// iterator->data.print();

	//iterator->data.printEquipment();



}
template <class listitem>
Equipment List<listitem>::sendCurrentEquip(int count) {

	return iterator->data.sendInventory(count);

}
template <class listitem>
void List<listitem>::dequipAll() {

	begin();

	while (!off_end()) {
		Hero tempChamp = iterator->data;
		if (tempChamp.getEquippedCount() != 0) {
			//cout << "InventoryCount: " << tempChamp.getEquippedCount() << endl;
			for (int i = 1; i <= tempChamp.getEquippedCount(); i++) {
				iterator->data.removeEquipment(1);
			}
		}

		scroll();
	}


}
template <class listitem>
string List<listitem>::currentEquipString(int count) {

	Equipment temp = iterator->data.sendInventory(count);

	return temp.getDataString();
}
template <class listitem>
string List<listitem>::equipmentString() {

	if (!off_end()) {
		return iterator->data.getDataString();
	}
	else
		return "\nequipmentString: iterator off end!";
}

#endif /* defined(____linkList__) */


