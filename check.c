#include<stdio.h>
#include<stdlib.h>

int main(int argc, char **argv) {

  if (argv[1] == NULL) { // Checking arguments first

    printf("No arguments\n") ;
    return 0 ;

  }

  if (argc > 2) {

    printf("Too many arguments\n") ;
    return 0 ;

  }

  char * input = argv[1] ;

  int numberOfTokens = 1 ;

  int i = 0 ;

  while (input[i] != '\0') { // Reading all tokens

      if (input[i] == ' ') {

          numberOfTokens++ ;

      }

      i++ ;

  }

  if (i > 0 && input[i - 1] == ' ') {

      numberOfTokens-- ;

  }

  //printf("Number of tokens: %d\n", numberOfTokens) ;

  char ** tokens = (char **)malloc(numberOfTokens * sizeof(char *)) ; // Create space to store tokens in after checking how many there are

  for (i = 0 ; i < numberOfTokens ; i++) {

    tokens[i] = (char *)malloc(6 * sizeof(char)) ;

  }

  i = 0 ;
  int j = 0 ;
  int k = 0 ;


  while (input[i] != '\0') {

      if (input[i] != ' ') {

         tokens[j][k] = input[i] ;
         k++ ;

      } else {

        if (k > 0) {

        tokens[j][k] = '\0' ;
        j++ ;
        k = 0 ;

        }

      }

      i++ ;
  }

/*
  for (i = 0 ; i < numberOfTokens ; i++) {

      printf("%s\n", tokens[i]) ;

  }
*/

  i = 0 ;

  int numberOfExpressions = 1 ;

    while (input[i] != '\0') {

        if (input[i] == ';' && i != 0 && input[i - 1] == ' ') {

          numberOfExpressions++ ;

        }

        i++ ;

    }

    char *** expressions = (char ***)malloc(numberOfExpressions * sizeof(char **)) ; // Triple pointer to store multiple expressions which hold multiple strings each

    j = 0 ;

    for (i = 0 ; i < numberOfTokens ; i++) {

        //printf("Parsing %s\n", tokens[i]) ;

        if (tokens[i][0] != ';') {

            int originalIndex = i ;
            int expSize = 0 ;

            while (i < numberOfTokens && tokens[i][0] != ';') {

              expSize++ ;
              i++ ;

            }

            //printf("exp size: %d\n", expSize) ;

            expressions[j] = (char **)malloc(expSize * sizeof(char *)) ;
            i = originalIndex ;
            k = 0 ;

            if (expSize < 3) {

              expressions[i][3] = NULL ;

              if (expSize < 4) {

                expressions[i][4] = NULL ;

              }

            }

            //printf("Expression number %d: ", j) ;

            while (i < numberOfTokens && tokens[i][0] != ';') {

                expressions[j][k] = (char *)malloc(6 * sizeof(char)) ; // The largest token that will be recognized is "false"
                expressions[j][k] = tokens[i] ;
                //printf("number %d: %s \n", k,expressions[j][k]) ;
                i++ ;
                k++ ;

                }

                while (k <= expSize) {

                  //if (expressions[j][k] != tokens[i]) {

                    //printf("%d = null\n", k) ;
                    expressions[j][k] = NULL ;

                 // }

                  k++ ;

                }

                j++ ;
                //printf("end\n") ;


        }

    }

  //printf("Number of expressions: %d\n", numberOfExpressions) ;

  int logical = 0 ;
  int arithmetic = 0 ;

  for (i = 0 ; i < numberOfExpressions ; i++) {

        if ((expressions[i][0] != NULL && isOperand(expressions[i][0]) == 1) && (expressions[i][1] != NULL && isOperator(expressions[i][1]) == 1) && (expressions[i][2] != NULL && isOperand(expressions[i][2]) == 1)) {

            arithmetic++ ;

        } else if ((expressions[i][0] != NULL && isOperand(expressions[i][0]) == 2) && (expressions[i][1] != NULL && isOperator(expressions[i][1]) == 2) && (expressions[i][2] != NULL && isOperand(expressions[i][2]) == 2)) {

            logical++ ;

        } else if ((expressions[i][0] != NULL && isOperator(expressions[i][0]) == 3) && (expressions[i][1] != NULL && isOperand(expressions[i][1]) == 2)) {

            logical++ ;

        }

  }

  printf("Found %d expressions: %d logical and %d arithmetic.\n", numberOfExpressions, logical, arithmetic) ;

  for (i = 0 ; i < numberOfExpressions ; i++) { // Error checking time

      if (expressions[i][1] != NULL) {

        if (isOperand(expressions[i][0]) == 0 && isOperator(expressions[i][0]) != 3) {

          if (isOperator(expressions[i][0]) > 0) {

              printf("Error: Parse error in expression %d: unexpected operator\n\"%s\"\n", i, expressions[i][0]) ;

          } else {

              printf("Error: Parse error in expression %d: unknown identifier\n\"%s\"\n", i, expressions[i][0]) ;

              if(isOperator(expressions[i][1]) > 0 && isOperator(expressions[i][1]) < 3) {

                printf("Error: Parse error in expression %d: unexpected operator\n\"%s\"\n", i, expressions[i][1]) ;

              }

          }

        } else if (isOperand(expressions[i][0]) == isOperator(expressions[i][1]) && isOperator(expressions[i][0]) != 3) {

            if (expressions[i][2] != NULL) {

              if (isOperand(expressions[i][2]) == isOperand(expressions[i][0])) {

                printf("OK.\n") ;

              } else if (isOperand(expressions[i][2]) != 0) {

                printf("Error: Parse error in expression %d: operand type mismatch\n\"%s %s %s\"\n", i, expressions[i][0], expressions[i][1], expressions[i][2]) ;

              } else if (isComplete(expressions[i][2]) == 0) {

                printf("Error: Parse error in expression %d: expression incomplete\n\"%s\"\n", i, expressions[i][2]) ;

              } else {

                printf("Error: Parse error in expression %d: unknown operand\n\"%s\"\n", i, expressions[i][2]) ;

              }

            } else {

              printf("Error: Parse error in expression %d: missing operand\n\"\"\n", i) ;

            }

        } else if (isOperator(expressions[i][0]) == 3 && isOperand(expressions[i][1]) == 2) {

            printf("OK.\n") ;

        } else if (isOperator(expressions[i][1]) == 0) {

            printf("Error: Parse error in expression %d: unknown operator\n\"%s\"\n", i, expressions[i][1]) ;

        } else {

            printf("Error: Parse error in expression %d: operand type mismatch\n\"%s %s\"\n", i, expressions[i][0], expressions[i][1]) ;

            if (expressions[i][2] != NULL && isOperand(expressions[i][2]) != isOperator(expressions[i][1])) {

              printf("Error: Parse error in expression %d: operator type mismatch\n\"%s %s %s\"\n", i, expressions[i][0], expressions[i][1], expressions[i][2]) ;

            }

        }

      } else {

        if (isOperand(expressions[i][0]) == 0) {

          printf("Error: Parse error in expression %d: unknown identifier\n\"%s\"\n", i, expressions[i][0]) ;

        }

        printf("Error: Parse error in expression %d: missing operator\n\"\"\n", i) ;

      }

      if (expressions[i][3] != NULL) { // If expression didn't end, further arguments need to be calculated as regular expressions too

          printf("Error: Parse error in expression %d: expression wasn't ended\n\"%s %s %s %s\"\n", i, expressions[i][0], expressions[i][1], expressions[i][2], expressions[i][3]) ;

          int count = 3 ;

          while (expressions[i][count] != NULL) {

            //printf("accessing [%d][%d]\n", i, count) ;

            if (isOperator(expressions[i][count]) > 0) {

              if (isOperator(expressions[i][count]) == 3) {

                if (expressions[i][count + 1] != NULL) {

                  count++ ;
                  if (isOperand(expressions[i][count]) != 2) {

                    printf("Error: Parse error in expression %d: unexpected operand\n\"%s\"\n", i, expressions[i][count]) ;

                  }

                }

              } else {

                printf("Error: Parse error in expression %d: unexpected operator\n\"%s\"\n", i, expressions[i][count]) ;

              }

            } else if (isOperand(expressions[i][count]) > 0) {

              if (count == 3) {

                printf("Error: Parse error in expression %d: unexpected operand\n\"%s\"\n", i, expressions[i][count]) ;

              } else {

                if (expressions[i][count + 1] != NULL) {

                count++ ;

                if (isOperator(expressions[i][count]) > 0) {

                  if (isOperator(expressions[i][count]) != isOperand(expressions[i][count - 1])) {

                    printf("Error: Parse error in expression %d: operand type mismatch\n\"%s\"\n", i, expressions[i][count]) ;

                  }


                } else {

                  if (isOperand(expressions[i][count]) != 0) {

                    printf("Error: Parse error in expression %d: unexpected operand\n\"%s\"\n", i, expressions[i][count]) ;

                  } else {

                    printf("Error: Parse error in expression %d: unknown operator\n\"%s\"\n", i, expressions[i][count]) ;

                  }

                }

                if (expressions[i][count + 1] != NULL) {

                  count++ ;

                  if (isOperand(expressions[i][count]) > 0) {

                    if (isOperand(expressions[i][count]) != isOperator(expressions[i][count - 1])) {

                      if (isOperand(expressions[i][count]) != 2 && isOperator(expressions[i][count - 1]) != 3) {

                        printf("Error: Parse error in expression %d: operator type mismatch\n\"%s\"\n", i, expressions[i][count]) ;

                      }

                    }

                    } else {

                  printf("Error: Parse error in expression %d: unknown operand\n\"%s\"\n", i, expressions[i][count]) ;

                }

              } else {

                printf("Error: Parse error in expression %d: missing operand\n\"\"\n", i) ;

              }

              } else {

                printf("Error: Parse error in expression %d: missing operator\n\"\"\n", i) ;

              }


              }

            } else if (isComplete(expressions[i][count]) == 0) {

              printf("Error: Parse error in expression %d: expression incomplete\n\"%s\"\n", i, expressions[i][count]) ;

            } else {

              printf("Error: Parse error in expression %d: unknown identifier\n\"%s\"\n", i, expressions[i][count]) ;

            }

            count++ ;

          }

      }

  } 

  k = 0 ;

   for (i = 0 ; i < numberOfTokens ; i++) { // Need to know expression sizes again to free dynamically allocated memory

        if (tokens[i][0] != ';') {

            int expSize = 0 ;

            while (i < numberOfTokens && tokens[i][0] != ';') {

              expSize++ ;
              i++ ;

            }

            for (j = 0 ; j < expSize ; j++) {

              free(expressions[k][j]) ;

            }

            k++ ;

        }

   }

   free(expressions) ;

  for (i = 0 ; i < numberOfTokens ; i++) {

    free(tokens[i]) ;

  }

  free(tokens) ;

  return 0 ;

}

int isOperand(char * input) { // No string library :(

    if (getStringLength(input) == 1) {

      if (input[0] == '0' || input[0] == '1' || input[0] == '2' || input[0] == '3' || input[0] == '4' || input[0] == '5' || input[0] == '6' || input[0] == '7' || input[0] == '8' || input[0] == '9') {
        return 1 ;

      }

    } else if (getStringLength(input) == 4) {

      if (input[0] == 't' && input[1] == 'r' && input[2] == 'u' && input[3] == 'e') {

          return 2 ;

      }

    } else if (getStringLength(input) == 5) {

      if (input[0] == 'f' && input[1] == 'a' && input[2] == 'l' && input[3] == 's' && input[4] == 'e') {

        return 2 ;

      }

    }

  return 0 ;

}

int isOperator(char * input) {

  if (getStringLength(input) == 1) {

    if (input[0] == '+' || input[0] == '-' || input[0] == '*' || input[0] == '/') {

      return 1 ;

    }

  } else if (getStringLength(input) == 3) {

    if (input[0] == 'A' && input[1] == 'N' && input[2] == 'D') {

      return 2 ;

    } else if (input[0] == 'N' && input[1] == 'O' && input[2] == 'T') {

      return 3 ;

    }

  } else if (getStringLength(input) == 2) {

    if (input[0] == 'O' && input[1] == 'R') {

      return 2 ;

    }

  }

  return 0 ;

}

int getStringLength(char * input) {

  int output = 0 ;

  while (input[output] != '\0') {

    output++ ;

  }

  return output ;

}

int isComplete(char * input) {

  int i = 0 ;

  while (input[i] != '\0') {

    if (input[i] == ';') {

        return 0 ;

    }

    i++ ;

  }

  return 1 ;

}

