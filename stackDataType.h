struct nodeStack;
typedef struct nodeStack nodeS;
typedef nodeS* stack;

struct nodeStack
{
   int value;
   stack next;
};

