#ifndef PARSER_H
#define PARSER_H
#include "token.h"
#include "node.h"
#include <string>

using namespace std;

//used to store token
extern Token tk;

//for scanning the program 
node_t *parser();

//BNF
node_t *program();
node_t *block();
node_t *vars();
node_t *expr();
node_t *A();
node_t *N();
node_t *M();
node_t *R();
node_t *stats();
node_t *mStat();
node_t *stat();
node_t *in();
node_t *out();
node_t *cond();
node_t *loop();
node_t *assign();
node_t *RO();

//print the error if incorrect token was received
void parserError();

//create a new node
node_t *createNode(string);

#endif
