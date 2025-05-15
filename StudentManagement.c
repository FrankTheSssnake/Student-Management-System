#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>

 struct student{
    int id;
    char name[50];
    float marks;
};

typedef struct student student;
char filename[50];
student* students = NULL;
int StudentCount=0;
int ismodified=0;
int IdLength = 2;      // Default ID length ("ID")
int NameLength = 4;    // Default Name length ("Name")
int MarksLength = 5;   // Default Marks length ("Marks")

void updateColumnWidth()
{
  IdLength=2;
   NameLength = 4; 
    MarksLength = 5;
    for(int i=0;i<StudentCount;i++)
    {
      int idWidth=snprintf(NULL,0,"%d",students[i].id);
      if(idWidth>IdLength) IdLength=idWidth;

      int nameWidth=strlen(students[i].name);
      if(nameWidth>NameLength) NameLength=nameWidth;

    }
}

void printdash(FILE*file)
{
 
     int sNoWidth = snprintf(NULL, 0, "%d", StudentCount) + 1;
   int totalWidth=5+sNoWidth+IdLength+NameLength+12;
     for(int i=0;i<totalWidth;i++)
     fprintf(file,"-");

     fprintf(file,"\n");
}

void clearbuffer()
{
  int c;
    while((c=getchar())!='\n'&&c!=EOF);
}

void addTxtExtension() {
    if (strstr(filename, ".txt") == NULL) {
        strcat(filename, ".txt");
    }
}

// Utility function to trim trailing whitespace
void trimTrailingSpaces(char *str) {
    int index, i;
    index = -1;

    // Find last non-white space character
    for (i = 0; str[i] != '\0'; ++i) {
        if (!isspace(str[i])) {
            index = i;
        }
    }
    // Mark the new end of the string
    str[index + 1] = '\0';
}

int validateNumber(char *input) {
    // Check if the string is empty
    if (input[0] == '\0') return 0;

    // Loop through each character to validate it's a digit
    for (int i = 0; input[i] != '\0'; i++) {
        if (!isdigit(input[i])) {
            return 0;
        }
    }
    return 1;
}


int validateName(char *input) {
    for (int i = 0; input[i] != '\0'; i++) {
        if (!isalpha(input[i]) && input[i] != ' ') {
            return 0; // Contains invalid characters
        }
    }
    return 1; // Valid name
}

int validateMarks(float marks) {
    return (marks >= 0 && marks <= 100);
}

void loadStudentFromFile()
{
  FILE* file=fopen(filename,"r");
  if(file==NULL)
  {
    printf("Error opening file. Starting with an empty list.\n");
    return;
  }
  char headerline[100];
  fgets(headerline,sizeof(headerline),file);
  fgets(headerline,sizeof(headerline),file);

  
  student temp;
  
  int sNo;

  while(fscanf(file, " %d | %d | %[^|] | %f\n", &sNo, &temp.id, temp.name, &temp.marks) == 4)
  {
    trimTrailingSpaces(temp.name);
    students=realloc(students,(StudentCount+1)*sizeof(student));
    if(students==NULL)
    {
      printf("Memory allocation failed.\n");
      exit(1);
    }
    students[StudentCount++]=temp;
  }
  fclose(file);
}

void sortAndSaveToFile()
{
  FILE* file= fopen(filename,"w");
  if(file==NULL)
  {
    printf("error opening file.\n");
    return;
  }


  updateColumnWidth();
  // Calculate the width of S.No based on StudentCount
    int sNoWidth = snprintf(NULL, 0, "%d", StudentCount) + 1;

     fprintf(file, "%-*s | %-*s | %-*s | %-*s\n",sNoWidth-1, "S.No", IdLength, "ID", NameLength, "Name", MarksLength, "Marks");
      printdash(file);


  for(int i=0;i<StudentCount-1;i++)
  {
    for(int j=0;j<StudentCount-i-1;j++)
    {
      if(strcmp(students[j].name,students[j+1].name)>0)
      {
        student temp=students[j];
        students[j]=students[j+1];
        students[j+1]=temp;
      }
    }
  }


    for (int i = 0; i < StudentCount; i++)
    {
        // Writing sorted student data to the file
        fprintf(file, " %-*d | %-*d | %-*s | %.2f\n",sNoWidth+1,i+1,IdLength, students[i].id,NameLength, students[i].name, students[i].marks);
    }
     printdash(file);

     fflush(file);
  fclose(file);
     printf("Data Saved in File.\n");
  
}

void addStudent()
{
  student newStudent;
  char IDinput[20];
  int idExists=0;

  do {
        printf("\nEnter student ID (numbers only): ");
        fgets(IDinput, sizeof(IDinput), stdin);
        IDinput[strcspn(IDinput, "\n")] = '\0'; // Removing newline character

        if (!validateNumber(IDinput)) {
            printf("Invalid ID! Please enter a valid numeric value.\n");
        }else{
          newStudent.id=atoi(IDinput);

          //check for unique ID

          idExists=0;
          for(int i=0;i<StudentCount;i++)
          {
            if(students[i].id==newStudent.id)
            {
              idExists=1;
              break;
            }
          }

          if(idExists)
          {
            printf("\nID already exists! Please enter a unique ID.\n");
          }

        }

    } while(idExists|| !validateNumber(IDinput));


  // for name
   do{
    printf("\bEnter student name(alphabets and space only): ");
   fgets(newStudent.name, sizeof(newStudent.name), stdin);
        newStudent.name[strcspn(newStudent.name, "\n")] = '\0';

        if (!validateName(newStudent.name)) {
            printf("Invalid Name! Please enter only alphabets and spaces.\n");
  }}while(!validateName(newStudent.name));

  // Marks Input
    do {
        printf("Enter marks (0 to 100): ");
        scanf("%f", &newStudent.marks);
        clearbuffer();
        
        if (!validateMarks(newStudent.marks)) {
            printf("Invalid Marks! Please enter a value between 0 and 100.\n");
        }
    } while (!validateMarks(newStudent.marks));

  students = realloc(students,(StudentCount+1)*sizeof(student));
  {
    if(students==NULL)
    {
      printf("Memory allocatiion failed.\n");
      exit(1);
    }
  }
  students[StudentCount++] = newStudent;

  ismodified=1;
  printf("Student Record has been updated.\n");
}

void displayStudents()
{
  FILE* file=fopen(filename,"r");
  if(file==NULL)
  {
    printf("Error opening file.\n");
    return;
  }
  char line[500];
  
  printf("\nDisplaying All Student Record \n");

  while(fgets(line, sizeof(line),file))
  {
    printf("%s",line);
  }
  fclose(file);
  return;
}

int searchStudents()
{
  char searchTerm[50];
  int found = 0;
  int isNumericSearch=1;
 
  fgets(searchTerm,sizeof(searchTerm),stdin);
  searchTerm[strcspn(searchTerm,"\n")]='\0';

  for(int i=0;searchTerm[i]!='\0';i++)
  {
    if (!isdigit(searchTerm[i]))
    {
      isNumericSearch=0;
      break;
    }
  }

  printf("\nsearching for %s\n",searchTerm);
    // trailing space for searching.
  if(!isNumericSearch)
 {
  trimTrailingSpaces(searchTerm);
 }
  for(int i=0;i<StudentCount;i++)
  {
    if((validateNumber(searchTerm)&&atoi(searchTerm)==students[i].id)||(validateName(searchTerm)&&strcmp(searchTerm,students[i].name)==0))
    {

      found=1;
      printf("Student record found for %s.\n",searchTerm);
      printf("ID: %d, Name: %s, Marks: %.2f\n",students[i].id,students[i].name,students[i].marks);
    }
  }
  if(!found)
  {
  
    printf("No record found for %s.\n",searchTerm);
    return 0;
  }
  else 
  return 1;
}

void updateStudents()
{
    int found = 0;
    int isNumericSearch = 1;
    char searchTerm[50];

    printf("enter student ID or Name to update: ");
    fgets(searchTerm, sizeof(searchTerm), stdin);
    searchTerm[strcspn(searchTerm, "\n")] = '\0';

    // Search through the array of students
    for (int i = 0; i < StudentCount; i++)
    {
        if ((validateNumber(searchTerm) && students[i].id == atoi(searchTerm)) || 
            (!validateNumber(searchTerm) && strcmp(students[i].name, searchTerm) == 0))
        {
            found = 1;
            printf("\nStudent found:\n");
            printf("ID: %d, Name: %s, Marks: %.2f\n", students[i].id, students[i].name, students[i].marks);

            char IDinput[20];
            int idExists=0;
          //enter new id 

        do {
        printf("\nEnter student New ID (numbers only): ");
        fgets(IDinput, sizeof(IDinput), stdin);
        IDinput[strcspn(IDinput, "\n")] = '\0'; // Removing newline character

        if (!validateNumber(IDinput)) {
            printf("Invalid ID! Please enter a valid numeric value.\n");
        }else{

          //check for unique ID

          idExists=0;
          for(int j=0;j<StudentCount;j++)
          {
            if(students[j].id==atoi(IDinput)&&students[j].id!=students[i].id)
            {
              idExists=1;
              break;
            }
          }

          if(idExists)
          {
            printf("\nID already exists! Please enter a unique ID.\n");
          }
          else{
            students[i].id=atoi(IDinput);
            break;
          }
        }
       } while (!validateNumber(IDinput)||atoi(IDinput) <=0||idExists);
  

              // Student new Name 
    do {
        printf("Enter student New name (alphabets and spaces only): ");
        fgets(students[i].name, sizeof(students[i].name), stdin);
        students[i].name[strcspn(students[i].name, "\n")] = '\0';

        if (!validateName(students[i].name)) {
            printf("Invalid Name! Please enter only alphabets and spaces.\n");
        }
    } while (!validateName(students[i].name));

     // Marks Input
    do {
        printf("Enter New marks (0 to 100): ");
        scanf("%f", &students[i].marks);
        clearbuffer();
        
        if (!validateMarks(students[i].marks)) {
            printf("Invalid Marks! Please enter a value between 0 and 100.\n");
        }
    } while (!validateMarks(students[i].marks));
  }
}

  if(found)
  {
    
            printf("\nStudent record updated successfully.\n");
            
  }
   

    if (!found)
    {
        printf("\nNo record found for %s.\n", searchTerm);
    }

    // Save the changes to the array
    ismodified=1;

  }


void deleteStudents()
{
  char deleteTerm[50];
  int found=0;
  int isNumericSearch=1;

  printf("Enter Student ID or Name to Delete: ");
  fgets(deleteTerm,sizeof(deleteTerm),stdin);
  deleteTerm[strcspn(deleteTerm,"\n")]='\0';
  
  for(int i=0;deleteTerm[i]!='\0';i++)
  {
    if(!isdigit(deleteTerm[i]))
    {
      isNumericSearch=0;
      break;
    }
  }

  // search and delete logic
  for(int i=0;i<StudentCount;i++)
  {
    if((isNumericSearch&&students[i].id==atoi(deleteTerm))||
       (!isNumericSearch&&strcmp(students[i].name,deleteTerm)==0))
       {
        found=1;
        //shift record
        for(int j=i;j<StudentCount-1;j++)
        {
          students[j] = students[j+1];
        }
        StudentCount--;
        students=realloc(students,StudentCount*sizeof(student));
        {
          if(students==NULL)
          {
            printf("Memory allocation failed.\n");
          }
        }
        printf("\nStudent record deleted successfully.\n");

        //save the updated list to the array
        ismodified=1;
        break;
       }
  } 

  if(!found){
    printf("\nNo record found for %s.\n",deleteTerm);
  }
}

int main()
{
  
  printf("enter file name: ");
  fgets(filename,sizeof(filename),stdin);
  filename[strcspn(filename,"\n")]='\0';

  addTxtExtension();
 
  loadStudentFromFile();

    int choice;
    do{
      printf("\n--- Student Management System ---\n");
      printf(" 1. Add Student\n 2. Display All Students\n 3. Search for a Student\n 4. Update Student Details\n 5. Delete a Student\n 6. Exit\n  Enter your choice: ");
      scanf("%d",&choice);
      clearbuffer();

      switch(choice)
      {
        case 1:
        addStudent();
        break;
        
        case 2:
            if(ismodified){
            sortAndSaveToFile();
            }
             displayStudents();
             break;
        
        case 3:
         printf("enter Student ID or Name to search: ");
        searchStudents();
        break;
        
        case 4:
        updateStudents();
        break;

        case 5:
        deleteStudents();
        break;

        case 6:
        if(ismodified)
        sortAndSaveToFile();
        printf("\nThank you for using the Student Management System!\n");
        exit(1);
        break;

        default:
        printf("Invalid choice.Try again.\n"); 
      }
      }while(1);


      free(students);

       return 0;
}