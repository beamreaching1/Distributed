import numpy as np
from mpi4py import MPI
from random import randint

def main():
    comm = MPI.COMM_WORLD
    rank = comm.Get_rank(); 
    size = comm.Get_size(); 
    status = MPI.Status();
    N = 64
    if rank == 0:
        unsorted = [randint(0,N) for i in range(N)]
        print("Unsorted:")
        print(unsorted)
    else:
        unsorted = None

    unsorted = comm.bcast(unsorted, root = 0)
    local_min = rank * (N/size);
    local_max = (rank + 1) * (N/size);
    bucket = []
    if rank == size-1:
        local_max += 1
    counter = 0
    for i in range(N):
        if ((unsorted[i] >= local_min) and (unsorted[i] < local_max)):
            counter += 1
            bucket.append(float(unsorted[i]))
    
    bucket.sort()
    #print("Rank: " + str(rank) + " | Max: " + str(local_max) + " | Min: " + str(local_min) + " | Local N: " + str(counter))
    #print("Rank: " + str(rank) + " " + str(bucket))
    
    counts = np.zeros(size, dtype="int")
    
    counter = np.array(counter)
        
    comm.Gather(counter, counts, root = 0)
    

    displ = np.zeros(size)
    sortf = np.zeros(N)
    displ[0] = 0;
    for i in range(size-1):
        displ[i+1] = displ[i] + counts[i];

    comm.Gatherv(sendbuf=[np.array(bucket), MPI.INT], recvbuf=[sortf, tuple(counts), tuple(displ), MPI.DOUBLE], root=0)
    
    if rank == 0:
        print("\nSorted:")
        print(sortf)
    
if __name__ == "__main__":
    main()