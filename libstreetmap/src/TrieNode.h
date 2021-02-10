/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   TrieNode.h
 * Author: ahmadda4
 *
 * Created on February 9, 2021, 10:22 PM
 */

#ifndef TRIENODE_H
#define TRIENODE_H
#include <vector>

struct TrieNode *makeNode(StreetIdx strtIdx);
struct TrieNode *makeNode();
void destroyTrie(TrieNode *root);
void insertToTrie(struct TrieNode *root, std::string key, StreetIdx idx);
std::vector<StreetIdx> findStreetName(struct TrieNode *root, std::string prefix);

#endif /* TRIENODE_H */

