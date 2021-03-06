#include <stdio.h>
#include <string.h>
#include "Evaluate.h"
#include "Stack.h"
#include "StringObject.h"
#include "Text.h"
#include "Token.h"
#include "tryEvaluatethenPush.h"
#include "operatorEvaluate.h"
#include "calculateToken.h"
#include "createNumberToken.h" 
#include "LinkedList.h"
#include "stackForEvaluate.h"
#include <malloc.h>
#include "ErrorCode.h"
#include "CException.h"

/**
	Evaluate all operator token on the operator stack that have strictly lower
	precedence than the operator token to be pushed. The evaluation of operators token
	is from the top of stack to bottom
	
	Input :
		stack the operatorStack
		operator the operator to be pushed onto the operator stack 
		
**/

void tryEvaluateOperatorOnStackThenPush(Operator *newToken,Stack *numberStack,Stack *operatorStack){
	Operator *previousToken;
	previousToken=(Operator*)stackPop(operatorStack);
	
	if(previousToken==NULL){
		stackPush(newToken,operatorStack); //I deleted the closing bracket execution details as it unnecessary to put it since the postfix will deal in function 
	}else{								  // tryEvaluateAndExecutePostfix
		while(previousToken!=NULL){
			if(newToken->info->precedence > previousToken->info->precedence){
				break;
			}
			else{
				operatorEvaluate(numberStack,previousToken);
			}
			previousToken=(Operator*)stackPop(operatorStack);
		}
		if(previousToken!=NULL ){
			stackPush(previousToken,operatorStack);
		}
		stackPush(newToken,operatorStack);
	}
}

/** 
	This function is use to convert the infix operator to prefix operator.
	If the infix operator cannot be convert, then will throw error
	cannot convert to prefix 
**/
void tryConvertToPrefix(Operator *opeToken){
	Stack *numberStack;
	int i;
	OperatorInfo *info=operatorFindAlternateInfoByName(opeToken->info->symbol);
	opeToken->info=info;
	
	if(opeToken->info == NULL)
	{
		Throw(ERR_CANNOT_CONVERT_TO_PREFIX);
	}
}

/**
	Evaluate postfix operator.
	when get closing bracket operator,push into the operator stack,then pop it out and execute.
	When doing execute,pop the opening bracket operator.
	Execute the expression and free the opening bracket
	precedence than the operator token to be pushed. The evaluation of operators token
	is from the top of stack to bottom
	
	Input :
		stack the operatorStack
		operator the operator to be pushed onto the operator stack 
		
**/

void tryEvaluateAndExecutePostfix(Operator *newToken,Stack *numberStack,Stack *operatorStack){
	Operator *previousToken=(Operator*)stackPop(operatorStack);
	if(previousToken == NULL){
		checkPrefixOperator(newToken,numberStack,operatorStack);
		stackPush(newToken,operatorStack);
	}
	else{
		while(previousToken!=NULL)
		{
			if(previousToken->info->affix == INFIX){
				operatorEvaluate(numberStack,previousToken);
			}else if(previousToken->info->affix == PREFIX){
				stackPush(newToken,operatorStack);
				operatorPrefixOrPostfixEvaluate(numberStack ,newToken);
				free(previousToken);
				break;
			}
			previousToken=(Operator*)stackPop(operatorStack);	
		}
			
	}
}

void checkPrefixOperator(Operator *newToken,Stack *numberStack,Stack *operatorStack){
		if(newToken->info->id ==CLOSING_BRACKET_OP){
			Throw(ERR_EXPECTING_OPENING_BRACKET);
		}
}



















































