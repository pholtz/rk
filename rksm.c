/* Following program is a C implementation of the Rabin Karp Algorithm 
   given in the CLRS book */
 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
 
// d is the number of characters in input alphabet
#define d 256 

typedef struct {
	unsigned long num_matches = 0;
	unsigned long spurious_matches = 0;
	int print_flag;
} search;

/*  pat  -> pattern
    txt  -> text
    q    -> A prime number
*/
void rabin_karp_search(char *pat, char *txt, int q, search *rk)
{
    int M = strlen(pat);
    int N = strlen(txt);
    int i, j;
    int p = 0;  // hash value for pattern
    int t = 0; // hash value for txt
    int h = 1;
  
    // The value of h would be "pow(d, M-1)%q"
    for (i = 0; i < M-1; i++)
        h = (h*d)%q;
  
    // Calculate the hash value of pattern and first window of text
    for (i = 0; i < M; i++)
    {
        p = (d*p + pat[i])%q;
        t = (d*t + txt[i])%q;
    }
  
    // Slide the pattern over text one by one 
    for (i = 0; i <= N - M; i++)
    {
        
        // Check the hash values of current window of text and pattern
        // If the hash values match then only check for characters one by one
        if ( p == t )
        {
            /* Check for characters one by one */
            for (j = 0; j < M; j++)
            {
                if (txt[i+j] != pat[j])
                    break;
            }
            if (j == M)  // if p == t and pat[0...M-1] = txt[i, i+1, ...i+M-1]
            {
				if(rk->print_flag)
					printf("Pattern found at index %d \n", i);
                rk->num_matches++;
            }
            else
            {
				rk->spurious_matches++;
			}
        }
         
        // Calculate hash value for next window of text: Remove leading digit, 
        // add trailing digit           
        if ( i < N-M )
        {
            t = (d*(t - txt[i]*h) + txt[i+M])%q;
             
            // We might get negative value of t, converting it to positive
            if(t < 0) 
              t = (t + q); 
        }
    }
}
  

int main(int argc, char *argv[])
{
	if(argc != 3)	//Check number of inputs
	{
		printf("Error: Wrong format. Use:\n$rksm [TEXT] [PATTERN]\n");
		return 1;
	}
	FILE *text_fp = NULL;	//Declare File Descriptor
	const char *filename = argv[1];	//Set the Input File to open
	
	//Open the Input Text File
	text_fp = fopen(filename, "r");
	if(text_fp == NULL)
	{
		perror("Error opening text file\n");
		return 1;
	}
	
	//Get the Input Text File Size
	fseek(text_fp, 0L, SEEK_END);
	size_t text_size = ftell(text_fp);
	fseek(text_fp, 0L, SEEK_SET);
	char *text = (char *)malloc(text_size);	//allocate memory for the text file

	//Read the Input File into a character array
	fread(text, 1, text_size, text_fp);

    char *pat = argv[2];	//Get the pattern input
    int q = 101;  // A prime number
    
    //If desired, print the text file
#ifdef PRINT
    for(int i = 0; i < strlen(text); i++)
    {
		printf("%c", text[i]);
	}
	printf("\n");
#endif

	//Time Variables
	struct timespec rk_clock;
	long rk_time = rk_clock.tv_nsec;
	double elapsed_time = 0;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &rk_clock);
	
	//Search Structs
	search p_search; p_search.print_flag = 1;
	search line_search; line_search.print_flag = 0;
	
	//Run the Rabin-Karp Pattern Matcher
    rabin_karp_search(pat, text, q, &p_search);
    
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &rk_clock);
    rk_time = (rk_clock.tv_nsec - rk_time);
    elapsed_time = (double)rk_time / 1000000000.0;
    
    //Print some info about the search
    printf("************************************\n");
    printf("RESULTS FOR PATTERN: '%s'\n", pat);
    printf("************************************\n");
    printf("Number of Matches:\t%lu\n", p_search.num_matches);
    printf("Spurious Matches:\t%lu\n", p_search.spurious_matches);
    printf("Time Elapsed:\t\t%f seconds\n", elapsed_time);
    
    printf("\nText File Searched:\t%s\n", filename);
    printf("Characters Searched:\t%d\n", strlen(text));
    rabin_karp_search("\n", text, q, &line_search);
    printf("Lines Searched:\t\t%lu\n", line_search.num_matches);
    
    //De-Init resources
    fclose(text_fp);
    free(text);
    //getchar();
    return 0;
}
