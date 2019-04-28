#include "stdio.h"
#include "lottery.h"

char separator;
FILE* fd;
int drawing[TIP_SIZE];

#define UUID_LEN 40
#define MAX_TIP_LEN 17
#define MAX_LINE_LEN (UUID_LEN + 1 + MAX_TIP_LEN + 1)

bool init_lottery(const char *csv_file, char csv_separator)
{
  fd=fopen(csv_file,"r");
  separator=csv_separator;

  for (int i = 0; i < TIP_SIZE; i++)
  {
    drawing[i]=0;
  }

  return fd!=0;
}


int char_to_int(char* chars, int count)
{
  int n = 0;

  for (int i = 0; i < count; i++)
  {
    n=n*10+chars[i]-'0';
  }

  return n;
}

int search_start(char* line)
{

  for (int i = 0; i < MAX_LINE_LEN; i++)
   {

    if(line[i]==separator)
    {
      return i+1;
    }

  }

  return -1;
}

bool get_tip(int tip_n, int tip[TIP_SIZE])
{
  char line[MAX_LINE_LEN];

  fseek(fd, 0 , SEEK_SET);

  if(tip_n<0)
  {
    return false;
  }

  for (int i = 0; i <= tip_n; i++)
   {
    if(fgets(line, MAX_LINE_LEN, fd) == 0)
    {
      return false;
    }

  }

  int indexOfFiles = 0;
  int count = 0;

  for (int i = search_start(line); i < MAX_LINE_LEN; i++)
   {
    char numbers[2];

    if(line[i]==separator || line[i]=='\n' || line[i]=='\0')
    {

      tip[indexOfFiles++]=char_to_int(numbers,count);
      count=0;

      if(indexOfFiles==TIP_SIZE)
      {
        break;
      }
    }

    else
    {
      numbers[count++]=line[i];
    }
  }

  return true;
}


bool set_drawing(int drawing_n[TIP_SIZE])
{
  for (int i = 0; i < TIP_SIZE; i++)
   {

    if(drawing_n[i]<1 || drawing_n[i] > 45)
    {
      return false;
    }

    drawing[i]=drawing_n[i];
  }

  return true;
}

bool is_in_drawings(int tip)
{

  for (int i = 0; i < TIP_SIZE; i++)
  {

    if(tip==drawing[i])
    {
      return true;
    }
  }

  return false;
}

int get_tip_result(int tip_n)
{
  if(drawing[0]==0)
  {
    return -1;
  }

  int tips[TIP_SIZE];

  if(!get_tip(tip_n  ,tips))
  {
    return -2;
  }

  int count=0;

  for (int i = 0; i < TIP_SIZE; i++)
  {
    if(is_in_drawings(tips[i]))
    {
      count++;
    }
  }

  return count;
}

int get_right_tips_count(int right_digits_count)
{

  if (right_digits_count<0 || right_digits_count>TIP_SIZE || drawing[0]==0)
  {
    return -1;
  }

  int count=0;
  fseek(fd, 0 , SEEK_SET);
  char line[MAX_LINE_LEN];

  for (int i = 0; i < 1000000; i++)
  {
    if(fgets(line, MAX_LINE_LEN, fd)==0)
    {
      return count;
    }

    if(get_tip_result(i)==right_digits_count)
    {
      count++;
    }
  }

  return count;
}
