# HW4P1
DS HW4 P1
In this homework, the structure used was a Linked List of Linked Lists. While parsing
through the script file, it determines whether or not it is read, write, or it is an expression.
When it reads, it takes all the words, and inserts the words into a linked list, and sets the name
of that linked list to what was given in the script file.
When it writes, it does the essentially the same thing by grabbing the LinkedList with the specified name
and writing to the file specified, and does not write duplicates and sorts the words it is outputting.
Then when parsing through the script it finds an expression, it checks first whether the expression is valid or not.
If it is invalid, then it is skipped and the program writes to a file that the expression was invalid and then
writes to the file what the invalid expression was.
If it is a valid expression, it is then converted to from infix to postfix using a stack.	
Then it determines whether the expression is a regular expression that performs union and intersection operations, an
assignment expression, or an overwrite expression.
Then postfix expression is then evaluated using a stack, and when it encounters operators + and *, it takes the two
most recent operands in the stack, finds the linked list that corresponds to them, and then performs the operation
that was read. Then after the operations are performed, the new linked list is added to the Linked List of Linked lists 
with its corresponding name so that it can be used later.
