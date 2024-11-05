#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/queue.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

typedef uint32_t u32;
typedef int32_t i32;

struct process
{
  u32 pid;
  u32 arrival_time;
  u32 burst_time;

  TAILQ_ENTRY(process) pointers;

  /* Additional fields here */
  bool neverRan; 
  int completeTime; 
  u32 original_burst_time; 
  /* End of "Additional fields here" */
};

TAILQ_HEAD(process_list, process);

u32 next_int(const char **data, const char *data_end)
{
  u32 current = 0;
  bool started = false;
  while (*data != data_end)
  {
    char c = **data;

    if (c < 0x30 || c > 0x39)
    {
      if (started)
      {
        return current;
      }
    }
    else
    {
      if (!started)
      {
        current = (c - 0x30);
        started = true;
      }
      else
      {
        current *= 10;
        current += (c - 0x30);
      }
    }

    ++(*data);
  }

  printf("Reached end of file while looking for another integer\n");
  exit(EINVAL);
}

u32 next_int_from_c_str(const char *data)
{
  char c;
  u32 i = 0;
  u32 current = 0;
  bool started = false;
  while ((c = data[i++]))
  {
    if (c < 0x30 || c > 0x39)
    {
      exit(EINVAL);
    }
    if (!started)
    {
      current = (c - 0x30);
      started = true;
    }
    else
    {
      current *= 10;
      current += (c - 0x30);
    }
  }
  return current;
}

void init_processes(const char *path,
                    struct process **process_data,
                    u32 *process_size)
{
  int fd = open(path, O_RDONLY);
  if (fd == -1)
  {
    int err = errno;
    perror("open");
    exit(err);
  }

  struct stat st;
  if (fstat(fd, &st) == -1)
  {
    int err = errno;
    perror("stat");
    exit(err);
  }

  u32 size = st.st_size;
  const char *data_start = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
  if (data_start == MAP_FAILED)
  {
    int err = errno;
    perror("mmap");
    exit(err);
  }

  const char *data_end = data_start + size;
  const char *data = data_start;

  *process_size = next_int(&data, data_end);

  *process_data = calloc(sizeof(struct process), *process_size);
  if (*process_data == NULL)
  {
    int err = errno;
    perror("calloc");
    exit(err);
  }

  for (u32 i = 0; i < *process_size; ++i)
  {
    (*process_data)[i].pid = next_int(&data, data_end);
    (*process_data)[i].arrival_time = next_int(&data, data_end);
    (*process_data)[i].burst_time = next_int(&data, data_end);
  }

  munmap((void *)data, size);
  close(fd);
}

int main(int argc, char *argv[])
{ 
  if (argc != 3)
  {
    return EINVAL;
  }
  struct process *data;
  u32 size;
  init_processes(argv[1], &data, &size);

  u32 quantum_length = next_int_from_c_str(argv[2]);

  struct process_list list;
  TAILQ_INIT(&list);

  u32 total_waiting_time = 0;
  u32 total_response_time = 0;

  /* Your code here */
  int totalBurstTime = 0; 
  
  for (int i = 0; i < size; i++)
  {
    totalBurstTime += (data+i)->burst_time; 
    (data + i)->original_burst_time = (data + i)->burst_time;
  }

  for (int i = 0; i < size; i++)
  {
    (data+i)->neverRan = true; 
    (data+i)->completeTime = 0; 
  } 
  int index = 0;
  int time = 0; 
  while (totalBurstTime > 0)
  {
    for (int i = 0; i < size; i++)
    {
      if ((data+i)->arrival_time == time)
      {
        TAILQ_INSERT_TAIL(&list, (data+i), pointers);
        //printf("Added PID:%u, Time:%i \n", TAILQ_LAST(&list, process_list)->pid, time);
      } 
    }
    if (!TAILQ_EMPTY(&list))
    {
      if (TAILQ_FIRST(&list)->neverRan == true)
      {
        total_response_time += (time-TAILQ_FIRST(&list)->arrival_time); 
        TAILQ_FIRST(&list)->neverRan = false; 
      }
      TAILQ_FIRST(&list)->burst_time--;
      totalBurstTime--;
      index++;
      //struct process *temp = TAILQ_FIRST(&list);
      //printf("Time: %i, PID: %u, Arrival Time: %u, Burst Time: %u \n",time, temp->pid, temp->arrival_time, temp->burst_time);
      if (TAILQ_FIRST(&list)->burst_time == 0)
      {
        struct process *first_process = TAILQ_FIRST(&list);
        u32 waiting_time = 1+time-first_process->arrival_time-first_process->original_burst_time; 
        total_waiting_time += waiting_time;
        TAILQ_REMOVE(&list, first_process, pointers);
        index = 0; 
        //printf("%i\n", total_waiting_time); 
      }
      else if(index == quantum_length)
      {
        index = 0;
        struct process *first_process = TAILQ_FIRST(&list);
        TAILQ_REMOVE(&list, first_process, pointers); 
        TAILQ_INSERT_TAIL(&list, first_process, pointers);
      }
      time++;
    }
    else
    {
      time++;
    }
  }




  //struct process *counter = TAILQ_FIRST(&list); 
  //printf("%i", size); 
  // printf("%u", list.tqh_first->pid);
  // printf("%u", list.tqh_first->arrival_time);
  // printf("%u", list.tqh_first->burst_time);
  // printf("%u", list.tqh_first->pid);
  // printf("%u", list.tqh_first->arrival_time);
  // printf("%u", list.tqh_first->burst_time);

  /* End of "Your code here" */

  printf("Average waiting time: %.2f\n", (float)total_waiting_time / (float)size);
  printf("Average response time: %.2f\n", (float)total_response_time / (float)size);

  free(data);
  return 0;
}
