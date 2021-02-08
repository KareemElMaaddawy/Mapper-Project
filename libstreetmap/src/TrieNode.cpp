/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TrieNode.cpp
 * Author: ahmadda4
 * 
 * Created on February 7, 2021, 6:33 PM
 */

#include "StreetsDatabaseAPI.h"
#include <iostream>
#include <vector>
#define SIZE 26 //alphabet size
 
struct TrieNode{
    struct TrieNode *children[SIZE]; //creates child for every letter of alphabet
    
    bool endOfName; //holds whether end of street name reached
    std::vector<StreetIdx> streetIndices; //holds idx of every street the current path covers
};

struct TrieNode *makeNode(StreetIdx strtIdx){ //add new node w value for streetidx
    struct TrieNode *ptr = new TrieNode;
    
    ptr->endOfName = false;
    ptr->streetIndices.push_back(strtIdx); //adds to end of vector
    
    for(int i = 0; i < SIZE; i++) ptr->children[i] = nullptr;
    
    return ptr;
}

struct TrieNode *makeNode(){ //makes root node
    struct TrieNode *ptr = new TrieNode;
    
    ptr->endOfName = false;
    
    for(int i = 0; i < SIZE; i++) ptr->children[i] = nullptr;
    
    return ptr;
}

int charToIndex(char c){ //converts from char to index value, a = 1, b = 2, .....
    return int(c) % 32;
} 

void insertToTrie(struct TrieNode *root, std::string key, StreetIdx idx){ //inserts streetname into trie
    for(int i = 0; i < key.length(); i++){
        if(root->children[charToIndex(key[i])] == nullptr){//if char not already included in trie makes new node for it
            root->children[charToIndex(key[i])] = makeNode(idx);
        }else{
            root->children[charToIndex(key[i])]->streetIndices.push_back(idx);//if already included adds streetidx to node
        } 
        
        root = root->children[charToIndex(key[i])]; //travels to next node
    }
}

std::vector<StreetIdx> findStreetName(struct TrieNode *root, std::string prefix){ //finds which streetnames include the prefix
    for(int i = 0; i < prefix.length(); i++){
        if(root->children[charToIndex(prefix[i])] == nullptr){ //reached point where prefix is not included in trie
            return {}; //returns empty vector
        }else{
            root = root->children[charToIndex(prefix[i])]; //if char found travels to next node
        }
    }
    
    return root->streetIndices;//returns all included streetindices if end of prefix reached and is all included
}