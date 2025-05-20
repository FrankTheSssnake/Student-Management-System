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

void clearbuffer()
{
    while(getchar()!='\n');
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

   
  fprintf(file,"S.No  | ID    | Name            | Marks\n");
    fprintf(file, "--------------------------------------------\n");
    for (int i = 0; i < StudentCount; i++)
    {
        // Writing sorted student data to the file
        fprintf(file, " %-4d | %-5d | %-15s | %.2f\n", i + 1, students[i].id, students[i].name, students[i].marks);
    }
     fprintf(file, "--------------------------------------------\n");

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
  students[StudentCount++] = newStudent;

  sortAndSaveToFile();
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
  char line[300];
  
  printf("\n------ Displaying All Student Record -------\n");

  if(StudentCount==0)
  {
    printf("No Students found.\n");
    return;
  }
   
  while(fgets(line, sizeof(line),file))
  {
    printf("%s",line);
  }
  fclose(file);


  return;
}

int searchStudents()
{
  char line[300];
  char headerline[300];
  char searchTerm[50];
  int found = 0;
  int isNumericSearch=1;

 FILE* file= fopen(filename,"r");

  if(file==NULL)
  {
    printf("error in opening file.\n");
    return 0;
  }

  fgets(headerline, sizeof(headerline),file);
  fgets(headerline,sizeof(headerline),file);

 
 
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
   

  while(fgets(line,sizeof(line),file))
  {
    int sNo, id;
    char name[50];
    float marks;

    int read = sscanf(line," %d | %d | %[^|] | %f",&sNo,&id,name,&marks);
     if(read==4)
    {
      name[strcspn(name," ")]='\0';

     if((isNumericSearch&& id==atoi(searchTerm))||(!isNumericSearch && strcmp(name,searchTerm)==0))
     {
      printf("\nfound record:\n");
        printf("S.No  | ID    | Name            | Marks\n");
    printf( "--------------------------------------------\n");
       printf(" %-4d | %-5d | %-15s | %.2f\n", sNo, id, name, marks);
      found=1;
      break;
     }
    }
  }
  if(!found)
  {
  fclose(file);
    printf("No record found for %s.\n",searchTerm);
    return 0;
  }

  if(found)
  {
    fclose(file);
    return 1;
  }

  
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

    // Save the changes to the file
    sortAndSaveToFile();

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
        printf("\nStudent record deleted successfully.\n");

        //save the updated list in file
        sortAndSaveToFile();
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
        printf("\nThank you for using the Student Management System!\n");
;
        break;

        
        default:
        printf("Invalid choice.Try again.\n"); 
      }
    }while(choice!=6||choice==1||choice==2||choice==3||choice==4||choice==5);


    free(students);

    return 0;
}