#include <stdio.h>

int main()
{
  char* states2[] = {"California", "Virginia", "Oklahoma", "Washington"}; //text
  (void)states2;

  char states3[4] = {'a', 'p', 'c', 'c'}; //stack
  states3[0] = 'z';
  char s4[3] = "pig"; //stack
  s4[0] = 'a';
  //char* represent string 
  //char represent char
  char* states[] = {"Pennsylvania", "West Virginia", "Ohio", "Maryland"}; //text no change
  int num_states = 4;

  for (int i = 0; i < num_states; i++) {
    printf("state #%d: %s\n", i, states[i]);
    printf("state #%d: %d\n", i, states3[i]);
    printf(states3);
    printf("state #%d: %d\n", 0, s4[0]);
    printf("state #%d: %d\n", 3, states3[0]);

  }

  return 0;
}

