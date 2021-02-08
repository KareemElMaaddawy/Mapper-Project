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
#define SIZE 26

struct TrieNode{
    struct TrieNode *children[SIZE];
    bool endOfName;
    std::vector<StreetIdx> streetIndices;
};

struct TrieNode *makeNode(StreetIdx strtIdx){
    struct TrieNode *ptr = new TrieNode;
    ptr->endOfName = false;
    ptr->streetIndices.push_back(strtIdx);
    for(int i = 0; i < SIZE; i++) ptr->children[i] = nullptr;
    
    return ptr;
}

struct TrieNode *makeNode(){
    struct TrieNode *ptr = new TrieNode;
    ptr->endOfName = false;
    for(int i = 0; i < SIZE; i++) ptr->children[i] = nullptr;
    
    return ptr;
}

int charToIndex(char c){
    return int(c) % 32;
} 

void insertToTrie(struct TrieNode *root, std::string key, StreetIdx idx){
    for(int i = 0; i < key.length(); i++){
        if(root->children[charToIndex(key[i])] == nullptr){
            root->children[charToIndex(key[i])] = makeNode(idx);
        }else{
            root->children[charToIndex(key[i])]->streetIndices.push_back(idx);
        } 
        
        root = root->children[charToIndex(key[i])];
    }
}

std::vector<StreetIdx> findStreetName(struct TrieNode *root, std::string prefix){
    for(int i = 0; i < prefix.length(); i++){
        if(root->children[charToIndex(prefix[i])] == nullptr){
            return {};
        }else{
            root = root->children[charToIndex(prefix[i])];
        }
    }
    return root->streetIndices;
}