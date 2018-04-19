//  Dalton Rick, Nathan Foster, Kush Patel, Desmond Lee, Kurtis Hoang
//
//  CIS 22C
//
//  Group Project - Leaugue of Legends Character engine
//
//  BST.h


#ifndef _BST_h
#define _BST_h

#include <cstddef>
#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;

template <class bstdata>
class BST
{
private:
    struct Node
    {
        bstdata data;
        int listIndex;
        Node* left;
        Node* right;
        
        Node(): listIndex(-1), left(NULL), right(NULL){}
        Node(bstdata newdata, int index): left(NULL), right(NULL), data(newdata), listIndex(index){}
    };
    
    typedef struct Node* Nodeptr;
    
    Nodeptr root;
    int size;
    
    /**Private helper functions*/
    
    void destructor(Nodeptr root);
    void addValue(Nodeptr root, bstdata value, int index);
    void printInOrder(Nodeptr root);
    void printPreOrder(Nodeptr root);
    void printPostOrder(Nodeptr root);
    bool containsValue(Nodeptr root, bstdata value);
    void updateValue(Nodeptr root, bstdata value, bstdata replace);
    vector<int> searchRange(Nodeptr root, bstdata min, bstdata max);
    bstdata findMin(Nodeptr root);
    bstdata findMax(Nodeptr root);
    Nodeptr removeValue(Nodeptr root, bstdata value);
    int height(Nodeptr root, int count);
    bool bstvalidate(Nodeptr root);
    
    //**Print to file
    void printInOrder(Nodeptr root, ofstream* outFile);
    void printPreOrder(Nodeptr root, ofstream* outFile);
    void printPostOrder(Nodeptr root, ofstream* outFile);
    
    
public:
    
    /**Public functions*/
    BST();
    ~BST();
    bool isEmpty();
    int getSize();
    int getHeight();
    void add(bstdata value, int index);
    bstdata getRoot();
    int getRootIndex(Nodeptr root);
    void inOrderPrint();
    void preOrderPrint();
    void postOrderPrint();
    bool contains(bstdata value);
    void containReplace(bstdata value, bstdata replace);
    vector<int> search(bstdata min, bstdata max);
    bstdata minimum();
    bstdata maximum();
    void remove(bstdata value);
    bool isBST();
   
    //**Print To File
    void inOrderPrint(ofstream* outFile);
    void preOrderPrint(ofstream* outFile);
    void postOrderPrint(ofstream* outFile);
    
};

//====================================================== constructor()
template <class bstdata>
BST<bstdata>::BST(): root(NULL), size(0){}
//====================================================== add()
template <class bstdata>
void BST<bstdata>::add(bstdata value, int index)
{
    if (root == NULL){
        root = new Node(value, index); //if the tree is empty insert the value at the root
        size++;
    }else
        addValue(root, value, index); //otherwise call the helper function, passing it the root
}
//====================================================== addValue(root, value) - private
template <class bstdata>
void BST<bstdata>::addValue(Nodeptr root, bstdata value, int index){
    
    if (value == root->data) { // no duplicate values in tree
        return;
    }else if(value < root->data){
        if (!root->left) {
            root->left = new Node(value, index);
            size++;
        }else{
            addValue(root->left, value, index);
        }
    }else if(value > root->data){
        if (!root->right) {
            root->right = new Node(value, index);
            size++;
        }else{
            addValue(root->right, value, index);
        }
        
        
    }
    
    
}
//====================================================== getRoot()
template <class bstdata>
bstdata BST<bstdata>::getRoot()
{
    if (isEmpty()) {
        cout << "\nRoot is empty!\n";
        exit(-1);
    }else{
    return root->data;
    }
}
//====================================================== getRootIndex()
template <class bstdata>
int BST<bstdata>::getRootIndex(Nodeptr root){
    if(isEmpty()){
        cout << "\nRoot is empty!\n";
        exit(-1);
    }else{
        return root->listIndex;
    }
}
//====================================================== isEmpty()
template <class bstdata>
bool BST<bstdata>::isEmpty(){
    if (size == 0) {
        return true;
    }else{
        return false;
    }
}
//====================================================== getSize()
template <class bstdata>
int BST<bstdata>::getSize(){
    return size;
}
//=================================================== printInOrder(root) - private
template <class bstdata>
void BST<bstdata>::printInOrder(Nodeptr root){
    
    if (root) {
        printInOrder(root->left);
        cout << root->data << endl;
        printInOrder(root->right);
    }
}
//====================================================== inOrder Print()
template <class bstdata>
void BST<bstdata>::inOrderPrint(){
    
    if (!isEmpty()) {
        printInOrder(root);
    }else{
        cout << "\nEmpty list!\n";
    }
}
//====================================================== printPreOrder(root) - private
template <class bstdata>
void BST<bstdata>::printPreOrder(Nodeptr root){
    
    if (root) {
        cout << root->data << endl;
        printPreOrder(root->left);
        printPreOrder(root->right);
    }
    
}
//====================================================== preOrderPrint()
template <class bstdata>
void BST<bstdata>::preOrderPrint(){
    
    if (!isEmpty()) {
        printPreOrder(root);
    }else{
        cout << "\nEmpty list!\n";
    }
}
//====================================================== printPostOrder(root) - private
template <class bstdata>
void BST<bstdata>::printPostOrder(Nodeptr root){
    
    if (root) {
        printPostOrder(root->left);
        printPostOrder(root->right);
        cout << root->data << endl;
    }
    
}
//====================================================== postOrderPrint()
template <class bstdata>
void BST<bstdata>::postOrderPrint(){
    
    if (!isEmpty()) {
        printPostOrder(root);
    }else{
        cout << "\nEmpty list!\n";
    }
    
}
//====================================================== ~BST()
template <class bstdata>
BST<bstdata>::~BST(){
    
    if (!isEmpty()) {
       destructor(root);
    }
}
//====================================================== destructor(root) - private
template <class bstdata>
void BST<bstdata>::destructor(Nodeptr root){
    
    if (root) {
        
        if(root->left)
        destructor(root->left);

        
        if(root->right)
        destructor(root->right);
        
        delete root;
    }
    
    return;
}
//====================================================== contains(value)
template <class bstdata>
bool BST<bstdata>::contains(bstdata value){
    
    if (!isEmpty()) {
       return containsValue(root, value);
    }
    else{
        return false;
    }
}
//====================================================== containReplace(value, replace)
template <class bstdata>
void BST<bstdata>::containReplace(bstdata value, bstdata replace){
    
    if(!isEmpty()){
        updateValue(root, value, replace);
    }
    
}
//====================================================== updateValue(root, value, replace)
template <class bstdata>
void BST<bstdata>::updateValue(Nodeptr root, bstdata value, bstdata replace){
    
    if(root){
        if(root->data == value)
            root->data = replace;
    }else if(value < root->data){
        return updateValue(root->left, value, replace);
    }else{
        return updateValue(root->right, value, replace);
    }
    
}

//====================================================== containsValue(root, value) - private
template <class bstdata>
bool BST<bstdata>::containsValue(Nodeptr root, bstdata value){
    
    if (root) {
        if(root->data == value){
            return true;
        }else if(value < root->data){
            return containsValue(root->left, value);
        }else{
            return containsValue(root->right, value);
        }
        
    }else{
        return false;
    }
}
//====================================================== search(min, max, indexArray)
template <class bstdata>
vector<int> BST<bstdata>::search(bstdata min, bstdata max){
    
    vector<int> index;
    
    if(!isEmpty()){
        return searchRange(root, min, max);
    }else{
        return index;
    }
}
//====================================================== searchRange(root, min, max, indexArray)
template <class bstdata>
vector<int> BST<bstdata>::searchRange(Nodeptr root, bstdata min, bstdata max){
    vector<int> index;
    
    if (root){
        if(root->data > max)
            return searchRange(root->left, min, max);
        else if(root->data < min){
            return searchRange(root->right, min, max);
        }
        else if(root->data >= min && root->data <= max){
            index.push_back(root->listIndex);
            vector<int> left = searchRange(root->left, min, max);
            vector<int> right = searchRange(root->right, min, max);
            
            index.reserve(left.size());
            index.insert(index.end(), left.begin(), left.end());
            index.reserve(right.size());
            index.insert(index.end(), right.begin(), right.end());
        }else{
            return index;
        }
    }
        return index;
    
    
    
}

//====================================================== findMin(root) - private
template <class bstdata>
bstdata BST<bstdata>::findMin(Nodeptr root){
    
    while (root->left) {
        root = root->left;
    }
    return root->data;
}
//====================================================== minimum()
template <class bstdata>
bstdata BST<bstdata>::minimum(){
    
    if (!root)
    {
        cout << "Minimum: Tree is empty" << endl;
        //will need to include cstdlib for exit below
        exit(-1); //you can also use assert here if desired
    }
    else
    {
        return findMin(root);
    }
}
//====================================================== findMax(root)
template <class bstdata>
bstdata BST<bstdata>::findMax(Nodeptr root){
    
    while (root->right) {
        root = root->right;
    }
    return root->data;
    
}
//====================================================== maximum()
template <class bstdata>
bstdata BST<bstdata>::maximum(){
    
    if (!root) {
        cout << "Maximum: Tree is empty" << endl;
        exit(-1);
    }else{
        return findMax(root);
    }
}
//====================================================== remove(value)
template <class bstdata>
void BST<bstdata>::remove(bstdata value)
{
    //add test(s) for pre-condition(s)
    if (!root) {
        cout << "Remove: Tree is empty" << endl;
        exit(-1);
    }else if(!contains(value)){
        cout << "Remove: Value not found!\n" << endl;
        exit(-1);
        }else{
    root = removeValue(root, value);
    size--;
    }
}
//====================================================== removeValue(root, value) - private
template <class bstdata>
typename BST<bstdata>::Nodeptr BST<bstdata>::removeValue(Nodeptr root, bstdata value)
{
    if (!root) {
        return root;
    }else if(value < root->data){
        root->left = removeValue(root->left, value);
    }else if(value > root->data) {
        root->right = removeValue(root->right, value);
    }else{
        
        if(!root->right && !root->left){
            delete root;
            root = NULL;
        }else if(!root->right && root->left){
            root = root->left;
            delete root->left;
            root->left = NULL;
        }else if(root->right && !root->left){
            root = root->right;
            delete root->right;
            root->right = NULL;
        }else{
            root->data = findMin(root->right);
            root->right = removeValue(root->right, root->data);
        }
    }
    
    
    
    return root;
    
    
}
//====================================================== height(root)
template <class bstdata>
int BST<bstdata>::height(Nodeptr Root, int count){
    
    int current = count;

    if (!Root->left && !Root->right) {
        return current;
    }else if (Root->right && !Root->left){
        return height(Root->right, ++current);
    }else if (Root->left && !Root->right){
        return height(Root->left, ++current);
    }else {
        current++;
        int left = height(Root->left, current);
        int right = height(Root->right, current);
        
        if (left >= right) {
            return left;
        }else{
            return right;
        }
    }
   
}
//====================================================== getHeight()
template <class bstdata>
int BST<bstdata>::getHeight(){
    
    if (!root) {
        cout << "Height: Empty tree\n";
        exit(-1);
    }else{
        int count = 0;
        count = height(root, count);
        return count;
    }
    
}
//====================================================== isBST()
template <class bstdata>
bool BST<bstdata>::isBST(){
    
    if(!root){
        cout << "BST is empty\n";
        exit(-1);
    }else{
        return bstvalidate(root);
    }
    
}
//====================================================== bstValidate(root)
template <class bstdata>
bool BST<bstdata>::bstvalidate(Nodeptr Root){
    
    
    if (!Root->left && !Root->right) {
        return true;
        
    }else if (!Root->left && Root->right){
        
        Root = Root->right;
        if (Root->right == NULL) {
            return true;
        }
        
    }else if (Root->left && !Root->right){
        Root = Root->left;
        if(Root->left == NULL){
            return true;
        }
            
    }else{
        bool left = bstvalidate(Root->left);
        bool right = bstvalidate(Root->right);
            
            if(left == true && right == true)
                return true;
        }
    return false;
}
//====================================================== preOrderPrint(outFile)
template <class bstdata>
void BST<bstdata>::preOrderPrint(ofstream* outFile){
    
    if (!isEmpty()) {
        printPreOrder(root, outFile);
    }else{
        cout << "\nEmpty list!\n";
    }
    
}
//====================================================== printPreOrder(root, outFile) - private
template <class bstdata>
void BST<bstdata>::printPreOrder(Nodeptr root, ofstream* outFile){
    
    if (root) {
        *outFile << root->data << " ";
        printPreOrder(root->left, outFile);
        printPreOrder(root->right, outFile);
    }
    
}
//=================================================== printInOrder(root, outFile) - private
template <class bstdata>
void BST<bstdata>::printInOrder(Nodeptr root, ofstream* outFile){
    
    if (root) {
        printInOrder(root->left, outFile);
        *outFile << root->data << " ";
        printInOrder(root->right, outFile);
    }
}
//====================================================== inOrder Print(outFile)
template <class bstdata>
void BST<bstdata>::inOrderPrint(ofstream* outFile){
    
    if (!isEmpty()) {
        printInOrder(root, outFile);
    }else{
        cout << "\nEmpty list!\n";
    }
}
//====================================================== printPostOrder(root, outFile) - private
template <class bstdata>
void BST<bstdata>::printPostOrder(Nodeptr root, ofstream* outFile){
    
    if (root) {
        printPostOrder(root->left, outFile);
        printPostOrder(root->right, outFile);
        *outFile << root->data << " ";
    }
    
}
//====================================================== postOrderPrint(outFile)
template <class bstdata>
void BST<bstdata>::postOrderPrint(ofstream* outFile){
    
    if (!isEmpty()) {
        printPostOrder(root, outFile);
    }else{
        cout << "\nEmpty list!\n";
    }
    
}

#endif
