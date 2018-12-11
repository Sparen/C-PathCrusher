#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Relative and absolute paths
#define DIR_PATH_REL = 0
#define DIR_PATH_ABS = 1

const char *simplify_pathname (const char *path);
char *simplify_parents (char* path);

/* Compresses the directory name. Assumes that input path is not null.
   Duplicate slashes are compressed.
 */
const char *
simplify_pathname (const char *path)
{
  // First, allocate space
  int maxlen = strlen (path); // DOES NOT TAKE NULL TERMINATOR INTO ACCOUNT
  if (maxlen <= 1) // Root directory
    {
      return path;
    }
  char *working_copy = malloc (maxlen + 1);
  strcpy (working_copy, path);

  // Check for starting /.. case and decompose to just /
  if (maxlen >= 3 && path[0] == '/' && path[1] == path[2] && path[1] == '.')
    {
      // Last iteration should move null terminator over
      for (int i = 1; i < maxlen - 1; i += 1)
        {
          working_copy[i] = working_copy[i + 2];
        }
    }

  int currlen = strlen (working_copy);

  // Check for starting ./ case and decompose by removal
  if (maxlen >= 2 && working_copy[0] == '.' && working_copy[1] == '/')
    {
      // Last iteration should move null terminator over
      for (int i = 0; i < maxlen - 1; i += 1)
        {
          working_copy[i] = working_copy[i + 2];
        }
    }

  currlen = strlen (working_copy);

  // Check for interior /./ case and decompose to /
  while (currlen > 2)
    {
      int mutcount = 0;
      // Check each position. currlen - 2 ensures that checks don't go past \0
      for (int i = 0; i < currlen - 2; i += 1)
        {
          if (working_copy[i] == '/' && working_copy[i + 1] == '.' &&
              working_copy[i + 2] == '/')
            {
              for (int j = i + 1; j < currlen - 1; j += 1)
                {
                  working_copy[j] = working_copy[j + 2];
                }
              currlen = strlen (working_copy); // Update
              mutcount += 1; // Enable next try of while loop
              break; // Mutated array, break out of for loop
            }
        }
      // Break out of no changes were made
      if (mutcount == 0)
        {
          break;
        }
    }

  currlen = strlen (working_copy);

  // Check for interior // case and decompose to /
  while (currlen > 1)
    {
      int mutcount = 0;
      // Check each position. currlen - 1 ensures that checks don't go past \0
      for (int i = 0; i < currlen - 1; i += 1)
        {
          if (working_copy[i] == '/' && working_copy[i + 1] == '/')
            {
              for (int j = i + 1; j < currlen; j += 1)
                {
                  working_copy[j] = working_copy[j + 1];
                }
              currlen = strlen (working_copy); // Update
              mutcount += 1; // Enable next try of while loop
              break; // Mutated array, break out of for loop
            }
        }
      // Break out of no changes were made
      if (mutcount == 0)
        {
          break;
        }
    }

  // Remove trailing /. and decompose to /
  if (currlen >= 2 && working_copy[currlen - 2] == '/' && 
      working_copy[currlen - 1] == '.')
    {
      // Move null terminator
      working_copy[currlen - 1] = '\0';
    }

  currlen = strlen (working_copy);

  // Cleanup .. cases
  working_copy = simplify_parents (working_copy);
  currlen = strlen (working_copy);

  if (currlen == maxlen)
    {
      return (const char*) working_copy;
    }
  else
    {
      // Recurse again!
      return simplify_pathname ((const char*) working_copy);
    }
}

/* Compresses the directory name. Assumes that input path is not null.
   Specifically handles ../ cases.
   Recurses until no .. exist.
   In the case that the strings are different, this function frees the old pointer
 */
char *
simplify_parents (char* path)
{
  // First, allocate space
  int maxlen = strlen (path); // DOES NOT TAKE NULL TERMINATOR INTO ACCOUNT
  if (maxlen <= 1) // Root directory
    {
      return path;
    }
  char *working_copy = malloc (maxlen + 1);
  strcpy (working_copy, path);

  int currlen = strlen (working_copy);

  // Next, find any occurrences of ..
  // In case of /.., 
  while (currlen > 3)
    {
      int mutcount = 0;
      // Check each position. currlen - 1 ensures that checks don't go past \0
      for (int i = 0; i < currlen - 1; i += 1)
        {
          if (working_copy[i] == '.' && working_copy[i + 1] == '.')
            {
              // Work backwards to find previous /, then compress
              int slashindex = -1;
              for (int j = i - 2; j >= 0; j -= 1)
                {
                  if (working_copy[j] == '/')
                    {
                      slashindex = j;
                      break;
                    }
                }
              if (slashindex == -1)
                {
                  break; // Break out and allow standard simplify_pathname to handle the case (..; ./..; a/.., etc)
                }
              // Calculate amount to shift by
              int shiftdist = i - slashindex + 2; // + 2 for the second ./
              // If string is not long enough (or ends in .. and not ../[stuff])
              if (slashindex + 1 + shiftdist >= currlen)
                {
                  working_copy[slashindex + 1] = '\0';
                }
              else
                {
                  for (int j = slashindex + 1; j < currlen - (shiftdist - 1); j += 1)
                    {
                      working_copy[j] = working_copy[j + shiftdist];
                    }
                }
              currlen = strlen (working_copy); // Update
              mutcount += 1; // Enable next try of while loop
              break; // Mutated array, break out of for loop
            }
        }
      // Break out of no changes were made
      if (mutcount == 0)
        {
          break;
        }
    }
  // Cleanup
  if (currlen != maxlen)
    {
      free ((void *)path);
      return working_copy;
    }
  free ((void *)working_copy);
  return path;
}

int main(int argc, char** argv)
{
  printf ("Test 1: '/'\n");
  printf ("%s\n", simplify_pathname ("/"));
  printf ("Test 2: '//'\n");
  printf ("%s\n", simplify_pathname ("//"));
  printf ("Test 3: '.'\n");
  printf ("%s\n", simplify_pathname ("."));
  printf ("Test 4: '..'\n");
  printf ("%s\n", simplify_pathname (".."));
  printf ("Test 5: './a.txt'\n");
  printf ("%s\n", simplify_pathname ("./a.txt"));
  printf ("Test 6: '/a.txt'\n");
  printf ("%s\n", simplify_pathname ("/a.txt"));
  printf ("Test 7: '//a.txt'\n");
  printf ("%s\n", simplify_pathname ("//a.txt"));
  printf ("Test 8: 'a//b'\n");
  printf ("%s\n", simplify_pathname ("a//b"));
  printf ("Test 9: '/../x'\n");
  printf ("%s\n", simplify_pathname ("/../x"));
  printf ("Test 10: '/./'\n");
  printf ("%s\n", simplify_pathname ("/./"));
  printf ("Test 11: '/../'\n");
  printf ("%s\n", simplify_pathname ("/../"));
  printf ("Test 12: '/../.'\n");
  printf ("%s\n", simplify_pathname ("/../."));
  printf ("Test 13: '/./..'\n");
  printf ("%s\n", simplify_pathname ("/./.."));
  printf ("Test 14: '/..'\n");
  printf ("%s\n", simplify_pathname ("/.."));

  printf ("Test B1: '/a/b/../c.txt'\n");
  printf ("%s\n", simplify_pathname ("/a/b/../c.txt")); 
  printf ("Test B2: '/a/b/c/d/e/f/g/h/i/../../../../../j'\n");
  printf ("%s\n", simplify_pathname ("/a/b/c/d/e/f/g/h/i/../../../../../j"));  
  printf ("Test B3: '/../../../../../a'\n");
  printf ("%s\n", simplify_pathname ("/../../../../../a"));
  printf ("Test B4: '/../../../../b/../a'\n");
  printf ("%s\n", simplify_pathname ("/../../../../b/../a"));
  printf ("Test B5: '/./.././.././.././../././/./././/.//./././/././/././/a.txt'\n");
  printf ("%s\n", simplify_pathname ("/./.././.././.././../././/./././/.//./././/././/././/a.txt"));
  printf ("Test B6: './../../a'\n");
  printf ("%s\n", simplify_pathname ("./../../a"));
  printf ("Test B7: '/a/..'\n");
  printf ("%s\n", simplify_pathname ("/a/.."));
}
