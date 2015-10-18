#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    // Initialize the MPI environment
    MPI_Init(NULL, NULL);

   
    //printf("argv1 %s\n", argv[1]);
    int temp = atoi(argv[1]);
    //printf("argv1-- %d\n", temp-1);

    //printf("\n");
    // Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Get the rank of the process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    // Get the name of the processor
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);
    
   
    int token=1;
    double startTime, endTime;
    startTime = MPI_Wtime();
    while(temp>0){
    //if the process is not process 0    
    if (world_rank != 0) {
        MPI_Recv(&token, 1, MPI_INT, world_rank - 1, 0, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);
        printf("Process %d received token %d from process %d\n", world_rank, token,
               world_rank - 1);
        if(token%10==0){
            printf("Token %d is a multiple of 10.\n", token);
        }           
        token++; 
        MPI_Send(&token, 1, MPI_INT, (world_rank + 1) % world_size, 0,
                MPI_COMM_WORLD);
      } else {
        
        MPI_Send(&token, 1, MPI_INT, (world_rank + 1) % world_size, 0,
                MPI_COMM_WORLD);
      }
      
      // now process 0 can receive from the last process
      if (world_rank == 0) {
        MPI_Recv(&token, 1, MPI_INT, world_size - 1, 0, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE);
        printf("Process %d received token %d from process %d\n", world_rank, token,
               world_size - 1);
        if(token%10==0){
            printf("Token %d is a multiple of 10.\n", token);
        }  
        token++;           
      }
      temp--;
     
    }
    if(world_rank==0 && temp==0){
    endTime = MPI_Wtime();
    //print the total time
    printf("Total time: %f seconds\n",endTime-startTime);
    }
   
    
    MPI_Finalize();

    
}
