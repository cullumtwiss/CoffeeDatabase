#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {//this is our structure definition
  char name[50];
  long production;
  float population;
  float income;
  int rank;
}country_s;

void displayBlurb(void);//just displays the info about my data
int fillArray(country_s countryArray[], int sortArray[]);//fills array with data from file and initializes sortArray
void showMenu(country_s countryArray[], int sortArray[], int length);//shows menu, takes user input, and calls correct function
void searchData(country_s countryArray[], int length);//gives user search options and executes search
void sortData(country_s countryArray[], int sortArray[], int length);//gives user sort options and executes sort
void displayData(country_s countryArray[], int sortArray[], int length);//displays data based on sortArray
void addRecord(country_s countryArray[], int *length);//lets user add new record to end of array of structures
void deleteRecord(country_s countryArray[], int *length);//lets user delete record by searching its name
void saveData(country_s countryArray[], int sortArray[], int length);//allows user to save data to file of their choosing

int main(void) {
  country_s countryArray[200];//max number of records is 200
  int sortArray[200];//sortArray just has to be integers from 0 to 200
  
  displayBlurb();//we display the opening blurb
  int length = fillArray(countryArray, sortArray);//fillArray returns the length of the array, so we change value in main
  showMenu(countryArray, sortArray, length);//everything else happens in showMenu
}//we need to pass it these three values because it calls all other functions from inside showMenu

void displayBlurb(void) {
  printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
         "|This is data started with my love of coffee.                  |\n"
         "|I wanted to find out which coutries produced the most coffee  |\n"
         "|and how that number relates to other fascinating statistics.  |\n"
         "|What resulted is the data you see here. This data contains 50 |\n"
         "|countries, the amount of coffee they produce (in metric tons),|\n"
         "|their production ranking, their population (in thousands), and|\n"
         "|their average monthly income (USD). After options that display|\n"
         "|data, hit enter again to return to the menu. -0 in a piece of |\n"
         "|data means that value is unknown. If enteringa record and you |\n"
         "|don't know the data, enter -0. Default can be chosen by       |\n"
         "pressing enter for each option when available. Enjoy!          |\n"
         "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
}

int fillArray(country_s countryArray[], int sortArray[]) {//we pass it the blank countryArray and sortArray
  int i;
  int length = 0;//length starts at zero
  char junk;//wherever you see junk, that means I'm using it to clear whitespace so scanf works properly
  FILE *dataFile;
  char fileName[50];//this will store the user's choice of file (limit is 50 characters)
  char choice;//in every function, choice refers to whatever user input I'm taking
  
  for(i = 0; i < 200; i++) {//initialize sortArray with ascending integers from 0 to 200
    sortArray[i] = i;
  }
  i = 0;//reset i to use in next loop
  
  printf("Would you like to choose a file to read from or use the default? Hit enter for default and f for your own file.\n");
  scanf("%c", &choice);
  
  while(choice != 10 && choice != 13 && choice!= 'f') {//keeps scanning until input is valid. We check 10 and 13 to see if they hit enter
      printf("Invalid choice. Try again.");
      scanf("%c", &choice);
    }
    
  if(choice == 10 || choice == 13) {//this is the default case
  dataFile = fopen("data.txt", "r");//so we open the default file
  } else {
    printf("Enter file name: ");//if they want to enter a name, we open that name
    scanf("%s", fileName);
    dataFile = fopen(fileName, "r");
  } 
  
  while(dataFile == NULL) {//if their file doesn't exist we keep asking for a name until it is valid
    printf("Invalid file. Type another name or enter / for default.");//give option to change their mind to default
    scanf("%s", fileName);
    if(*fileName == '/') {
        dataFile = fopen("data.txt", "r");//if they change their mind we open the default file and exit the while loop
    } else{ 
    dataFile = fopen(fileName, "r");//we check again if their new choice exists
    }
  }
  
  
  while(fscanf(dataFile, "%s %c %li %c %f %c %f %c %d", &countryArray[i].name[0], &junk, &countryArray[i].production, &junk, &countryArray[i].population, &junk, &countryArray[i].income, &junk, &countryArray[i].rank) != EOF
  && i < 200) {//each country name is followed by a space to avoid it taking the comma into the string. Then we eat every comma with a junk characters
    i++;
    length++;//we increase the length by one every time another structure is read. This works because we only read until the end of file
  }
  
  fclose(dataFile);//close file and return length to pass to showMenu
  return(length);
}

void showMenu(country_s countryArray[], int sortArray[], int length) {
  char choice;//choice and junk are used in almost every function
  char junk;
  
  printf("What would you like to do? You can press 0 at any time to quit.\n"
         "(1)Search\n(2)Sort\n(3)Display Data\n(4)Add Record\n(5)Delete Record\n(6)Save Data\n");
         scanf("%c", &choice);
         scanf("%c", &junk);
     
         switch(choice) {//take choice as a character to avoid program crashing if user enters a number
         case 48://all the cases call the corresponding functions then call showMenu again
         exit(1);
         break;
         case 49:
         searchData(countryArray, length);
         showMenu(countryArray, sortArray, length);
         break;
         case 50:
         sortData(countryArray, sortArray, length);
         showMenu(countryArray, sortArray, length);
         break;
         case 51:
         displayData(countryArray, sortArray, length);
         showMenu(countryArray, sortArray, length);
         break;
         case 52:
         addRecord(countryArray, &length);
         showMenu(countryArray, sortArray, length);
         break;
         case 53:
         deleteRecord(countryArray, &length);
         showMenu(countryArray, sortArray, length);
         break;
         case 54:
         saveData(countryArray, sortArray, length);
         showMenu(countryArray, sortArray, length);
         break;
         default://if entry is invalid then nothing happens and showMenu begins again
         printf("Invalid option.\n");
         scanf("%c", &junk);
         showMenu(countryArray, sortArray, length);
         break;
         }
         
         
}

void searchData(country_s countryArray[], int length) {
  int i;//for loop
  char choice, junk;
  //these two variables store the user's search terms
  float incomeSearch;
  char searchName[50];
  
  printf("You have chosen to search. By what criteria would you like to search?\n(n)Country Name\n(a)Average Monthly Salary or press enter for default (country name)\n");
  scanf("%c", &choice); 
  if(choice == 48) {//this is put after every user input to allow them to exit by pressing 0 (ASCII code 48)
    exit(1);
  }
  
    while(choice != 10 && choice != 13 && choice!= 'n' && choice!= 'a' && choice != 'N' && choice != 'A' && choice != 0) {
      printf("Invalid choice. Try again.\n");//keeps  scanning until choice is valid
      scanf("%c", &junk);
      scanf("%c", &choice);    
    }
    
  if(choice == 48) {
     exit(1);
  }
  
  if(choice == 'n' || choice == 10 || choice == 13) {//if user chooses n or hits enter
    printf("Enter the country name you would like to search for. If it has spaces, replace the spaces with _");
    scanf("%s", searchName);//it means they want to search by name, so we take the name
    printf("Country||Production||Population||Average Monthly Income||Prod Ranking||\n");//print the labels to make data less confusing
    
    for(i = 0; i < length; i++) {//if country name matches their search we print it
      if(strcmp(countryArray[i].name, searchName) == 0) {
        printf("%s, %.2li, %.2f, %.2f, %d\n", countryArray[i].name, countryArray[i].production, countryArray[i].population, countryArray[i].income, countryArray[i].rank);
      }
    }
    
  } else {//if choice isn't n or default, they want to search by income
  
    printf("Would you like to search for countries with average monthly incomes higher (h), lower(l), or equal(e) to your input? Default is higher.\n");
    scanf("%c", &junk);
    scanf("%c", &choice); 
    
    if(choice == 48) {
    exit(1);
  }
  
  while(choice != 10 && choice != 13 && choice!= 'h' && choice!= 'l' && choice != 'e') {
      printf("Invalid choice. Try again.\n");//keep scanning until choice is valid
      scanf("%c", &junk);
      scanf("%c", &choice);    
    }
    
    scanf("%c", &junk);//now we know if they want to search higher, lower, or equal, so we take the salary
    printf("What salary would you like to search for?\n");
    scanf("%f", &incomeSearch);
    printf("Country||Production||Population||Average Monthly Income||Prod Ranking||\n");
    
    switch(choice) {//switch for higher, lower, or equal (default is enter key)
    
      case('h'):
      for(i = 0; i < length; i++) {
        if(countryArray[i].income > incomeSearch) {
           printf("%s, %.2li, %.2f, %.2f, %d 1st\n", countryArray[i].name, countryArray[i].production, countryArray[i].population, countryArray[i].income, countryArray[i].rank);
        }
      } 
      break;
      
      case('l'):
      for(i = 0; i < length; i++) {
        if(countryArray[i].income < incomeSearch && countryArray[i].income != -0) {//makes sure income is actually a value
           printf("%s, %.2li, %.2f, %.2f, %d 2st\n", countryArray[i].name, countryArray[i].production, countryArray[i].population, countryArray[i].income, countryArray[i].rank);
        }
      }
      break;
      
      case('e'):
      for(i = 0; i < length; i++) {
        if(countryArray[i].income == incomeSearch) {//print countries with equal income
           printf("%s, %.2li, %.2f, %.2f, %d 3rd\n", countryArray[i].name, countryArray[i].production, countryArray[i].population, countryArray[i].income, countryArray[i].rank);
        }
      }
      break;
      
      default:
      for(i = 0; i < length; i++) {//print countries with higher income is default
        if(countryArray[i].income > incomeSearch) {
           printf("%s, %.2li, %.2f, %.2f, %d\n", countryArray[i].name, countryArray[i].production, countryArray[i].population, countryArray[i].income, countryArray[i].rank);
        }
      }
      break;
      
    }
  }
  scanf("%c", &junk);//take extra whitespace at the end of each function
}

void sortData(country_s countryArray[], int sortArray[], int length) {
  char choice, junk;
  int i, j;//loop variables
  
  printf("You have chosen to sort. Would you like to sort by rank(r) or name(n)? Rank is the default");
  scanf("%c", &choice);
  
  if(choice == 48) {
    exit(1);
  }
  
   while(choice!= 'r' && choice!= 'n' && choice != 10 && choice != 13 && choice != 0) {
      printf("Invalid choice. Try again.");
      scanf("%c", &junk); 
      scanf("%c", &choice); 
    }
    
    if(choice == 48) {
    exit(1);
    }
    
    if(choice == 'r' || choice == 10 || choice == 13) {//if they choose r give option to sort in ascending or descending order
      printf("Would you like to sort from highest to lowest(h) or lowest to highest(l)? Default is lowest to highest.");
      scanf("%c", &choice);
      
      if(choice == 48) {
        exit(1);
      }
      
      while(choice!= 'l' && choice!= 'h' && choice != 10 && choice != 13 && choice != 0) {
        printf("Invalid choice. Try again.");
        scanf("%c", &junk); //keep scanning until choice is valid
        scanf("%c", &choice); 
      }
      
      if(choice == 48) {
        exit(1);
      }
      
      if(choice == 'l' || choice == 10 || choice == 13) {
        
        for(j = 0; j < length; j ++) {//j starts at the 0th index
          for(i = j + 1; i < length; i++) {//i is always the index to the right of j
          
            if(countryArray[sortArray[j]].rank > countryArray[sortArray[i]].rank) {//if countries are out of order
              int swap = sortArray[j];//we swap the indexes of sort array to reflect this
              sortArray[j] = sortArray[i];//plain old bubble sort
              sortArray[i] = swap;
            }
            
          }  
        }
      } else {
          for(j = 0; j < length; j ++) {
            for(i = j + 1; i < length; i++) {
          
              if(countryArray[sortArray[j]].rank < countryArray[sortArray[i]].rank) {
                int swap = sortArray[j];//bubble sort by rank in descending order
                sortArray[j] = sortArray[i];
                sortArray[i] = swap;
              }
            
            } 
          }
        }
      
    } else {
        for(j = 0; j < length-1; j ++) {
          for(i = 0; i < length-1; i++) {
            if((strcmp(countryArray[sortArray[i]].name, countryArray[sortArray[i+1]].name) > 0)) {//if j is farther in alphabet than i
        
              int swap = sortArray[i];//bubble sort alphabetically with strcmp()
              sortArray[i] = sortArray[i+1];
              sortArray[i+1] = swap;
              
            }
          } 
        }
      }
      
  scanf("%c", &junk);
}

void displayData(country_s countryArray[], int sortArray[], int length) {
  char junk;
  int i;
  
  printf("Country||Production||Population||Average Monthly Income||Prod Ranking||\n");
  for(i = 0; i < length; i++) {
      printf("%s, %.2li, %.2f, %.2f, %d\n", countryArray[sortArray[i]].name, countryArray[sortArray[i]].production, countryArray[sortArray[i]].population, countryArray[sortArray[i]].income, countryArray[sortArray[i]].rank);
  }//just print every country based on order of sortArray
  
  scanf("%c", &junk);
}

void addRecord(country_s countryArray[], int *length) {//pass pointer of length so we can change it
  int again = 1;//again decides if function is called again, starts as true
  char junk;
  
  while(again) {//while the user has more records to add we take each piece of data
  printf("You have chosen to add a record. What is the name of the country? (Max 50 characters) Remember to use underscores for spaces.\n");
  scanf("%s", countryArray[*length].name);
  printf("How much coffee does it produce (in metric tons)?\n");
  scanf("%li", &countryArray[*length].production);
  printf("What is its population (in thousands)?\n");
  scanf("%f", &countryArray[*length].population);
  printf("What is its average monthly income (in USD)?\n");
  scanf("%f", &countryArray[*length].income);
  printf("What is its world coffee production ranking?\n");
  scanf("%d", &countryArray[*length].rank);
  
  *length = *length + 1;//increase length by one because array now has one more structure
  
  printf("Would you like to add any more records?\n(0)No\n(1)Yes");
  scanf("%d", &again);
  
  if(again == 48) {
    exit(1);
  }//always gives user option to quit so they don't get stuck in a loop
  
  }
   scanf("%c", &junk);
}

void deleteRecord(country_s countryArray[], int *length) {
  char choice[50];//stores user's string input
  int i;
  int indexFound = -1;//this variable stores the index of the place where the search term is found
  char again;
  char junk;
  
  printf("You've chosen to delete an entry. Enter the name of the country whose entry you would like to delete:");
  scanf("%s", choice);
  
  for(i = 0; i < *length; i++) {
     if((strcmp(countryArray[i].name, choice)) == 0) {
      indexFound = i;//if the name is found, we keep track of the index where it was found
     }
  }
  if(indexFound != -1) {
    
    for(i = indexFound; i < *length-1; i++) {
      countryArray[i] = countryArray[i+1];
    }//shift each element one to the left from index to the index so that the user's entry is deleted
    
    *length = *length - 1;//decrease length by one because array is now one shorter
    
    printf("Would you like to delete another entry?\n(1)No\n(Any Key)Yes\n");
    scanf("%c", &junk);
    scanf("%c", &again);//find out if they have any more entries they want to delete
    
    if(again == 48) {
    exit(1);
    }
    
  } else {//if country name is not found we ask them if they want to search again
      printf("Country name not found. Would you like to search again?\n(1)No\n(Any Key)Yes\n");
      scanf("%c", &junk);
      scanf("%c", &again);
      
      if(again == 48) {
        exit(1);
      }
    } 
    
  if(again != 49) {
    deleteRecord(countryArray, length);
  }
  
  scanf("%c", &junk);
}

void saveData(country_s countryArray[], int sortArray[], int length) {
  FILE *saveFile;//this is the file we are going to open
  int i;
  int exists = 0;//is true if file exists and false if it doesn't
  char choice, junk;
  char fileName[50];
  char extension[] = ".txt";//we will add this to the end of user entry to ensure data is saved to a text file
  
  printf("Enter the name of the file you would like to save the data to, without the file extension: \n");
  scanf("%s", fileName); 
  strcat(fileName, extension);//append .txt to whatever file name the user entered
  
    if((saveFile = fopen(fileName, "r"))) {//check if file exists by opening it for reading and seeing if fileopen is null
      fclose(saveFile);
      exists = 1;//if it exists we set this variable to true
    }
    
    if(exists) {//if it exists we first ask if they want to overwrite it
    printf("File already exists. Are you sure you want to overwrite it?\n(y)Yes\n(n)No\n"); 
    scanf("%c", &junk);
    scanf("%c", &choice);
    if(choice == 48) {
      exit(1);
    }
    
    scanf("%c", &junk);
    
    while(choice != 'n' && choice != 'y') {//keep scanning until input is valid
      printf("Invalid choice. Try again.\n");
      scanf("%c", &choice);
    }
    
    if(choice == 48) {
    exit(1);
  }
    if(choice == 'y') {//if they choose to overwrite it then we write data to file based on order of sortArray
    saveFile = fopen(fileName, "w"); //open file they entered for writing
    
    while(i < length) {//write to file in correct format
      fprintf(saveFile, "%s , %.2li, %.2f, %.2f, %d\n", countryArray[sortArray[i]].name, countryArray[sortArray[i]].production, countryArray[sortArray[i]].population, countryArray[sortArray[i]].income, countryArray[sortArray[i]].rank);
      i++;
    }
    
     printf("File saved\n");//user gets nervous if we don't confirm that file is saved
     fclose(saveFile);
     
  } else {//if they chose not to overwrite file then we call function again to get different file name
      saveData(countryArray, sortArray, length);
    } 
  } else {//if file doesn't already exist we just save data to it
    saveFile = fopen(fileName, "w");
    
    while(i < length) {
      fprintf(saveFile, "%s , %.2li, %.2f, %.2f, %d\n", countryArray[sortArray[i]].name, countryArray[sortArray[i]].production, countryArray[sortArray[i]].population, countryArray[sortArray[i]].income, countryArray[sortArray[i]].rank);
      i++;
    }
    
    printf("File saved\n");
    fclose(saveFile);
  }
  
  scanf("%c", &junk);
}
/*There is one error I could never figure out. If the user adds a country and then tries to sort alphabetically,
the new country always stays at the bottom. I've tried increasing the length of the sort loops which didn't work.
When I printed the result of each string comparison between every country and the new country, they all will always 
come back negative, which is why the new country won't move. I cannot figure out why this happens and I have tried 
everything to fix it, but unfortunately I've had to leave it as is. I'm completely baffled as to why this happens, 
especially because this error never occurs when adding a new country then sorting by rank. Any suggestions for a fix
are welcome and I would be happy to resubmit if you can identify the error. */