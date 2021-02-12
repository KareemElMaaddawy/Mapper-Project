/* 
 * File:   TrieNode.cpp
 * Author: ahmadda4
 * 
 * Created on February 7, 2021, 6:33 PM
 */

#include "StreetsDatabaseAPI.h"
#include "TrieNode.h"
#include <iostream>
#include <vector>
#define SIZE 128 //ascii table size
 
struct TrieNode{
    struct TrieNode *children[SIZE]; //creates child for every character in an ASCII table
    
    bool endOfName; //holds whether end of street name reached
    std::vector<StreetIdx> streetIndices; //holds idx of every street the current path covers
};

struct TrieNode *makeNode(StreetIdx strtIdx){ //add new node w value for streetidx
    struct TrieNode *ptr = new TrieNode;
    
    ptr->streetIndices.push_back(strtIdx); //adds to end of vector
    
    for(int i = 0; i < SIZE; i++) ptr->children[i] = nullptr;
    
    return ptr;
}

struct TrieNode *makeNode(){ //makes root node
    struct TrieNode *ptr = new TrieNode;
    
    for(int i = 0; i < SIZE; i++) ptr->children[i] = nullptr;
    
    return ptr;
}

void destroyTrie(TrieNode *root){//dealloc trie
    for(int i = 0; i < SIZE; i++){
        if(root->children[i] != nullptr){//recursively calls children that arent nullptrs
            destroyTrie(root->children[i]);
        }
    }
    delete root;//deletes nodes
    root = nullptr;
} 

void insertToTrie(struct TrieNode *root, std::string key, StreetIdx idx){ //inserts streetname into trie
    for(int i = 0; i < key.length(); i++){
        if(int(key[i]) - 1  == -30){
            std::cout << "error: char " << key[i] << " index: " << int(key[i]) - 1 << " shit prefix: " << key << std::endl;
        }
        if(root->children[abs(int(key[i]))] == nullptr){//if char not already included in trie makes new node for it
            root->children[abs(int(key[i]))] = makeNode(idx);
        }else{
            root->children[abs(int(key[i]))]->streetIndices.push_back(idx);//if already included adds streetidx to node
        } 
        
        root = root->children[abs(int(key[i]))]; //travels to next node
    }
}

std::vector<StreetIdx> findStreetName(struct TrieNode *root, std::string prefix){ //finds which streetnames include the prefix
    for(int i = 0; i < prefix.length(); i++){
        if(root->children[abs(int(prefix[i]))] == nullptr){ //reached point where prefix is not included in trie
            return {}; //returns empty vector
        }else{
            root = root->children[abs(int(prefix[i]))]; //if char found travels to next node
        }
    }
    
    return root->streetIndices;//returns all included streetindices if end of prefix reached and is all included
}